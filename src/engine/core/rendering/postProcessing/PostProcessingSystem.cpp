//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingSystem.h"
#include "RenderingSharedState.h"
#include "PostProcessingUtilities.h"
#include "PostProcessingPassBloom.h"
#include "ScopedBind.h"
#include "Engine.h"
#include "ShaderProgram.h"
#include <cassert>

using namespace en;

PostProcessingSystem::PostProcessingSystem(std::shared_ptr<RenderingSharedState> renderingSharedState) :
    m_renderingSharedState(std::move(renderingSharedState))
{
    assert(m_renderingSharedState);
    m_postProcessingPasses.push_back(std::make_unique<PostProcessingPassBloom>());
}

void PostProcessingSystem::start() {

    const auto [width, height] = m_engine->getWindow().getSize();
    setUp({width, height});
}

void PostProcessingSystem::draw() {

    const gl::ScopedBind bindZeroFBO(gl::FramebufferObject{}, GL_FRAMEBUFFER);
    for (const std::unique_ptr<PostProcessingPass>& pass : m_postProcessingPasses) {
        assert(pass);
        pass->render(m_renderingSharedState->prePostProcessingFramebuffer.colorTexture);
    }
}

void PostProcessingSystem::receive(const sf::Event& event) {

    if (event.type == sf::Event::Resized) {
        setUp({event.size.width, event.size.height});
    }
}

void PostProcessingSystem::setUp(const glm::u32vec2& size) {

    m_renderingSharedState->prePostProcessingFramebuffer = PostProcessingUtilities::makeFramebuffer(size);

    for (const std::unique_ptr<PostProcessingPass>& pass : m_postProcessingPasses) {
        assert(pass);
        pass->setUp(size);
    }
}
