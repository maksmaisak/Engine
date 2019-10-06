//
// Created by Maksym Maisak on 5/10/19.
//

#include "LineRenderer.h"
#include "Camera.h"
#include "GlobalComponents.h"
#include "ScopedBind.h"
#include "ShaderProgram.h"
#include "Transform.h"

using namespace en;

namespace {

    constexpr std::size_t NumComponentsPerVertex = 3 + 4; // 3 for position, 4 for color
    constexpr std::size_t InitialVertexCapacity = 100;
    constexpr float LineWidth = 1.f;

    glm::vec2 getHalfWidthInClipspace(Engine& engine) {

        constexpr float LineHalfWidth = LineWidth * 0.5f;

        const sf::Vector2u size = engine.getWindow().getSize();
        return {LineHalfWidth / size.x, LineHalfWidth / size.y};
    }
}

LineRenderer& LineRenderer::get(Engine& engine) {
    return GlobalComponents::getOrAddGlobalComponent<LineRenderer>(engine);
}

LineRenderer::LineRenderer(Actor& actor, std::size_t maxNumVerticesPerDrawCall) :
    Behavior(actor),
    m_maxNumVerticesPerDrawCall(maxNumVerticesPerDrawCall),
    m_wireframeShader(Resources<ShaderProgram>::get("line")),
    m_vao(gl::ForceCreate{}),
    m_vbo(gl::ForceCreate{})
{
    assert(m_wireframeShader);

    m_vertexData.reserve(InitialVertexCapacity * NumComponentsPerVertex);

    const auto bindVAO = gl::ScopedBind(m_vao);
    {
        const auto bindVBO = gl::ScopedBind(m_vbo, GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NumComponentsPerVertex * m_maxNumVerticesPerDrawCall, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * NumComponentsPerVertex, nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * NumComponentsPerVertex, (void*)(sizeof(GLfloat) * 3));
    }
}

void LineRenderer::draw() {

    if (Actor cameraActor = getEngine().getMainCamera()) {

        const glm::mat4 matrixView = glm::inverse(cameraActor.get<Transform>().getWorldTransform());
        const glm::mat4 matrixProjection = cameraActor.get<Camera>().getCameraProjectionMatrix(getEngine());
        render(matrixProjection * matrixView);
    }
}

void LineRenderer::render(const glm::mat4& matrixPVM) {

    m_wireframeShader->use();
    m_wireframeShader->setUniformValue("matrixPVM", matrixPVM);
    m_wireframeShader->setUniformValue("halfWidthInClipspace", getHalfWidthInClipspace(getEngine()));

    {
        const auto bindVBO = gl::ScopedBind(m_vbo, GL_ARRAY_BUFFER);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data());
    }

    {
        const auto bindVAO = gl::ScopedBind(m_vao);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertexData.size() / NumComponentsPerVertex));
        glEnable(GL_DEPTH_TEST);
    }

    m_vertexData.clear();
}

void LineRenderer::addLineSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color) {

    addVertex(start, color);
    addVertex(end, color);
}

void LineRenderer::addLineSegment(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color) {
    addLineSegment(glm::vec3(start, 1.f), glm::vec3(end, 1.f), color);
}

void LineRenderer::addVertex(const glm::vec3& position, const glm::vec4& color) {

    assert(m_vertexData.size() + NumComponentsPerVertex <= m_maxNumVerticesPerDrawCall * NumComponentsPerVertex && "Can't render that many vertices in one go.");

    m_vertexData.push_back(position.x);
    m_vertexData.push_back(position.y);
    m_vertexData.push_back(position.z);

    m_vertexData.push_back(color.r);
    m_vertexData.push_back(color.g);
    m_vertexData.push_back(color.b);
    m_vertexData.push_back(color.a);
}
