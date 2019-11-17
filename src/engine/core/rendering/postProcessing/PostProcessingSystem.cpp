//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingSystem.h"
#include <cassert>
#include "RenderingSharedState.h"
#include "PostProcessingUtilities.h"
#include "PostProcessingPassBloom.h"
#include "PostProcessingPassTonemapping.h"
#include "ScopedBind.h"
#include "Engine.h"
#include "ShaderProgram.h"

using namespace en;

PostProcessingSystem::PostProcessingSystem(std::shared_ptr<RenderingSharedState> renderingSharedState) :
    m_renderingSharedState(std::move(renderingSharedState))
{
    assert(m_renderingSharedState);
    m_postProcessingPasses.push_back(std::make_unique<PostProcessingPassBloom>());
    m_postProcessingPasses.push_back(std::make_unique<PostProcessingPassTonemapping>());
}

void PostProcessingSystem::start() {

    setUp(m_engine->getWindow().getFramebufferSize());
}

void PostProcessingSystem::draw() {

    std::vector<en::PostProcessingPass*> enabledPasses;
    for (const std::unique_ptr<en::PostProcessingPass>& pass : m_postProcessingPasses) {

        assert(pass);
        pass->displayImGui();
        if (pass->getIsEnabled()) {
            enabledPasses.push_back(pass.get());
        }
    }

    static const gl::FramebufferObject screenFramebuffer = {};

    for (std::size_t i = 0; i < enabledPasses.size(); ++i) {

        const bool isFirst = i == 0;
        const bool isLast = i == enabledPasses.size() - 1;
        const bool drawToSecondBuffer = i % 2 == 1;

        const gl::TextureObject& source = isFirst ?
            m_renderingSharedState->prePostProcessingFramebuffer.colorTexture :
            m_intermediateBuffers[drawToSecondBuffer ? 0 : 1].colorTexture;

        const gl::FramebufferObject& target = isLast ?
            screenFramebuffer :
            m_intermediateBuffers[drawToSecondBuffer ? 1 : 0].framebuffer;

        if (isLast) {
            glEnable(GL_FRAMEBUFFER_SRGB);
        }

        {
            const gl::ScopedBind bindFramebuffer(target, GL_FRAMEBUFFER);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            enabledPasses[i]->render(source);
        }

        if (isLast) {
            glDisable(GL_FRAMEBUFFER_SRGB);
        }
    }

    if (enabledPasses.empty()) {

        glEnable(GL_FRAMEBUFFER_SRGB);
        gl::blit(m_renderingSharedState->prePostProcessingFramebuffer.colorTexture, screenFramebuffer);
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
}

void PostProcessingSystem::receive(const Window::FramebufferSizeEvent& event) {
    setUp({event.width, event.height});
}

void PostProcessingSystem::setUp(const glm::u32vec2& size) {

    m_renderingSharedState->prePostProcessingFramebuffer = gl::makeFramebuffer(size, true);

    for (gl::FramebufferBundle& fb : m_intermediateBuffers) {
        fb = gl::makeFramebuffer(size);
    }

    for (const std::unique_ptr<PostProcessingPass>& pass : m_postProcessingPasses) {
        assert(pass);
        pass->setUp(size);
    }
}
