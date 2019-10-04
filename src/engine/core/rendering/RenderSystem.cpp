//
// Created by Maksym Maisak on 22/10/18.
//

#include "RenderSystem.h"
#include <iostream>
#include <GL/glew.h>
#include "GLHelpers.h"
#include "Transform.h"
#include "Name.h"
#include "RenderInfo.h"
#include "Render3DSystem.h"
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

void RenderSystem::start() {

    getConfigFromLua();
    setOpenGLSettings();
    m_debugHud = std::make_unique<DebugHud>(*m_engine, m_renderingSharedState.vertexRenderer);

    addSystem<Render3DSystem>(m_renderingSharedState);
    addSystem<RenderSkyboxSystem>();
    addSystem<Render2DSystem>();
    addSystem<RenderUISystem>(m_renderingSharedState);
    CompoundSystem::start();
}

void RenderSystem::draw() {

    if (glCheckError() != GL_NO_ERROR) {
        std::cerr << "Uncaught openGL error(s) before rendering." << std::endl;
    }

    CompoundSystem::draw();

    if (m_renderingSharedState.enableDebugOutput) {
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

void RenderSystem::getConfigFromLua() {

    auto& lua = m_engine->getLuaState();

    lua_getglobal(lua, "Config");
    const auto popConfig = lua::PopperOnDestruct(lua);

    m_renderingSharedState.referenceResolution = lua.tryGetField<glm::vec2>("referenceResolution").value_or(m_renderingSharedState.referenceResolution);
    m_renderingSharedState.enableStaticBatching = lua.tryGetField<bool>("enableStaticBatching").value_or(m_renderingSharedState.enableStaticBatching);
    m_renderingSharedState.enableDebugOutput = lua.tryGetField<bool>("enableDebugOutput").value_or(m_renderingSharedState.enableDebugOutput);
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

void RenderSystem::renderDebug() {

    m_debugHud->setDebugInfo({m_engine->getFps(), m_engine->getFrameTimeMicroseconds()});
    m_debugHud->draw();
}
