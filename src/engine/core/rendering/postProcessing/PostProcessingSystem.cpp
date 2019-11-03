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

    const auto [width, height] = m_engine->getWindow().getSize();
    setUp({width, height});
}

void PostProcessingSystem::draw() {

    assert(std::all_of(
        m_postProcessingPasses.begin(),
        m_postProcessingPasses.end(),
        [](const std::unique_ptr<en::PostProcessingPass>& pass) -> bool {return pass.get();}
    ));

    static const gl::FramebufferObject screenFramebuffer = {};

    for (std::size_t i = 0; i < m_postProcessingPasses.size(); ++i) {

        const bool isFirst = i == 0;
        const bool isLast = i == m_postProcessingPasses.size() - 1;
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

            m_postProcessingPasses[i]->render(source);
        }

        if (isLast) {
            glDisable(GL_FRAMEBUFFER_SRGB);
        }
    }
}

void PostProcessingSystem::receive(const sf::Event& event) {

    if (event.type == sf::Event::Resized) {
        setUp({event.size.width, event.size.height});
    }
}

void PostProcessingSystem::setUp(const glm::u32vec2& size) {

    m_renderingSharedState->prePostProcessingFramebuffer = PostProcessingUtilities::makeFramebuffer(size);

    for (gl::FramebufferBundle& fb : m_intermediateBuffers) {
        fb = PostProcessingUtilities::makeFramebuffer(size);
    }

    for (const std::unique_ptr<PostProcessingPass>& pass : m_postProcessingPasses) {
        assert(pass);
        pass->setUp(size);
    }
}
