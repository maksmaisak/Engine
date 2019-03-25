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
#include "RenderInfo.h"
#include "Transform.h"
#include "Camera.h"
#include "Name.h"
#include "Sprite.h"
#include "Text.h"
#include "GLHelpers.h"
#include "GameTime.h"
#include "Resources.h"
#include "Material.h"
#include "Exception.h"
#include "UIRect.h"

using namespace en;

namespace {

    void checkRenderingError(const Actor& actor) {

        if (glCheckError() == GL_NO_ERROR)
            return;

        auto* namePtr = actor.tryGet<en::Name>();
        std::string name = namePtr ? namePtr->value : "unnamed";
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

    std::shared_ptr<Texture> getDefaultSkybox(LuaState& lua) {

        lua_getglobal(lua, "Config");
        auto popConfig = PopperOnDestruct(lua);
        if (lua_isnil(lua, -1))
            return nullptr;

        lua_getfield(lua, -1, "defaultSkybox");
        auto popSkybox = PopperOnDestruct(lua);
        if (lua_isnil(lua, -1))
            return nullptr;

        static std::string keys[] = {
            "right",
            "left",
            "top",
            "bottom",
            "front",
            "back"
        };

        std::array<std::string, 6> imagePaths;
        for (int i = 0; i < 6; ++i) {

            std::optional<std::string> path = lua.tryGetField<std::string>(keys[i]);
            if (!path)
                return nullptr;

            imagePaths[i] = "assets/" + *path;
        }

        return Resources<Texture>::get("defaultSkybox", imagePaths);
    }

    const float MAX_SHADOW_DISTANCE = 300.f;
    const glm::vec3 SHADOW_CASTERS_BOUNDS_PADDING = {5, 5, 5};
}

RenderSystem::RenderSystem() :
    m_directionalDepthShader(Resources<ShaderProgram>::get("depthDirectional")),
    m_positionalDepthShader (Resources<ShaderProgram>::get("depthPositional")),
    m_depthMaps(4, {1024, 1024}, 10, {64, 64}),
    m_vertexRenderer(4096)
{}

void RenderSystem::start() {

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

    m_debugHud = std::make_unique<DebugHud>(*m_engine, m_vertexRenderer);

    auto& lua = m_engine->getLuaState();
    {
        lua_getglobal(lua, "Config");
        auto popConfig = lua::PopperOnDestruct(lua);

        m_referenceResolution  = lua.tryGetField<glm::vec2>("referenceResolution").value_or(glm::vec2(1920, 1080));
        m_enableStaticBatching = lua.tryGetField<bool>("enableStaticBatching").value_or(true);
        m_enableDebugOutput    = lua.tryGetField<bool>("enableDebugOutput").value_or(false);
        m_defaultSkybox        = getDefaultSkybox(lua);
    }

    {
        lua_getglobal(lua, "Game");
        auto popGame = lua::PopperOnDestruct(lua);
        lua.setField("getUIScaleFactor", [this](){return getUIScaleFactor();});
    }
}

namespace {

    glm::mat4 getCameraProjectionMatrix(Engine& engine, Camera& camera, std::optional<float> rangeLimit = std::nullopt) {

        const auto size = engine.getWindow().getSize();
        const float aspectRatio = (float)size.x / size.y;

        const float farPlaneDistance = std::min(rangeLimit.value_or(camera.farPlaneDistance), camera.farPlaneDistance);

        if (camera.isOrthographic) {

            glm::vec2 halfSize = {
                camera.orthographicHalfSize * aspectRatio,
                camera.orthographicHalfSize
            };

            return glm::ortho(
                -halfSize.x, halfSize.x,
                -halfSize.y, halfSize.y,
                camera.nearPlaneDistance, farPlaneDistance
            );
        }

        return glm::perspective(
            glm::radians(camera.fov),
            aspectRatio,
            camera.nearPlaneDistance,
            farPlaneDistance
        );
    }
}

void RenderSystem::draw() {

    if (glCheckError() != GL_NO_ERROR) {
        std::cerr << "Uncaught openGL error(s) before rendering." << std::endl;
    }

    if (m_enableStaticBatching)
        updateBatches();

    updateShadowCastersBounds();
    updateDepthMaps();
    renderEntities();
    renderSkybox();
    renderUI();

    if (m_enableDebugOutput)
        renderDebug();
}

void RenderSystem::updateBatches() {

    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (renderInfo.isInBatch || !renderInfo.isBatchingStatic)
            continue;

        if (!renderInfo.material || !renderInfo.model)
            continue;

        auto it = m_batches.find(renderInfo.material);
        if (it == m_batches.end()) {
            std::tie(it, std::ignore) = m_batches.emplace(std::make_pair(renderInfo.material, Mesh{}));
        }

        const auto& worldMatrix = m_registry->get<Transform>(e).getWorldTransform();
        for (auto& mesh : renderInfo.model->getMeshes())
            it->second.add(mesh, worldMatrix);
        renderInfo.isInBatch = true;
    }

