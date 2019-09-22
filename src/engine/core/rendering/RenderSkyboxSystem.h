//
// Created by Maksym Maisak on 2019-03-13.
//

#ifndef ENGINE_RENDERSKYBOXSYSTEM_H
#define ENGINE_RENDERSKYBOXSYSTEM_H

#include <memory>
#include <GL/glew.h>
#include "glm.h"
#include "ShaderProgram.hpp"
#include "System.h"

namespace en {

    class RenderSkyboxSystem : public System {

    public:
        RenderSkyboxSystem();
        ~RenderSkyboxSystem() override;
        RenderSkyboxSystem(const RenderSkyboxSystem&) = delete;
        RenderSkyboxSystem& operator=(const RenderSkyboxSystem&) = delete;
        RenderSkyboxSystem(RenderSkyboxSystem&&) = delete;
        RenderSkyboxSystem& operator=(RenderSkyboxSystem&&) = delete;

        void start() override;
        void draw() override;

    private:

        void renderSkyboxCubemap(const class Texture& cubemap, const glm::mat4& matrixPV);

        GLuint m_vao = 0;
        GLuint m_bufferId = 0;
        std::shared_ptr<ShaderProgram> m_shader;

        std::shared_ptr<Texture> m_defaultSkybox;
    };
}

#endif //ENGINE_RENDERSKYBOXSYSTEM_H
