//
// Created by Maksym Maisak on 2019-04-02.
//

#include "DebugVolumeRenderer.h"
#include <algorithm>
#include <cassert>
#include "GLHelpers.h"

using namespace en;

namespace {

    const glm::vec3 cubeVertices[24] {
        {-1, -1, -1}, { 1, -1, -1},
        {-1, -1, -1}, {-1, -1,  1},
        { 1, -1,  1}, { 1, -1, -1},
        { 1, -1,  1}, {-1, -1,  1},

        {-1, -1, -1}, {-1,  1, -1},
        {-1, -1,  1}, {-1,  1,  1},
        { 1, -1, -1}, { 1,  1, -1},
        { 1, -1,  1}, { 1,  1,  1},

        {-1,  1, -1}, { 1,  1, -1},
        {-1,  1, -1}, {-1,  1,  1},
        { 1,  1,  1}, { 1,  1, -1},
        { 1,  1,  1}, {-1,  1,  1}
    };
}

DebugVolumeRenderer::DebugVolumeRenderer(std::size_t maxNumVerticesPerDrawCall) :
    m_maxNumVerticesPerDrawCall(maxNumVerticesPerDrawCall),
    m_wireframeShader(Resources<ShaderProgram>::get("wireframe"))
{
    m_vertexData.reserve(100 * 7);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * m_maxNumVerticesPerDrawCall, nullptr, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)(sizeof(GLfloat) * 3));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
}

void DebugVolumeRenderer::addCube(const glm::vec3& center, const glm::vec3& halfSize, const glm::vec4& color) {

    assert(m_vertexData.size() + 24 * 7 < m_maxNumVerticesPerDrawCall * 7 && "Can't render that many vertices in one go.");

    for (const glm::vec3& pos : cubeVertices) {

        const glm::vec3 vertex = pos * halfSize + center;
        m_vertexData.push_back(vertex.x);
        m_vertexData.push_back(vertex.y);
        m_vertexData.push_back(vertex.z);
        m_vertexData.push_back(color.r);
        m_vertexData.push_back(color.g);
        m_vertexData.push_back(color.b);
        m_vertexData.push_back(color.a);
    }
}

void DebugVolumeRenderer::render(const glm::mat4& matrixPVM) {

    m_wireframeShader->use();
    m_wireframeShader->setUniformValue("matrixPVM", matrixPVM);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_LINES, 0, (GLsizei)(m_vertexData.size() / 7));
        glCheckError();
    }
    glBindVertexArray(0);

    m_vertexData.clear();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

