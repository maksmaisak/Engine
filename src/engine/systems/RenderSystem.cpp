//
// Created by Maksym Maisak on 22/10/18.
//

#include "RenderSystem.h"
#include <iostream>
#include <tuple>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <GL/glew.h>
#include "RenderInfo.h"
#include "Transform.h"
#include "Camera.h"
#include "Name.h"
#include "GLHelpers.h"
#include "GameTime.h"
#include "Resources.h"
#include "Material.h"
#include "Exception.h"
#include "Render2DSystem.h"
#include "RenderSkyboxSystem.h"
#include "RenderUISystem.h"

using namespace en;

namespace {

    inline void checkRenderingError(const Actor& actor) {

        if (glCheckError() == GL_NO_ERROR) {
            return;
        }

        const en::Name* const namePtr = actor.tryGet<en::Name>();
        const std::string name = namePtr ? namePtr->value : "unnamed";
        std::cerr << "Error while rendering " << name << std::endl;
    }

    void GLAPIENTRY messageCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam
    ) {

        fprintf(
            stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message
        );
    }

    void enableDebug() {

        glEnable(GL_DEBUG_OUTPUT);
        glCheckError();
        glDebugMessageCallback(messageCallback, 0);
    }

    const float MAX_SHADOW_DISTANCE = 300.f;
    const glm::vec3 SHADOW_CASTERS_BOUNDS_PADDING = {5, 5, 5};
}

RenderSystem::RenderSystem() :
    m_directionalDepthShader(Resources<ShaderProgram>::get("depthDirectional")),
    m_positionalDepthShader (Resources<ShaderProgram>::get("depthPositional")),
    m_enableStaticBatching(true),
    m_enableDebugOutput(false),
    m_shadowReceiversBounds()
{}

void RenderSystem::start() {

    getConfigFromLua();

    setOpenGLSettings();

    m_debugHud = std::make_unique<DebugHud>(*m_engine, m_renderingSharedState.vertexRenderer);

    addSystem<RenderSkyboxSystem>();
    addSystem<Render2DSystem>();
    addSystem<RenderUISystem>(m_renderingSharedState);
    CompoundSystem::start();
}

void RenderSystem::draw() {

    if (glCheckError() != GL_NO_ERROR) {
        std::cerr << "Uncaught openGL error(s) before rendering." << std::endl;
    }

    if (m_enableStaticBatching) {
        updateBatches();
    }

    updateShadowCastersBounds();
    updateDepthMaps();
    renderEntities();
    CompoundSystem::draw();

    if (m_enableDebugOutput) {
        renderDebug();
    }
}

void RenderSystem::receive(const SceneManager::OnSceneClosed& info) {

    m_batches.clear();
}

void RenderSystem::receive(const sf::Event& event) {

    if (event.type == sf::Event::EventType::Resized) {
        // Make viewport match window size.
        glViewport(0, 0, event.size.width, event.size.height);
    }
}

void RenderSystem::setOpenGLSettings() {

    glEnable(GL_DEPTH_TEST);

    // Counterclockwise vertex order
    glFrontFace(GL_CCW);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set the default blend mode
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0, 0, 0, 1);

    // Convert output from fragment shaders from linear to sRGB
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void RenderSystem::getConfigFromLua() {

    auto& lua = m_engine->getLuaState();
    {
        lua_getglobal(lua, "Config");
        auto popConfig = lua::PopperOnDestruct(lua);

        m_renderingSharedState.referenceResolution = lua.tryGetField<glm::vec2>("referenceResolution").value_or(m_renderingSharedState.referenceResolution);
        m_enableStaticBatching = lua.tryGetField<bool>("enableStaticBatching").value_or(m_enableStaticBatching);
        m_enableDebugOutput    = lua.tryGetField<bool>("enableDebugOutput").value_or(m_enableDebugOutput);
    }
}

