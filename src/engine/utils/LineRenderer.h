//
// Created by Maksym Maisak on 5/10/19.
//

#ifndef ENGINE_LINERENDERER_H
#define ENGINE_LINERENDERER_H

#include <vector>
#include <memory>
#include "Behavior.h"
#include "glm.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace en {

    class LineRenderer : public Behavior {

    public:
        static LineRenderer& get(Engine& engine);
        explicit LineRenderer(class Actor& actor, std::size_t maxNumVerticesPerDrawCall = 32768);
        void draw() override;

        void addLineSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color = glm::vec4(1.f));
        void addLineSegment(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color = glm::vec4(1.f));

    private:
        void render(const glm::mat4& matrixPVM);
        void addVertex(const glm::vec3& position, const glm::vec4& color);

        std::size_t m_maxNumVerticesPerDrawCall;
        std::shared_ptr<class ShaderProgram> m_wireframeShader;
        gl::VertexArrayObject m_vao;
        gl::VertexBufferObject m_vbo;
        std::vector<float> m_vertexData;
    };
}


#endif //ENGINE_LINERENDERER_H
