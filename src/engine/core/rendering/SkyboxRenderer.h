//
// Created by Maksym Maisak on 2019-03-13.
//

#ifndef SAXION_Y2Q2_RENDERING_SKYBOXRENDERER_H
#define SAXION_Y2Q2_RENDERING_SKYBOXRENDERER_H

#include <memory>
#include <GL/glew.h>
#include "glm.hpp"
#include "Texture.hpp"
#include "ShaderProgram.hpp"

namespace en {

    class SkyboxRenderer final {

    public:
        SkyboxRenderer();
        ~SkyboxRenderer();
        SkyboxRenderer(const SkyboxRenderer&) = delete;
        SkyboxRenderer& operator=(const SkyboxRenderer&) = delete;
        SkyboxRenderer(SkyboxRenderer&&) = delete;
        SkyboxRenderer& operator=(SkyboxRenderer&&) = delete;
        void draw(const Texture& cubemap, const glm::mat4& matrixPV);

    private:
        GLuint m_vao = 0;
        GLuint m_bufferId = 0;
        std::shared_ptr<ShaderProgram> m_shader;
    };
}

#endif //SAXION_Y2Q2_RENDERING_SKYBOXRENDERER_H
