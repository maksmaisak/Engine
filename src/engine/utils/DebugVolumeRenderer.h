//
// Created by Maksym Maisak on 2019-04-02.
//

#ifndef ENGINE_DEBUGVOLUMERENDERER_H
#define ENGINE_DEBUGVOLUMERENDERER_H

#include <vector>
#include <GL/glew.h>
#include "glm.h"
#include "Engine.h"
#include "Resources.h"
#include "ShaderProgram.hpp"

namespace en {

    class DebugVolumeRenderer {

    public:
        DebugVolumeRenderer(std::size_t maxNumVerticesPerDrawCall = 32768);

        void addAABB(const glm::vec3& center, const glm::vec3& halfSize, const glm::vec4& color = {1, 1, 1, 1});
        void addAABB(const glm::vec3& center, const glm::vec3& halfSize, std::size_t numEntities);
        void render(const glm::mat4& matrixPVM);

    private:

        std::size_t m_maxNumVerticesPerDrawCall = 0;

        std::shared_ptr<ShaderProgram> m_wireframeShader;

        GLuint m_vao = 0;
        GLuint m_vbo = 0;

        std::vector<float> m_vertexData;
    };
}

#endif //ENGINE_DEBUGVOLUMERENDERER_H
