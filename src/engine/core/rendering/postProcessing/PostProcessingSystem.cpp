//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingSystem.h"
#include "RenderingSharedState.h"
#include "ScopedBind.h"
#include "Engine.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include <cassert>

using namespace en;

namespace {

    std::pair<gl::FramebufferObject, gl::TextureObject> makePrePostProcessingFramebuffer(Engine& engine) {

        gl::TextureObject texture(gl::ForceCreate);
        {
            const gl::ScopedBind bindTexture(texture, GL_TEXTURE_2D);

            const auto [width, height] = engine.getWindow().getSize();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        gl::FramebufferObject fbo(gl::ForceCreate);
        const gl::ScopedBind bindFbo(fbo, GL_FRAMEBUFFER);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        return {std::move(fbo), std::move(texture)};
    }

    void renderQuad() {

        static gl::VertexArrayObject vao;
        static gl::VertexBufferObject vbo;

        if (!vao) {

            vao.create();
            const gl::ScopedBind bindVAO(vao);

            vbo.create();
            const gl::ScopedBind bindVBO(vbo, GL_ARRAY_BUFFER);

            constexpr float quadVertices[] {
                // positions       // uvs
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }

        const gl::ScopedBind bindVAO(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

PostProcessingSystem::PostProcessingSystem(std::shared_ptr<struct RenderingSharedState> renderingSharedState) :
    m_renderingSharedState(std::move(renderingSharedState))
{
    assert(m_renderingSharedState);
}

void PostProcessingSystem::start() {

    std::tie(
        m_renderingSharedState->prePostProcessingFramebuffer,
        m_prePostProcessRenderTexture
    ) = makePrePostProcessingFramebuffer(*m_engine);
}

void PostProcessingSystem::draw() {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Resources<ShaderProgram>::get("blit")->use();
    gl::setUniform(0, m_prePostProcessRenderTexture);
    renderQuad();
}