void RenderSystem::updateBatches() {

    const auto findBatchMesh = [&batches = m_batches](const std::shared_ptr<en::Material>& material) -> en::Mesh& {

        const auto foundIt = batches.find(material);
        if (foundIt != batches.end()) {
            return foundIt->second;
        }

        const auto [it, didEmplace] = batches.emplace(std::make_pair(material, Mesh{}));
        assert(didEmplace);
        return it->second;
    };

    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (renderInfo.isInBatch || !renderInfo.isBatchingStatic || !renderInfo.material || !renderInfo.model) {
            continue;
        }

        en::Mesh& batchMesh = findBatchMesh(renderInfo.material);
        const glm::mat4& worldMatrix = m_registry->get<Transform>(e).getWorldTransform();
        for (const en::Mesh& mesh : renderInfo.model->getMeshes()) {
            batchMesh.add(mesh, worldMatrix);
        }

        renderInfo.isInBatch = true;
    }

    for (auto& [material, batch] : m_batches) {
        batch.updateBuffers();
    }
}

void RenderSystem::updateDepthMaps() {

    std::vector<Entity> directionalLights;
    std::vector<Entity> pointLights;
    for (Entity lightEntity : m_registry->with<Transform, Light>()) {
        if (m_registry->get<Light>(lightEntity).kind == Light::Kind::DIRECTIONAL) {
            directionalLights.push_back(lightEntity);
        } else {
            pointLights.push_back(lightEntity);
        }
    }

    updateDepthMapsDirectionalLights(directionalLights);
    updateDepthMapsPositionalLights(pointLights);

    // Reset viewport back to the window size.
    const auto size = m_engine->getWindow().getSize();
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError();
}

void RenderSystem::renderEntities() {

    DepthMaps& depthMaps = m_renderingSharedState.depthMaps;

    Actor mainCamera = getMainCamera();
    if (!mainCamera) {
        return;
    }

    const glm::mat4 matrixView = glm::inverse(mainCamera.get<Transform>().getWorldTransform());
    const glm::mat4 matrixProjection = mainCamera.get<Camera>().getCameraProjectionMatrix(*m_engine);

    // Render batches
    for (const auto& [material, mesh] : m_batches) {
        material->render(&mesh, m_engine, &depthMaps, glm::mat4(1), matrixView, matrixProjection);
    }

    // Render entities
    int numSavedByBatching = 0;
    const Material* previousMaterial = nullptr;
    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        const auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (!renderInfo.isEnabled || !renderInfo.material || !renderInfo.model) {
            continue;
        }

        if (renderInfo.isInBatch) {
            numSavedByBatching += renderInfo.model->getMeshes().size();
            continue;
        }

        const glm::mat4& matrixModel = m_registry->get<Transform>(e).getWorldTransform();
        if (renderInfo.material.get() != previousMaterial) {
            renderInfo.material->use(m_engine, &depthMaps, matrixModel, matrixView, matrixProjection);
        } else {
            renderInfo.material->updateModelMatrix(matrixModel);
        }

        for (const Mesh& mesh : renderInfo.model->getMeshes()) {
            renderInfo.material->setAttributesAndDraw(&mesh);
        }

        previousMaterial = renderInfo.material.get();

        checkRenderingError(m_engine->actor(e));
    }

    //std::cout << "Draw calls saved by batching: " << numSavedByBatching - m_batches.size() << '\n';
}

void RenderSystem::renderDebug() {

    m_debugHud->setDebugInfo({m_engine->getFps(), m_engine->getFrameTimeMicroseconds()});
    m_debugHud->draw();
}

Actor RenderSystem::getMainCamera() {

    const Entity entity = m_registry->with<Transform, Camera>().tryGetOne();
    return m_engine->actor(entity);
}

utils::Bounds RenderSystem::getCameraFrustumBounds() {

    const Actor mainCamera = getMainCamera();
    if (!mainCamera) {
        return {glm::vec3(-100.f), glm::vec3(100.f)};
    }

    const glm::mat4 viewToWorld = mainCamera.get<Transform>().getWorldTransform();
    const glm::mat4 clipToView  = glm::inverse(mainCamera.get<Camera>().getCameraProjectionMatrix(*m_engine, MAX_SHADOW_DISTANCE));
    const glm::mat4 clipToWorld = viewToWorld * clipToView;
    const std::array<glm::vec4, 8> corners = {
        clipToWorld * glm::vec4(-1, -1, -1, 1),
        clipToWorld * glm::vec4(-1, -1,  1, 1),
        clipToWorld * glm::vec4(-1,  1, -1, 1),
        clipToWorld * glm::vec4(-1,  1,  1, 1),
        clipToWorld * glm::vec4( 1, -1, -1, 1),
        clipToWorld * glm::vec4( 1, -1,  1, 1),
        clipToWorld * glm::vec4( 1,  1, -1, 1),
        clipToWorld * glm::vec4( 1,  1,  1, 1)
    };

    utils::Bounds bounds = {
        glm::vec3(std::numeric_limits<float>::max()),
        glm::vec3(std::numeric_limits<float>::min())
    };
    for (const glm::vec4& corner : corners) {
        const glm::vec3 cornerWorldspace = corner / corner.w;
        bounds.min = glm::min(bounds.min, cornerWorldspace);
        bounds.max = glm::max(bounds.max, cornerWorldspace);
    }
    return bounds;
}

