//
// Created by Maksym Maisak on 2019-02-10.
//

#ifndef ENGINE_DEPTHMAPS_H
#define ENGINE_DEPTHMAPS_H

#include <GL/glew.h>
#include "FramebufferObject.h"
#include "glm.h"
#include "TextureObject.h"

namespace en {

    /// Owns depth map textures and framebuffers used for shadowmapping.
    class DepthMaps final {

    public:
        using Resolution = glm::vec<2, GLsizei>;

        explicit DepthMaps(
            GLsizei maxNumDirectionalLights = 4, Resolution directionalMapResolution = {1024, 1024},
            GLsizei maxNumPositionalLights = 10, Resolution cubemapResolution = {512, 512}
        );

        Resolution getDirectionalMapResolution() const;
        GLsizei getMaxNumDirectionalLights() const;
        gl::FramebufferObject& getDirectionalMapsFramebuffer();
        gl::TextureObject& getDirectionalMapsTexture();

        Resolution getCubemapResolution() const;
        GLsizei getMaxNumPositionalLights() const;
        gl::FramebufferObject& getCubemapsFramebuffer();
        gl::TextureObject& getCubemapsTexture();

    private:

        Resolution m_cubemapResolution;
        GLsizei m_maxNumPositionalLights;
        gl::FramebufferObject m_cubemapsFBO;
        gl::TextureObject m_cubemapsTexture;

        Resolution m_directionalMapResoltuion;
        GLsizei m_maxNumDirectionalLights;
        gl::FramebufferObject m_directionalMapsFBO;
        gl::TextureObject m_directionalMapsTexture;
    };
}

#endif //ENGINE_DEPTHMAPS_H