    for (auto& [material, batch] : m_batches) {
        //batch.removeDestroyedEntities();
        batch.updateBuffers();
    }
}

void RenderSystem::updateDepthMaps() {

    std::vector<Entity> directionalLights;
    std::vector<Entity> pointLights;

    for (Entity lightEntity : m_registry->with<Transform, Light>()) {

        auto& light = m_registry->get<Light>(lightEntity);
        if (light.kind == Light::Kind::DIRECTIONAL)
            directionalLights.push_back(lightEntity);
        else
            pointLights.push_back(lightEntity);
    }

    updateDepthMapsDirectionalLights(directionalLights);
    updateDepthMapsPositionalLights(pointLights);

    auto size = m_engine->getWindow().getSize();
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError();
}

void RenderSystem::renderEntities() {

    Actor mainCamera = getMainCamera();
    if (!mainCamera)
        return;

    const glm::mat4 matrixView = glm::inverse(mainCamera.get<Transform>().getWorldTransform());
    const glm::mat4 matrixProjection = getCameraProjectionMatrix(*m_engine, mainCamera.get<Camera>());

    // Draw batches
    for (const auto& [material, mesh] : m_batches) {
        material->render(&mesh, m_engine, &m_depthMaps, glm::mat4(1), matrixView, matrixProjection);
    }

    int numBatched = 0;
    Material* previousMaterial = nullptr;
    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        const auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (!renderInfo.isEnabled || !renderInfo.material || !renderInfo.model)
            continue;

        if (renderInfo.isInBatch) {
            numBatched += renderInfo.model->getMeshes().size();
            continue;
        }

        const glm::mat4& matrixModel = m_registry->get<Transform>(e).getWorldTransform();
        if (renderInfo.material.get() != previousMaterial)
            renderInfo.material->use(m_engine, &m_depthMaps, matrixModel, matrixView, matrixProjection);
        else
            renderInfo.material->updateModelMatrix(matrixModel);

        for (const Mesh& mesh : renderInfo.model->getMeshes())
            renderInfo.material->setAttributesAndDraw(&mesh);

        previousMaterial = renderInfo.material.get();

        checkRenderingError(m_engine->actor(e));
    }

    //std::cout << "Draw calls saved by batching: " << numBatched - m_batches.size() << '\n';
}

void RenderSystem::renderSkybox() {

    Actor mainCamera = getMainCamera();
    if (!mainCamera)
        return;

    Scene* scene = m_engine->getSceneManager().getCurrentScene();
    if (!scene)
        return;

    const auto& renderSettings = scene->getRenderSettings();
    if (!renderSettings.useSkybox)
        return;

    const std::shared_ptr<Texture>& skyboxTexture = renderSettings.skyboxTexture ? renderSettings.skyboxTexture : m_defaultSkybox;
    if (!skyboxTexture || !skyboxTexture->isValid() || skyboxTexture->getKind() != Texture::Kind::TextureCube)
        return;

    const glm::mat4 matrixView = glm::mat4(glm::inverse(mainCamera.get<Transform>().getWorldRotation()));

    const auto& camera = mainCamera.get<Camera>();
    const auto size = m_engine->getWindow().getSize();
    const float aspectRatio = (float) size.x / size.y;
    const glm::mat4 matrixProjection = glm::perspective(
        glm::radians(camera.isOrthographic ? 90.f : camera.fov),
        aspectRatio,
        camera.nearPlaneDistance,
        camera.farPlaneDistance
    );
    m_skyboxRenderer.draw(*skyboxTexture, matrixProjection * matrixView);
}

namespace {

    void updateUIRect(Engine& engine, EntityRegistry& registry, Entity e, const glm::vec2& parentSize, const glm::vec2& parentPivot, float scaleFactor) {

        auto* rect = registry.tryGet<UIRect>(e);
        if (!rect)
            return;

        auto* tf = registry.tryGet<Transform>(e);
        if (!tf)
            return;

        const glm::vec2 parentMinToLocalMin = parentSize * rect->anchorMin + rect->offsetMin * scaleFactor;
        const glm::vec2 parentMinToLocalMax = parentSize * rect->anchorMax + rect->offsetMax * scaleFactor;
        rect->computedSize = parentMinToLocalMax - parentMinToLocalMin;

        const glm::vec2 parentMinToLocalPivot = glm::lerp(parentMinToLocalMin, parentMinToLocalMax, rect->pivot);
        const glm::vec2 parentPivotToParentMin = -parentSize * parentPivot;
        const glm::vec2 parentPivotToLocalPivot = parentPivotToParentMin + parentMinToLocalPivot;
        tf->setLocalPosition(glm::vec3(parentPivotToLocalPivot, tf->getLocalPosition().z));

        for (Entity child : tf->getChildren())
            updateUIRect(engine, registry, child, rect->computedSize, rect->pivot, scaleFactor);
    }
}

