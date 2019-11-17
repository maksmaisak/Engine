//
// Created by Maksym Maisak on 2019-02-13.
//

#include "VertexRenderer.h"
#include <algorithm>
#include <cassert>
#include "ScopedBind.h"
#include "GLHelpers.h"

using namespace en;

VertexRenderer::VertexRenderer(std::size_t maxNumVerticesPerDrawCall) :
    m_maxNumVerticesPerDrawCall(maxNumVerticesPerDrawCall),
    m_vao(gl::ForceCreate),
    m_vbo(gl::ForceCreate)
{
    const gl::ScopedBind bindVAO(m_vao);
    const gl::ScopedBind bindVBO(m_vbo, GL_ARRAY_BUFFER);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * m_maxNumVerticesPerDrawCall, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
}

void VertexRenderer::renderVertices(const std::vector<Vertex>& vertices) {

    const std::size_t numFloats = vertices.size() * 5;
    auto data = std::make_unique<GLfloat[]>(numFloats);
    for (std::size_t i = 0; i < vertices.size(); ++i) {

        const Vertex& vertex = vertices[i];
        const std::size_t index = i * 5;
        data[index + 0] = vertex.position.x;
        data[index + 1] = vertex.position.y;
        data[index + 2] = vertex.position.z;
        data[index + 3] = vertex.uv.x;
        data[index + 4] = vertex.uv.y;
    }

    const gl::ScopedBind bindVAO(m_vao);

    {
        const gl::ScopedBind bindVBO(m_vbo, GL_ARRAY_BUFFER);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * numFloats, data.get());
    }

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    glCheckError();
}
