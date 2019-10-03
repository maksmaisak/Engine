//
// Created by Maksym Maisak on 2019-02-10.
//

#include "DepthMaps.h"
#include <iostream>
#include <tuple>
#include "GLHelpers.h"

using namespace en;

std::tuple<GLuint, GLuint> makeDepthCubemaps(const glm::vec<2, GLsizei> resolution, GLsizei maxNumLights) {

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texture);
    {
        glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, resolution.x, resolution.y, 6 * maxNumLights, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
    glCheckError();

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Failed to create the framebuffer for depth cubemaps" << std::endl;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();

    return {texture, fbo};
}

std::tuple<GLuint, GLuint> makeDirectionalDepthMaps(const glm::vec<2, GLsizei> resolution, GLsizei maxNumLights) {

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    {
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, resolution.x, resolution.y, maxNumLights, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1, 1, 1, 1};
        glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glCheckError();

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Failed to create the framebuffer for directional depth maps" << std::endl;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();

    return {texture, fbo};
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
    std::tie(m_cubemapsTexture, m_cubemapsFBO) = makeDepthCubemaps(m_cubemapResolution, m_maxNumPositionalLights);
    std::tie(m_directionalMapsTexture, m_directionalMapsFBO) = makeDirectionalDepthMaps(m_directionalMapResoltuion, m_maxNumDirectionalLights);
}

DepthMaps::~DepthMaps() {

    glDeleteTextures(1, &m_cubemapsTexture);
    glDeleteFramebuffers(1, &m_cubemapsFBO);

    glDeleteTextures(1, &m_directionalMapsTexture);
    glDeleteFramebuffers(1, &m_directionalMapsFBO);
}

DepthMaps::Resolution DepthMaps::getCubemapResolution() const {return m_cubemapResolution;}
GLsizei DepthMaps::getMaxNumPositionalLights() const {return m_maxNumPositionalLights;}
GLuint DepthMaps::getCubemapsFramebufferId() const {return m_cubemapsFBO;}
GLuint DepthMaps::getCubemapsTextureId() const {return m_cubemapsTexture;}

DepthMaps::Resolution DepthMaps::getDirectionalMapResolution() const {return m_directionalMapResoltuion;}
GLsizei DepthMaps::getMaxNumDirectionalLights() const {return m_maxNumDirectionalLights;}
GLuint DepthMaps::getDirectionalMapsFramebufferId() const {return m_directionalMapsFBO;}
GLuint DepthMaps::getDirectionalMapsTextureId() const {return m_directionalMapsTexture;}
