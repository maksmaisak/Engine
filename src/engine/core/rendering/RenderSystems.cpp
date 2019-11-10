//
// Created by Maksym Maisak on 22/10/18.
//

#include "RenderSystems.h"

#include <iostream>
#include <GL/glew.h>
#include <imgui.h>

#include "Render3DSystem.h"
#include "Render2DSystem.h"
#include "RenderSkyboxSystem.h"
#include "RenderUISystem.h"
#include "RenderImguiSystem.h"
#include "PostProcessingSystem.h"
#include "RenderingSharedState.h"
#include "Resources.h"
#include "ScopedBind.h"

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

        // Disable the byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }

    void renderGLTest() {

        static auto shader = Resources<ShaderProgram>::get("sprite");
        static GLint matrixLocation = shader->getUniformLocation("matrixProjection");
        static GLint spriteTextureLocation = shader->getUniformLocation("spriteTexture");
        static GLint spriteColorLocation = shader->getUniformLocation("spriteColor");

        shader->use();
        gl::setUniform(matrixLocation, glm::ortho(-2.f, 2.f, -2.f, 2.f));
        gl::setUniform(spriteTextureLocation, Textures::white().get(), 0);
        gl::setUniform(spriteColorLocation, glm::vec4(0, 1, 0, 1));

        Mesh::getQuad()->render(0, -1, 1);
    }
}

RenderSystems::RenderSystems() :
    m_renderingSharedState(std::make_shared<RenderingSharedState>())
{}

void RenderSystems::start() {

    setOpenGLSettings();
    m_renderingSharedState->loadConfigFromLua(m_engine->getLuaState());

    addSystem<Render3DSystem>(m_renderingSharedState);
    addSystem<RenderSkyboxSystem>();
    addSystem<Render2DSystem>();
    addSystem<RenderUISystem>(m_renderingSharedState);
    addSystem<RenderImguiSystem>();
    addSystem<PostProcessingSystem>(m_renderingSharedState);

    CompoundSystem::start();
}

void RenderSystems::draw() {

    if (glCheckError() != GL_NO_ERROR) {
        std::cerr << "Uncaught openGL error(s) before rendering." << std::endl;
    }

    const gl::FramebufferObject& prePostProcessingFbo = m_renderingSharedState->prePostProcessingFramebuffer.framebuffer;
    const bool usePostProcessing = prePostProcessingFbo.isValid();
    if (usePostProcessing) {

        const gl::ScopedBind bind(prePostProcessingFbo, GL_FRAMEBUFFER);
        renderToCurrentFramebuffer();

    } else {

        // Convert output from fragment shaders from linear to sRGB
        glEnable(GL_FRAMEBUFFER_SRGB);
        renderToCurrentFramebuffer();
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
}

void RenderSystems::receive(const SceneManager::OnSceneClosed& event) {

    if (m_renderingSharedState) {
        m_renderingSharedState->batches.clear();
    }
}

void RenderSystems::receive(const Window::FramebufferSizeEvent& event) {

    // Make viewport match window size.
    glViewport(0, 0, event.width, event.height);
}

void RenderSystems::renderToCurrentFramebuffer() {

    m_engine->getWindow().setViewport();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //renderGLTest();

    CompoundSystem::draw();

    if (m_renderingSharedState->enableDebugOutput) {
        renderDebug();
    }
}

void RenderSystems::renderDebug() {

    ImGui::Text("fps: %i", glm::iround(m_engine->getFps()));
    ImGui::Text("render time: %f ms", m_engine->getFrameTimeMicroseconds() / 1000.0);
}