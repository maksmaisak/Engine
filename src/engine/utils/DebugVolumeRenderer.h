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
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace en {

    class DebugVolumeRenderer {

    public:
        explicit DebugVolumeRenderer(std::size_t maxNumVerticesPerDrawCall = 32768);

        void addAABB(const glm::vec3& center, const glm::vec3& halfSize, const glm::vec4& color = {1, 1, 1, 1});
        void addAABBMinMax(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color = {1, 1, 1, 1});
        void addAABB(const glm::vec3& center, const glm::vec3& halfSize, std::size_t numEntities);
        void render(const glm::mat4& matrixPVM);

    private:
        std::size_t m_maxNumVerticesPerDrawCall = 0;
        std::shared_ptr<ShaderProgram> m_wireframeShader;
        gl::VertexArrayObject m_vao;
        gl::VertexBufferObject m_vbo;
        std::vector<float> m_vertexData;
    };
}

#endif //ENGINE_DEBUGVOLUMERENDERER_H
