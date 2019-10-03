//
// Created by Maksym Maisak on 22/10/18.
//

#include "RenderSystem.h"
#include <iostream>
#include <GL/glew.h>
#include "Camera.h"
#include "GLHelpers.h"
#include "Transform.h"
#include "Material.h"
#include "Name.h"
#include "RenderInfo.h"
#include "Render2DSystem.h"
#include "RenderSkyboxSystem.h"
#include "RenderUISystem.h"
#include "Resources.h"

using namespace en;

namespace {

    inline void checkRenderingError(const Actor& actor) {

        if (glCheckError() == GL_NO_ERROR) {
            return;
        }

        const auto* const namePtr = actor.tryGet<en::Name>();
        const std::string name = namePtr ? namePtr->value : "unnamed";
        std::cerr << "Error while rendering " << name << std::endl;
    }
}

RenderSystem::RenderSystem() :
    m_enableStaticBatching(true),
    m_enableDebugOutput(false)
{}

void RenderSystem::start() {

    getConfigFromLua();

    setOpenGLSettings();

    m_shadowMapper = std::make_unique<ShadowMapper>(*m_engine, m_renderingSharedState);
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

    m_shadowMapper->updateDepthMaps();
    renderEntities();
    CompoundSystem::draw();

    if (m_enableDebugOutput) {
        renderDebug();
    }
}

void RenderSystem::receive(const SceneManager::OnSceneClosed& info) {

    m_renderingSharedState.batches.clear();
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

    // Disable the byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void RenderSystem::getConfigFromLua() {

    auto& lua = m_engine->getLuaState();

    lua_getglobal(lua, "Config");
    auto popConfig = lua::PopperOnDestruct(lua);

    m_renderingSharedState.referenceResolution = lua.tryGetField<glm::vec2>("referenceResolution").value_or(m_renderingSharedState.referenceResolution);
    m_enableStaticBatching = lua.tryGetField<bool>("enableStaticBatching").value_or(m_enableStaticBatching);
    m_enableDebugOutput = lua.tryGetField<bool>("enableDebugOutput").value_or(m_enableDebugOutput);
}

void RenderSystem::updateBatches() {

    const auto findBatchMesh = [&batches = m_renderingSharedState.batches](const std::shared_ptr<Material>& material) -> en::Mesh& {

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

    for (auto& [material, batch] : m_renderingSharedState.batches) {
        batch.updateBuffers();
    }
}

void RenderSystem::renderEntities() {

    Actor mainCamera = m_engine->getMainCamera();
    if (!mainCamera) {
        return;
    }

    const glm::mat4 matrixView = glm::inverse(mainCamera.get<Transform>().getWorldTransform());
    const glm::mat4 matrixProjection = mainCamera.get<Camera>().getCameraProjectionMatrix(*m_engine);

    // Render batches
    for (const auto& [material, mesh] : m_renderingSharedState.batches) {
        material->render(&mesh, m_engine, &m_renderingSharedState.depthMaps, glm::mat4(1), matrixView, matrixProjection);
    }

    // Render entities
    int numSavedByBatching = 0;
    const Material* previousMaterial = nullptr;
    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        const auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (!renderInfo.isValidForRendering()) {
            continue;
        }

        if (renderInfo.isInBatch) {
            numSavedByBatching += renderInfo.model->getMeshes().size();
            continue;
        }

        const glm::mat4& matrixModel = m_registry->get<Transform>(e).getWorldTransform();
        if (previousMaterial == renderInfo.material.get()) {
            renderInfo.material->updateModelMatrix(matrixModel);
        } else {
            renderInfo.material->use(m_engine, &m_renderingSharedState.depthMaps, matrixModel, matrixView, matrixProjection);
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
