//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingSystem.h"
#include "RenderingSharedState.h"
#include "PostProcessingUtilities.h"
#include "ScopedBind.h"
#include "Engine.h"
#include "ShaderProgram.h"
#include <cassert>

using namespace en;

PostProcessingSystem::PostProcessingSystem(std::shared_ptr<struct RenderingSharedState> renderingSharedState) :
    m_renderingSharedState(std::move(renderingSharedState))
{
    assert(m_renderingSharedState);
}

void PostProcessingSystem::start() {

    const auto [width, height] = m_engine->getWindow().getSize();
    m_renderingSharedState->prePostProcessingFramebuffer = PostProcessingUtilities::makeFramebuffer({width, height});
}

void PostProcessingSystem::draw() {

    const gl::ScopedBind bindZeroFBO(gl::FramebufferObject{}, GL_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_postProcessPassBloom.render(m_renderingSharedState->prePostProcessingFramebuffer.colorTexture);
}