void RenderSystem::updateShadowCastersBounds() {

    const utils::Bounds constrainingBounds = getCameraFrustumBounds();

    utils::Bounds bounds = {
        glm::vec3(std::numeric_limits<float>::max()),
        glm::vec3(std::numeric_limits<float>::min())
    };
    for (Entity e : m_registry->with<RenderInfo, Transform>()) {

        const auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (!renderInfo.isEnabled || !renderInfo.model || !renderInfo.material)
            continue;

        const glm::vec3 position = m_registry->get<Transform>(e).getWorldPosition();
        if (glm::clamp(position, constrainingBounds.min, constrainingBounds.max) != position)
            continue;

        bounds.min = glm::min(bounds.min, position);
        bounds.max = glm::max(bounds.max, position);
    }

    m_shadowReceiversBounds = bounds;
}

namespace {

    glm::mat4 getDirectionalLightspaceTransform(const Light& light, const Transform& lightTransform, const utils::Bounds& shadowReceiversBounds) {

        const auto& min = shadowReceiversBounds.min - SHADOW_CASTERS_BOUNDS_PADDING;
        const auto& max = shadowReceiversBounds.max + SHADOW_CASTERS_BOUNDS_PADDING;

        const glm::mat4 matrixLightView = glm::lookAt({0, 0, 0}, lightTransform.getForward(), {0, 1, 0});
        const std::array<glm::vec3, 8> corners = {
            matrixLightView * glm::vec4(min.x, min.y, min.z, 1.f),
            matrixLightView * glm::vec4(min.x, min.y, max.z, 1.f),
            matrixLightView * glm::vec4(min.x, max.y, min.z, 1.f),
            matrixLightView * glm::vec4(min.x, max.y, max.z, 1.f),
            matrixLightView * glm::vec4(max.x, min.y, min.z, 1.f),
            matrixLightView * glm::vec4(max.x, min.y, max.z, 1.f),
            matrixLightView * glm::vec4(max.x, max.y, min.z, 1.f),
            matrixLightView * glm::vec4(max.x, max.y, max.z, 1.f)
        };
        utils::Bounds transformedBounds = {
            glm::vec3(std::numeric_limits<float>::max()),
            glm::vec3(std::numeric_limits<float>::min())
        };
        for (const glm::vec3& corner : corners) {
            transformedBounds.min = glm::min(transformedBounds.min, corner);
            transformedBounds.max = glm::max(transformedBounds.max, corner);
        }

        const glm::mat4 lightProjectionMatrix = glm::ortho(
            transformedBounds.min.x, transformedBounds.max.x,
            transformedBounds.min.y, transformedBounds.max.y,
            light.nearPlaneDistance, std::min(transformedBounds.max.z - transformedBounds.min.z, MAX_SHADOW_DISTANCE)
        );

        return lightProjectionMatrix * glm::translate(glm::vec3(0, 0, -transformedBounds.max.z)) * matrixLightView;
    }
}

