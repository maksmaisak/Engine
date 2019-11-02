//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingPassBloom.h"
#include <imgui.h>
#include "PostProcessingUtilities.h"
#include "Engine.h"
#include "ScopedBind.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Resources.h"
#include "ShaderProgram.h"
#include "GLHelpers.h"

using namespace en;

PostProcessingPassBloom::PostProcessingPassBloom() {

    const auto [width, height] = Engine::get().getWindow().getSize();
    m_intermediateFramebuffer = PostProcessingUtilities::makeFramebuffer({width, height});
}

void PostProcessingPassBloom::render(const gl::TextureObject& sourceTexture) {

    const std::shared_ptr<ShaderProgram> blit = PostProcessingUtilities::getPostProcessingShader("blit");
    const std::shared_ptr<ShaderProgram> isolateBrightFragments = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/isolateBrightFragments");
    const std::shared_ptr<ShaderProgram> blurHorizontal = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/blurHorizontal");
    const std::shared_ptr<ShaderProgram> blurVertical = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/blurVertical");
    const std::shared_ptr<ShaderProgram> bloomCombine = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/bloomCombine");

    glDisable(GL_FRAMEBUFFER_SRGB);

    {
        const gl::ScopedBind bindFBO(m_intermediateFramebuffer.framebuffer, GL_FRAMEBUFFER);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        isolateBrightFragments->use();
        gl::setUniform(isolateBrightFragments->getUniformLocation("sourceTexture"), sourceTexture, 0);
        renderQuad();

        blurHorizontal->use();
        gl::setUniform(blurHorizontal->getUniformLocation("sourceTexture"), m_intermediateFramebuffer.colorTexture, 0);
        renderQuad();

        blurVertical->use();
        gl::setUniform(blurVertical->getUniformLocation("sourceTexture"), m_intermediateFramebuffer.colorTexture, 0);
        renderQuad();
    }

    static float intensity = 1.f;
    ImGui::SliderFloat("Bloom intensity", &intensity, 0.f, 5.f);

    bloomCombine->use();
    bloomCombine->setUniformValue("intensity", intensity);
    gl::setUniform(bloomCombine->getUniformLocation("sourceTexture"), sourceTexture, 0);
    gl::setUniform(bloomCombine->getUniformLocation("blurredTexture"), m_intermediateFramebuffer.colorTexture, 1);

    glEnable(GL_FRAMEBUFFER_SRGB);
    renderQuad();
    glDisable(GL_FRAMEBUFFER_SRGB);
}

void PostProcessingPassBloom::renderQuad() {

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
