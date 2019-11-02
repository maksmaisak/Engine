//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingUtilities.h"
#include "ScopedBind.h"
#include "ShaderProgram.h"
#include "Resources.h"
#include "Config.h"

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
