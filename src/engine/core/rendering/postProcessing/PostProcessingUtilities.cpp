//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingUtilities.h"
#include "ScopedBind.h"
#include "ShaderProgram.h"
#include "Resources.h"
#include "Config.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

using namespace en;

gl::FramebufferBundle PostProcessingUtilities::makeFramebuffer(const glm::u32vec2& size) {

    gl::FramebufferObject fbo(gl::ForceCreate);

    const gl::ScopedBind bindFbo(fbo, GL_FRAMEBUFFER);

    gl::TextureObject texture(gl::ForceCreate);
    {
        const gl::ScopedBind bindTexture(texture, GL_TEXTURE_2D);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size.x, size.y, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    }

    gl::RenderbufferObject rbo(gl::ForceCreate);
    {
        const gl::ScopedBind bindRbo(rbo, GL_RENDERBUFFER);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE && "Framebuffer now complete!");

    return {
        std::move(fbo),
        std::move(texture),
        std::move(rbo)
    };
}

std::shared_ptr<ShaderProgram> PostProcessingUtilities::getPostProcessingShader(const std::string& name) {

    /// Use the blit vertex shader so that you only need to implement a fragment shader when creating new post processing effects.
    return Resources<ShaderProgram>::get(name, config::SHADER_PATH + "blit.vs", config::SHADER_PATH + name + ".fs");
}

void PostProcessingUtilities::renderQuad() {

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

void PostProcessingUtilities::blit(const gl::TextureObject& sourceTexture, const gl::FramebufferObject& target, bool clearTarget) {

    static const std::shared_ptr<ShaderProgram> shader = getPostProcessingShader("blit");
    static const GLint sourceTextureLocation = shader->getUniformLocation("sourceTexture");

    shader->use();
    gl::setUniform(sourceTextureLocation, sourceTexture, 0);

    const gl::ScopedBind bindFBO(target, GL_FRAMEBUFFER);
    if (clearTarget) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    renderQuad();
}