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

    glm::vec4 getColorFromNumEntities(std::size_t numEntities) {

        constexpr float MAX_NUM_ENTITIES_IN_CELL = 10.f;

        const float t = glm::saturate<float, glm::defaultp>((numEntities - 1.f) / MAX_NUM_ENTITIES_IN_CELL);
        return glm::mix(glm::vec4(1, 1, 1, 0.8f), glm::vec4(1, 0, 0, 1), glm::vec4(t));
    }
}

DebugVolumeRenderer::DebugVolumeRenderer(std::size_t maxNumVerticesPerDrawCall) :
    m_maxNumVerticesPerDrawCall(maxNumVerticesPerDrawCall),
    m_wireframeShader(Resources<ShaderProgram>::get("wireframe")),
    m_vao(gl::CreateImmediately{}),
    m_vbo(gl::CreateImmediately{})
{
    m_vertexData.reserve(100 * 7);

    m_vao.bind();
    {
        m_vbo.bind(GL_ARRAY_BUFFER);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * m_maxNumVerticesPerDrawCall, nullptr, GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, nullptr);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)(sizeof(GLfloat) * 3));
        }
        m_vbo.unbind(GL_ARRAY_BUFFER);
    }
    m_vao.unbind();
}

void DebugVolumeRenderer::addAABB(const glm::vec3& center, const glm::vec3& halfSize, const glm::vec4& color) {

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

void DebugVolumeRenderer::addAABBMinMax(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color) {

    const glm::vec3 center = (min + max) * 0.5f;
    addAABB(center, max - center, color);
}

void DebugVolumeRenderer::addAABB(const glm::vec3& center, const glm::vec3& halfSize, std::size_t numEntities) {

    addAABB(center, halfSize, getColorFromNumEntities(numEntities));
}

void DebugVolumeRenderer::render(const glm::mat4& matrixPVM) {

    m_wireframeShader->use();
    m_wireframeShader->setUniformValue("matrixPVM", matrixPVM);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_vao.bind();
    {
        m_vbo.bind(GL_ARRAY_BUFFER);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data());
        m_vbo.unbind(GL_ARRAY_BUFFER);

        glDrawArrays(GL_LINES, 0, (GLsizei)(m_vertexData.size() / 7));
        glCheckError();
    }
    m_vao.unbind();

    m_vertexData.clear();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

