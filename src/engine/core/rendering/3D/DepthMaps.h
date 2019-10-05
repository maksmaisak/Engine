//
// Created by Maksym Maisak on 2019-02-10.
//

#ifndef ENGINE_DEPTHMAPS_H
#define ENGINE_DEPTHMAPS_H

#include <GL/glew.h>
#include "glm.h"
#include "GLWrapper.h"
#include "FramebufferObject.h"

namespace en {

    /// Owns depth maps from light sources, used for shadowmapping.
    class DepthMaps final {

    public:
        using Resolution = glm::vec<2, GLsizei>;

        explicit DepthMaps(
            GLsizei maxNumDirectionalLights = 4,
            Resolution directionalMapResolution = {1024, 1024},
            GLsizei maxNumPositionalLights = 10,
            Resolution cubemapResolution = {512, 512}
        );
        ~DepthMaps();
        DepthMaps(const DepthMaps& other) = delete;
        DepthMaps& operator=(const DepthMaps& other) = delete;
        DepthMaps(DepthMaps&& other) = delete;
        DepthMaps& operator=(DepthMaps&& other) = delete;

        Resolution getCubemapResolution() const;
        GLsizei getMaxNumPositionalLights() const;
        GLuint getCubemapsFramebufferId() const;
        GLuint getCubemapsTextureId() const;

        Resolution getDirectionalMapResolution() const;
        GLsizei getMaxNumDirectionalLights() const;
        GLuint getDirectionalMapsFramebufferId() const;
        GLuint getDirectionalMapsTextureId() const;

    private:

        Resolution m_cubemapResolution;
        GLsizei m_maxNumPositionalLights;
        GLuint m_cubemapsFBO     = 0;
        GLuint m_cubemapsTexture = 0;

        Resolution m_directionalMapResoltuion;
        GLsizei m_maxNumDirectionalLights;
        GLuint m_directionalMapsFBO     = 0;
        GLuint m_directionalMapsTexture = 0;
    };
}

#endif //ENGINE_DEPTHMAPS_H
