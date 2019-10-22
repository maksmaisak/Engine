//
// Created by Maksym Maisak on 2/10/19.
//

#include "ShadowMapper.h"

#include <limits>
#include "Camera.h"
#include "Entity.h"
#include "Transform.h"
#include "Light.h"
#include "RenderingSharedState.h"
#include "Engine.h"
#include "RenderInfo.h"
#include "ScopedBind.h"

using namespace en;

namespace {

    constexpr float MaxShadowDistance = 300.f;
    constexpr float ShadowCastersBoundsPadding = 5.f;

    Bounds3D computeCameraFrustumBounds(Engine& engine) {

        const Actor mainCamera = engine.getMainCamera();
        if (!mainCamera) {
            return {};
        }

        // Convert the 8 corners of the frustum from clipspace to worldspace.
        const glm::mat4 viewToWorld = mainCamera.get<Transform>().getWorldTransform();
        const glm::mat4 clipToView  = glm::inverse(mainCamera.get<Camera>().getCameraProjectionMatrix(engine, MaxShadowDistance));
        const glm::mat4 clipToWorld = viewToWorld * clipToView;
        const glm::vec4 corners[] {
            clipToWorld * glm::vec4(-1, -1, -1, 1),
            clipToWorld * glm::vec4(-1, -1,  1, 1),
            clipToWorld * glm::vec4(-1,  1, -1, 1),
            clipToWorld * glm::vec4(-1,  1,  1, 1),
            clipToWorld * glm::vec4( 1, -1, -1, 1),
            clipToWorld * glm::vec4( 1, -1,  1, 1),
            clipToWorld * glm::vec4( 1,  1, -1, 1),
            clipToWorld * glm::vec4( 1,  1,  1, 1)
        };

        Bounds3D bounds {
            glm::vec3(std::numeric_limits<float>::max()),
            glm::vec3(std::numeric_limits<float>::lowest())
        };
        for (const glm::vec4& corner : corners) {
            // perspective division.
            bounds.add(corner / corner.w);
        }

        return bounds;
    }

    /// Computes the worldspace AABB of all shadow-receiving entities in the camera frustum.
    /// If there aren't any such entities in the camera frustum, returns false.
    bool computeShadowReceiversBounds(Engine& engine, Bounds3D& outBounds) {

        const Bounds3D constrainingBounds = computeCameraFrustumBounds(engine);

        outBounds = {
            glm::vec3(std::numeric_limits<float>::max()),
            glm::vec3(std::numeric_limits<float>::lowest())
        };

        bool anyShadowReceivers = false;
        const EntityRegistry& registry = engine.getRegistry();
        for (Entity e : registry.with<RenderInfo, Transform>()) {

            if (!registry.get<RenderInfo>(e).isValidForRendering()) {
                continue;
            }

            const glm::vec3 position = registry.get<Transform>(e).getWorldPosition();
            if (!constrainingBounds.contains(position)) {
                continue;
            }

            outBounds.add(position);
            anyShadowReceivers = true;
        }

        return anyShadowReceivers;
    }

    glm::mat4 getDirectionalLightspaceTransform(const Light& light, const Transform& lightTransform, const Bounds3D& shadowReceiversBounds) {

        const glm::mat4 worldToLightView = glm::lookAt(glm::vec3(0.f), lightTransform.getForward(), {0.f, 1.f, 0.f});
        const glm::vec3& min = shadowReceiversBounds.min - ShadowCastersBoundsPadding;
        const glm::vec3& max = shadowReceiversBounds.max + ShadowCastersBoundsPadding;
        const glm::vec3 corners[] {
            worldToLightView * glm::vec4(min.x, min.y, min.z, 1.f),
            worldToLightView * glm::vec4(min.x, min.y, max.z, 1.f),
            worldToLightView * glm::vec4(min.x, max.y, min.z, 1.f),
            worldToLightView * glm::vec4(min.x, max.y, max.z, 1.f),
            worldToLightView * glm::vec4(max.x, min.y, min.z, 1.f),
            worldToLightView * glm::vec4(max.x, min.y, max.z, 1.f),
            worldToLightView * glm::vec4(max.x, max.y, min.z, 1.f),
            worldToLightView * glm::vec4(max.x, max.y, max.z, 1.f)
        };

        Bounds3D transformedBounds {
            glm::vec3(std::numeric_limits<float>::max()),
            glm::vec3(std::numeric_limits<float>::lowest())
        };
        for (const glm::vec3& corner : corners) {
            transformedBounds.add(corner);
        }

        const glm::mat4 lightProjectionMatrix = glm::ortho(
            transformedBounds.min.x, transformedBounds.max.x,
            transformedBounds.min.y, transformedBounds.max.y,
            light.nearPlaneDistance, glm::min(transformedBounds.max.z - transformedBounds.min.z, MaxShadowDistance)
        );
        return lightProjectionMatrix * glm::translate(glm::vec3(0, 0, -transformedBounds.max.z)) * worldToLightView;
    }

    std::array<glm::mat4, 6> getPointLightPVTransforms(const DepthMaps& depthMaps, const Light& light, const glm::vec3& lightPosition) {

        const float nearPlaneDistance = light.nearPlaneDistance;
        const glm::mat4 lightProjectionMatrix = glm::perspective(
            glm::radians(90.f),
            (float)depthMaps.getCubemapResolution().x / (float)depthMaps.getCubemapResolution().y,
            nearPlaneDistance,
            light.farPlaneDistance
        );

        return {
            lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
            lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
            lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)),
            lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
            lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f))
        };
    }
}