void RenderSystem::renderUI() {

    glDisable(GL_DEPTH_TEST);

    for (Entity e : m_registry->with<Transform, UIRect>())
        if (!m_registry->get<Transform>(e).getParent())
            updateUIRect(*m_engine, *m_registry, e, getWindowSize(), {0, 0}, getUIScaleFactor());

    for (Entity e : m_registry->with<Transform, UIRect>())
        if (!m_registry->get<Transform>(e).getParent())
            renderUIRect(e, m_registry->get<UIRect>(e));

    glEnable(GL_DEPTH_TEST);
}

void RenderSystem::renderUIRect(Entity e, UIRect& rect) {

    if (!rect.isEnabled)
        return;

    auto* tf = m_registry->tryGet<Transform>(e);
    if (!tf)
        return;

    const glm::vec2 windowSize = getWindowSize();
    const glm::mat4 matrixProjection = glm::ortho(0.f, windowSize.x, 0.f, windowSize.y);

    auto* sprite = m_registry->tryGet<Sprite>(e);
    if (sprite && sprite->isEnabled && sprite->material) {

        const glm::mat4& transform = tf->getWorldTransform();
        const glm::vec2 localMin = -rect.computedSize * rect.pivot;
        const glm::vec2 localMax =  rect.computedSize * (1.f - rect.pivot);
        const glm::vec3 corners[] = {
            transform * glm::vec4(localMin              , 0, 1),
            transform * glm::vec4(localMin.x, localMax.y, 0, 1),
            transform * glm::vec4(localMax.x, localMin.y, 0, 1),
            transform * glm::vec4(localMax              , 0, 1)
        };
        const std::vector<Vertex> vertices = {
            {corners[1], {0, 1}},
            {corners[0], {0, 0}},
            {corners[2], {1, 0}},

            {corners[1], {0, 1}},
            {corners[2], {1, 0}},
            {corners[3], {1, 1}},
        };

        sprite->material->use(m_engine, &m_depthMaps, glm::mat4(1), glm::mat4(1), matrixProjection);
        m_vertexRenderer.renderVertices(vertices);
    }

    auto* text = m_registry->tryGet<Text>(e);
    if (text && text->getMaterial()) {

        const std::vector<Vertex>& vertices = text->getVertices();

        const glm::vec2& alignment = text->getAlignment();
        const glm::vec2 boundsAlignPoint = glm::lerp(text->getBoundsMin(), text->getBoundsMax(), alignment);
        const glm::vec2 offsetInRect = rect.computedSize * (alignment - rect.pivot);

        // Scale the bounds by the scale factor and bring them to the rect's position.
        glm::mat4 matrix = glm::translate(glm::vec3(-boundsAlignPoint, 0.f));
        matrix = glm::scale(glm::vec3(getUIScaleFactor())) * matrix;
        matrix = glm::translate(glm::vec3(offsetInRect, 0.f)) * matrix;
        matrix = tf->getWorldTransform() * matrix;

        text->getMaterial()->use(m_engine, &m_depthMaps, glm::mat4(1), glm::mat4(1), matrixProjection * matrix);
        m_vertexRenderer.renderVertices(vertices);
    }

    for (Entity child : tf->getChildren())
        if (auto* childRect = m_registry->tryGet<UIRect>(child))
            renderUIRect(child, *childRect);
}

void RenderSystem::renderDebug() {

    m_debugHud->setDebugInfo({m_engine->getFps(), m_engine->getFrameTimeMicroseconds()});
    m_debugHud->draw();
}

Actor RenderSystem::getMainCamera() {

    const Entity entity = m_registry->with<Transform, Camera>().tryGetOne();
    return m_engine->actor(entity);
}

