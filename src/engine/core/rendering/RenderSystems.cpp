//
// Created by Maksym Maisak on 22/10/18.
//

#include "RenderSystems.h"

#include <iostream>
#include <GL/glew.h>

#include "Render3DSystem.h"
#include "Render2DSystem.h"
#include "RenderSkyboxSystem.h"
#include "RenderUISystem.h"
#include "RenderingSharedState.h"
#include "Resources.h"

using namespace en;

namespace {

    void setOpenGLSettings() {

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

        // Enable antialiasing on lines
        glEnable(GL_LINE_SMOOTH);

        // Convert output from fragment shaders from linear to sRGB
        glEnable(GL_FRAMEBUFFER_SRGB);

        // Disable the byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }
}

RenderSystems::RenderSystems() :
    m_renderingSharedState(std::make_shared<RenderingSharedState>())
{}

void RenderSystems::start() {

    setOpenGLSettings();
    m_renderingSharedState->loadConfigFromLua(m_engine->getLuaState());
    m_debugHud = std::make_unique<DebugHud>(*m_engine, m_renderingSharedState->vertexRenderer);

    addSystem<Render3DSystem>(m_renderingSharedState);
    addSystem<RenderSkyboxSystem>();
    addSystem<Render2DSystem>();
    addSystem<RenderUISystem>(m_renderingSharedState);
    CompoundSystem::start();
}

void RenderSystems::draw() {

    if (glCheckError() != GL_NO_ERROR) {
        std::cerr << "Uncaught openGL error(s) before rendering." << std::endl;
    }

    CompoundSystem::draw();

    if (m_renderingSharedState->enableDebugOutput) {
        renderDebug();
    }
}

void RenderSystems::receive(const SceneManager::OnSceneClosed& info) {

    if (m_renderingSharedState) {
        m_renderingSharedState->batches.clear();
    }
}

void RenderSystems::receive(const sf::Event& event) {

    if (event.type == sf::Event::EventType::Resized) {
        // Make viewport match window size.
        glViewport(0, 0, event.size.width, event.size.height);
    }
}

void RenderSystems::renderDebug() {

    m_debugHud->setDebugInfo({m_engine->getFps(), m_engine->getFrameTimeMicroseconds()});
    m_debugHud->draw();
}
