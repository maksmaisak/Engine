//
// Created by Maksym Maisak on 2019-03-13.
//

#ifndef ENGINE_RENDERSKYBOXSYSTEM_H
#define ENGINE_RENDERSKYBOXSYSTEM_H

#include <memory>
#include <GL/glew.h>
#include "glm.h"
#include "ShaderProgram.h"
#include "System.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace en {

    class RenderSkyboxSystem : public System {

    public:
        RenderSkyboxSystem();
        void start() override;
        void draw() override;

    private:
        void renderSkyboxCubemap(const class Texture& cubemap, const glm::mat4& matrixPV);

        gl::VertexArrayObject m_vao;
        gl::VertexBufferObject m_buffer;
        std::shared_ptr<ShaderProgram> m_shader;

        std::shared_ptr<Texture> m_defaultSkybox;
    };
}

#endif //ENGINE_RENDERSKYBOXSYSTEM_H