void RenderSystem::updateDepthMapsDirectionalLights(const std::vector<Entity>& directionalLights) {

    DepthMaps& depthMaps = m_renderingSharedState.depthMaps;

    glViewport(0, 0, depthMaps.getDirectionalMapResolution().x, depthMaps.getDirectionalMapResolution().y);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMaps.getDirectionalMapsFramebufferId());
    glClear(GL_DEPTH_BUFFER_BIT);

    m_directionalDepthShader->use();

    // Set uniforms
    int numLights = 0;
    for (Entity e : directionalLights) {

        if (numLights >= depthMaps.getMaxNumDirectionalLights()) {
            break;
        }

        auto& light = m_registry->get<Light>(e);
        const auto& tf = m_registry->get<Transform>(e);
        light.matrixPV = getDirectionalLightspaceTransform(light, tf, m_shadowReceiversBounds);
        m_directionalDepthShader->setUniformValue("matrixPV[" + std::to_string(numLights) + "]", light.matrixPV);

        numLights += 1;
    }
    m_directionalDepthShader->setUniformValue("numLights", numLights);
    m_directionalDepthShader->setUniformValue("matrixModel", glm::mat4(1));

    // Render batches
    for (const auto& [material, mesh] : m_batches) {
        mesh.render(0, -1, -1);
    }

    // Render entities
    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (renderInfo.isInBatch || !renderInfo.isEnabled || !renderInfo.model) {
            continue;
        }

        const glm::mat4& modelTransform = m_registry->get<Transform>(e).getWorldTransform();
        m_directionalDepthShader->setUniformValue("matrixModel", modelTransform);
        for (const Mesh& mesh : renderInfo.model->getMeshes()) {
            mesh.render(0, -1, -1);
        }

        checkRenderingError(m_engine->actor(e));
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

namespace {

    std::tuple<float, glm::vec3, std::array<glm::mat4, 6>> getPointLightUniforms(const DepthMaps& depthMaps, const Light& light, const Transform& lightTransform) {

        const float nearPlaneDistance = light.nearPlaneDistance;
        const float farPlaneDistance  = light.farPlaneDistance;
        const glm::mat4 lightProjectionMatrix = glm::perspective(
            glm::radians(90.f),
            (float)depthMaps.getCubemapResolution().x / (float)depthMaps.getCubemapResolution().y,
            nearPlaneDistance,
            farPlaneDistance
        );

        const glm::vec3 lightPosition = lightTransform.getWorldPosition();

        return {
            farPlaneDistance,
            lightPosition,
            {
                lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
                lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
                lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)),
                lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
                lightProjectionMatrix * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f))
            }
        };
    }
}

void RenderSystem::updateDepthMapsPositionalLights(const std::vector<Entity>& pointLights) {

    DepthMaps& depthMaps = m_renderingSharedState.depthMaps;

    glViewport(0, 0, depthMaps.getCubemapResolution().x, depthMaps.getCubemapResolution().y);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMaps.getCubemapsFramebufferId());
    glClear(GL_DEPTH_BUFFER_BIT);

    m_positionalDepthShader->use();

    struct Sphere {
        float radius = 0.f;
        glm::vec3 position;
    };

    // Set uniforms, collect info on lightspheres.
    std::vector<Sphere> lightSpheres;
    int i = 0;
    for (Entity e : pointLights) {

        const auto& light = m_registry->get<Light>(e);
        const auto& tf = m_registry->get<Transform>(e);
        const auto [farPlaneDistance, lightPosition, pvMatrices] = getPointLightUniforms(depthMaps, light, tf);
        lightSpheres.push_back({light.range, lightPosition});

        for (unsigned int face = 0; face < 6; ++face) {
            m_positionalDepthShader->setUniformValue("matrixPV[" + std::to_string(i * 6 + face) + "]", pvMatrices[face]);
        }
        std::string prefix = "lights[" + std::to_string(i) + "].";
        m_positionalDepthShader->setUniformValue(prefix + "position", lightPosition);
        m_positionalDepthShader->setUniformValue(prefix + "farPlaneDistance", farPlaneDistance);

        if (++i >= depthMaps.getMaxNumPositionalLights()) {
            break;
        }
    }
    m_positionalDepthShader->setUniformValue("numLights", i);

    // Render entities
    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        const auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (!renderInfo.isEnabled || !renderInfo.model) {
            continue;
        }

        // If not in range of any lights, skip.
        const glm::mat4& modelTransform = m_registry->get<Transform>(e).getWorldTransform();
        if (std::none_of(lightSpheres.begin(), lightSpheres.end(), [pos = glm::vec3(modelTransform[3])](const Sphere& sphere){
            return glm::distance2(pos, sphere.position) < sphere.radius * sphere.radius;
        }))
            continue;

        m_positionalDepthShader->setUniformValue("matrixModel", modelTransform);
        for (const Mesh& mesh : renderInfo.model->getMeshes()) {
            mesh.render(0);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
