//
// Created by Maksym Maisak on 2019-02-10.
//

#include "DepthMaps.h"
#include <iostream>
#include <tuple>
#include "GLHelpers.h"

using namespace en;

namespace {

    gl::FramebufferObject makeFramebuffer(gl::TextureObject& texture) {

        assert(texture);

        gl::FramebufferObject fbo;
        fbo.create();
        fbo.bind(GL_FRAMEBUFFER);
        {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                std::cerr << "Failed to create framebuffer" << std::endl;
            }
        }
        fbo.unbind(GL_FRAMEBUFFER);

        return fbo;
    }

    std::pair<gl::TextureObject, gl::FramebufferObject> makeDirectionalDepthMaps(const glm::vec<2, GLsizei> resolution, GLsizei maxNumLights) {

        gl::TextureObject texture;
        texture.create();
        texture.bind(GL_TEXTURE_2D_ARRAY);
        {
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, resolution.x, resolution.y, maxNumLights, 0,
                         GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            const float borderColor[] = {1, 1, 1, 1};
            glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
        texture.unbind(GL_TEXTURE_2D_ARRAY);

        gl::FramebufferObject fbo = makeFramebuffer(texture);

        return {std::move(texture), std::move(fbo)};
    }

    std::pair<gl::TextureObject, gl::FramebufferObject> makeDepthCubemaps(const glm::vec<2, GLsizei> resolution, GLsizei maxNumLights) {

        gl::TextureObject texture;
        texture.create();
        texture.bind(GL_TEXTURE_CUBE_MAP_ARRAY);
        {
            glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, resolution.x, resolution.y, 6 * maxNumLights,
                         0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
        texture.unbind(GL_TEXTURE_CUBE_MAP_ARRAY);

        gl::FramebufferObject fbo = makeFramebuffer(texture);

        return {std::move(texture), std::move(fbo)};
    }
}

DepthMaps::DepthMaps(
    GLsizei maxNumDirectionalLights, Resolution directionalMapResolution,
    GLsizei maxNumPositionalLights , Resolution cubemapResolution
) :
    m_maxNumDirectionalLights(maxNumDirectionalLights),
    m_directionalMapResoltuion(directionalMapResolution),
    m_maxNumPositionalLights(maxNumPositionalLights),
    m_cubemapResolution(cubemapResolution)
{
    std::tie(m_directionalMapsTexture, m_directionalMapsFBO) = makeDirectionalDepthMaps(m_directionalMapResoltuion, m_maxNumDirectionalLights);
    std::tie(m_cubemapsTexture, m_cubemapsFBO) = makeDepthCubemaps(m_cubemapResolution, m_maxNumPositionalLights);
}

DepthMaps::Resolution DepthMaps::getDirectionalMapResolution() const {return m_directionalMapResoltuion;}
GLsizei DepthMaps::getMaxNumDirectionalLights() const {return m_maxNumDirectionalLights;}
gl::FramebufferObject& DepthMaps::getDirectionalMapsFramebuffer() {return m_directionalMapsFBO;}
gl::TextureObject& DepthMaps::getDirectionalMapsTexture() {return m_directionalMapsTexture;}

DepthMaps::Resolution DepthMaps::getCubemapResolution() const {return m_cubemapResolution;}
GLsizei DepthMaps::getMaxNumPositionalLights() const {return m_maxNumPositionalLights;}
gl::FramebufferObject& DepthMaps::getCubemapsFramebuffer() {return m_cubemapsFBO;}
gl::TextureObject& DepthMaps::getCubemapsTexture() {return m_cubemapsTexture;}