utils::Bounds RenderSystem::getCameraFrustrumBounds() {

    const Actor mainCamera = getMainCamera();
    if (!mainCamera) // Even though this function shouldn't get called at all if there's no camera.
        return {glm::vec3(-100.f), glm::vec3(100.f)};

    const glm::mat4 viewToWorld = mainCamera.get<Transform>().getWorldTransform();
    const glm::mat4 clipToView  = glm::inverse(getCameraProjectionMatrix(*m_engine, mainCamera.get<Camera>(), MAX_SHADOW_DISTANCE));
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

    const utils::Bounds constrainingBounds = getCameraFrustrumBounds();

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

    glViewport(0, 0, m_depthMaps.getDirectionalMapResolution().x, m_depthMaps.getDirectionalMapResolution().y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMaps.getDirectionalMapsFramebufferId());
    glClear(GL_DEPTH_BUFFER_BIT);

    m_directionalDepthShader->use();
    int i = 0;
    for (Entity e : directionalLights) {

        if (i >= m_depthMaps.getMaxNumDirectionalLights())
            break;

        auto& light = m_registry->get<Light>(e);
        auto& tf = m_registry->get<Transform>(e);
        light.matrixPV = getDirectionalLightspaceTransform(light, tf, m_shadowReceiversBounds);
        m_directionalDepthShader->setUniformValue("matrixPV[" + std::to_string(i) + "]", light.matrixPV);

        i += 1;
    }
    m_directionalDepthShader->setUniformValue("numLights", i);

    m_directionalDepthShader->setUniformValue("matrixModel", glm::mat4(1));
    for (const auto& [material, mesh] : m_batches) {
        mesh.render(0, -1, -1);
    }

    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (renderInfo.isInBatch || !renderInfo.isEnabled || !renderInfo.model)
            continue;

        const glm::mat4& modelTransform = m_registry->get<Transform>(e).getWorldTransform();
        m_directionalDepthShader->setUniformValue("matrixModel", modelTransform);
        for (const Mesh& mesh : renderInfo.model->getMeshes())
            mesh.render(0, -1, -1);

        checkRenderingError(m_engine->actor(e));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

namespace {

    std::tuple<float, glm::vec3, std::array<glm::mat4, 6>> getPointLightUniforms(const DepthMaps& depthMaps, const Light& light, const Transform& lightTransform) {

        float nearPlaneDistance = light.nearPlaneDistance;
        float farPlaneDistance  = light.farPlaneDistance;
        glm::mat4 lightProjectionMatrix = glm::perspective(
            glm::radians(90.f),
            (float)depthMaps.getCubemapResolution().x / (float)depthMaps.getCubemapResolution().y,
            nearPlaneDistance,
            farPlaneDistance
        );

        glm::vec3 lightPosition = lightTransform.getWorldPosition();

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

    glViewport(0, 0, m_depthMaps.getCubemapResolution().x, m_depthMaps.getCubemapResolution().y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMaps.getCubemapsFramebufferId());
    glClear(GL_DEPTH_BUFFER_BIT);

    m_positionalDepthShader->use();

    struct Sphere {
        float radius = 0.f;
        glm::vec3 position;
    };

    std::vector<Sphere> lightSpheres;
    int i = 0;
    for (Entity e : pointLights) {

        auto& light = m_registry->get<Light>(e);
        auto& tf = m_registry->get<Transform>(e);
        auto [farPlaneDistance, lightPosition, pvMatrices] = getPointLightUniforms(m_depthMaps, light, tf);
        lightSpheres.push_back({light.range, lightPosition});

        for (unsigned int face = 0; face < 6; ++face)
            m_positionalDepthShader->setUniformValue("matrixPV[" + std::to_string(i * 6 + face) + "]", pvMatrices[face]);
        std::string prefix = "lights[" + std::to_string(i) + "].";
        m_positionalDepthShader->setUniformValue(prefix + "position", lightPosition);
        m_positionalDepthShader->setUniformValue(prefix + "farPlaneDistance", farPlaneDistance);

        if (++i >= m_depthMaps.getMaxNumPositionalLights())
            break;
    }
    m_positionalDepthShader->setUniformValue("numLights", i);

//    m_positionalDepthShader->setUniformValue("matrixModel", glm::mat4(1));
//    for (const auto& [material, mesh] : m_batches) {
//        mesh.render(0);
//    }

    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (/*renderInfo.isInBatch ||*/ !renderInfo.isEnabled || !renderInfo.model)
            continue;

        const glm::mat4& modelTransform = m_registry->get<Transform>(e).getWorldTransform();
        if (std::none_of(lightSpheres.begin(), lightSpheres.end(), [pos = glm::vec3(modelTransform[3])](const Sphere& sphere){
            return glm::distance2(pos, sphere.position) < sphere.radius * sphere.radius;
        }))
            continue;

        m_positionalDepthShader->setUniformValue("matrixModel", modelTransform);
        for (const Mesh& mesh : renderInfo.model->getMeshes())
            mesh.render(0);
        glCheckError();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
}

glm::vec2 RenderSystem::getWindowSize() {

    sf::Vector2u windowSizeSf = m_engine->getWindow().getSize();
    return glm::vec2(windowSizeSf.x, windowSizeSf.y);
}

float RenderSystem::getUIScaleFactor() {

    const glm::vec2 windowSize = getWindowSize();
    return std::sqrt((windowSize.x / m_referenceResolution.x) * (windowSize.y / m_referenceResolution.y));
}

void RenderSystem::receive(const SceneManager::OnSceneClosed& info) {

    m_batches.clear();
}