ShadowMapper::ShadowMapper(Engine& engine, std::shared_ptr<RenderingSharedState> renderingSharedState) :
    m_engine(&engine),
    m_renderingSharedState(std::move(renderingSharedState)),
    m_directionalDepthShader(Resources<ShaderProgram>::get("depthDirectional")),
    m_positionalDepthShader (Resources<ShaderProgram>::get("depthPositional"))
{
    assert(m_renderingSharedState);
}

void ShadowMapper::updateDepthMaps() {

    assert(m_engine);
    assert(m_renderingSharedState);

    if (!computeShadowReceiversBounds(*m_engine, m_shadowReceiversBounds)) {
        return;
    }

    std::vector<Entity> directionalLights;
    std::vector<Entity> pointLights;

    EntityRegistry& registry = m_engine->getRegistry();
    for (Entity lightEntity : registry.with<Transform, Light>()) {
        if (registry.get<Light>(lightEntity).kind == Light::Kind::DIRECTIONAL) {
            directionalLights.push_back(lightEntity);
        } else {
            pointLights.push_back(lightEntity);
        }
    }

    updateDepthMapsDirectionalLights(directionalLights);
    updateDepthMapsPositionalLights(pointLights);
}

void ShadowMapper::updateDepthMapsDirectionalLights(const std::vector<Entity>& lightEntities) {

    DepthMaps& depthMaps = m_renderingSharedState->depthMaps;
    EntityRegistry& registry = m_engine->getRegistry();

    glViewport(0, 0, depthMaps.getDirectionalMapResolution().x, depthMaps.getDirectionalMapResolution().y);
    const auto framebufferBind = gl::ScopedBind(depthMaps.getDirectionalMapsFramebuffer(), GL_FRAMEBUFFER);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_directionalDepthShader->use();

    // Set uniforms
    int numLights = 0;
    for (Entity e : lightEntities) {

        if (numLights >= depthMaps.getMaxNumDirectionalLights()) {
            break;
        }

        auto& light = registry.get<Light>(e);
        light.matrixPV = getDirectionalLightspaceTransform(light, registry.get<Transform>(e), m_shadowReceiversBounds);
        m_directionalDepthShader->setUniformValue("matrixPV[" + std::to_string(numLights) + "]", light.matrixPV);

        numLights += 1;
    }
    m_directionalDepthShader->setUniformValue("numLights", numLights);
    m_directionalDepthShader->setUniformValue("matrixModel", glm::mat4(1));

    // Render batches
    for (const auto& [material, mesh] : m_renderingSharedState->batches) {
        mesh.render(0);
    }

    // Render entities
    for (Entity e : registry.with<Transform, RenderInfo>()) {

        auto& renderInfo = registry.get<RenderInfo>(e);
        if (renderInfo.isInBatch || !renderInfo.isEnabled || !renderInfo.model) {
            continue;
        }

        m_directionalDepthShader->setUniformValue("matrixModel", registry.get<Transform>(e).getWorldTransform());
        for (const Mesh& mesh : renderInfo.model->getMeshes()) {
            mesh.render(0);
        }
    }
}

void ShadowMapper::updateDepthMapsPositionalLights(const std::vector<Entity>& lightEntities) {

    DepthMaps& depthMaps = m_renderingSharedState->depthMaps;
    EntityRegistry& registry = m_engine->getRegistry();

    glViewport(0, 0, depthMaps.getCubemapResolution().x, depthMaps.getCubemapResolution().y);
    const auto framebufferBind = gl::ScopedBind(depthMaps.getCubemapsFramebuffer(), GL_FRAMEBUFFER);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_positionalDepthShader->use();

    struct Sphere {
        float radius = 0.f;
        glm::vec3 position;
    };

    // Set uniforms, collect info on lightspheres
    std::vector<Sphere> lightSpheres;
    int i = 0;
    for (Entity e : lightEntities) {

        const auto& light = registry.get<Light>(e);
        const glm::vec3 lightPosition = registry.get<Transform>(e).getWorldPosition();

        const std::array<glm::mat4, 6> pvMatrices = getPointLightPVTransforms(depthMaps, light, lightPosition);
        for (unsigned int face = 0; face < 6; ++face) {
            m_positionalDepthShader->setUniformValue("matrixPV[" + std::to_string(i * 6 + face) + "]", pvMatrices[face]);
        }

        lightSpheres.push_back({light.range, lightPosition});

        const std::string prefix = "lights[" + std::to_string(i) + "].";
        m_positionalDepthShader->setUniformValue(prefix + "position", lightPosition);
        m_positionalDepthShader->setUniformValue(prefix + "farPlaneDistance", light.farPlaneDistance);

        if (++i >= depthMaps.getMaxNumPositionalLights()) {
            break;
        }
    }
    m_positionalDepthShader->setUniformValue("numLights", i);

    // Render entities
    for (Entity e : registry.with<Transform, RenderInfo>()) {

        const auto& renderInfo = registry.get<RenderInfo>(e);
        if (!renderInfo.isEnabled || !renderInfo.model) {
            continue;
        }

        const glm::mat4& modelTransform = registry.get<Transform>(e).getWorldTransform();

        // If not in range of any lights, skip.
        const auto isInSphereRange = [pos = glm::vec3(modelTransform[3])](const Sphere& sphere) {
            return glm::distance2(pos, sphere.position) < sphere.radius * sphere.radius;
        };
        if (std::none_of(lightSpheres.begin(), lightSpheres.end(), isInSphereRange)) {
            continue;
        }

        m_positionalDepthShader->setUniformValue("matrixModel", modelTransform);
        for (const Mesh& mesh : renderInfo.model->getMeshes()) {
            mesh.render(0);
        }
    }
}
