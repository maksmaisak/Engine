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

    constexpr std::size_t NumComponentsPerVertex = 3 + 1 + 4; // 3 for position, 1 for line width, 4 for color
    constexpr std::size_t InitialVertexCapacity = 100;

    glm::vec2 getInverseWindowSize(Engine& engine) {

        const sf::Vector2u size = engine.getWindow().getSize();
        return {1.f / size.x, 1.f / size.y};
    }
}

LineRenderer& LineRenderer::get(Engine& engine) {
    return GlobalComponents::getOrAddGlobalComponent<LineRenderer>(engine);
}

LineRenderer::LineRenderer(Actor& actor, std::size_t maxNumVerticesPerDrawCall) :
    Behavior(actor),
    m_maxNumVerticesPerDrawCall(maxNumVerticesPerDrawCall),
    m_wireframeShader(Resources<ShaderProgram>::get("line")),
    m_vao(gl::ForceCreate),
    m_vbo(gl::ForceCreate)
{
    assert(m_wireframeShader);

    m_vertexData.reserve(InitialVertexCapacity * NumComponentsPerVertex);

    const auto bindVAO = gl::ScopedBind(m_vao);
    const auto bindVBO = gl::ScopedBind(m_vbo, GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NumComponentsPerVertex * m_maxNumVerticesPerDrawCall, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * NumComponentsPerVertex, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * NumComponentsPerVertex, (void*)(sizeof(GLfloat) * 4));
}

void LineRenderer::draw() {

    if (Actor cameraActor = getEngine().getMainCamera()) {

        const glm::mat4 matrixView = glm::inverse(cameraActor.get<Transform>().getWorldTransform());
        const glm::mat4 matrixProjection = cameraActor.get<Camera>().getCameraProjectionMatrix(getEngine());
        render(matrixProjection * matrixView);
    }
}

void LineRenderer::render(const glm::mat4& matrixPVM) {

    const std::size_t maxNumVertexComponentsPerDrawCall = m_maxNumVerticesPerDrawCall * NumComponentsPerVertex;

    useWireframeShader(matrixPVM);
    glDisable(GL_DEPTH_TEST);
    const auto bindVAO = gl::ScopedBind(m_vao);
    const auto bindVBO = gl::ScopedBind(m_vbo, GL_ARRAY_BUFFER);

    std:size_t startIndex = 0;
    while (startIndex < m_vertexData.size()) {

        const std::size_t numRenderedVertexComponents = std::min(m_vertexData.size() - startIndex, maxNumVertexComponentsPerDrawCall);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * numRenderedVertexComponents, m_vertexData.data() + startIndex);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(numRenderedVertexComponents / NumComponentsPerVertex));

        startIndex += numRenderedVertexComponents;
    }
    m_vertexData.clear();

    glEnable(GL_DEPTH_TEST);
}

void LineRenderer::addLineSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float lineWidth) {

    addVertex(start, color, lineWidth);
    addVertex(end, color, lineWidth);
}

void LineRenderer::addLineSegment(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float lineWidth) {

    addLineSegment(glm::vec3(start, 1.f), glm::vec3(end, 1.f), color, lineWidth);
}

void LineRenderer::addAABB(const Bounds2D& bounds, const glm::vec4& color, float lineWidth) {

    addLineSegment({bounds.min.x, bounds.min.y, 1.f}, {bounds.min.x, bounds.max.y, 1.f}, color, lineWidth);
    addLineSegment({bounds.min.x, bounds.max.y, 1.f}, {bounds.max.x, bounds.max.y, 1.f}, color, lineWidth);
    addLineSegment({bounds.max.x, bounds.max.y, 1.f}, {bounds.max.x, bounds.min.y, 1.f}, color, lineWidth);
    addLineSegment({bounds.max.x, bounds.min.y, 1.f}, {bounds.min.x, bounds.min.y, 1.f}, color, lineWidth);
}

void LineRenderer::addVertex(const glm::vec3& position, const glm::vec4& color, float lineWidth) {

    m_vertexData.push_back(position.x);
    m_vertexData.push_back(position.y);
    m_vertexData.push_back(position.z);
    m_vertexData.push_back(lineWidth);

    m_vertexData.push_back(color.r);
    m_vertexData.push_back(color.g);
    m_vertexData.push_back(color.b);
    m_vertexData.push_back(color.a);
}

void LineRenderer::useWireframeShader(const glm::mat4& matrixPVM) {

    assert(m_wireframeShader);
    m_wireframeShader->use();
    m_wireframeShader->setUniformValue("matrixPVM", matrixPVM);
    const sf::Vector2u viewportSize = getEngine().getWindow().getSize();
    m_wireframeShader->setUniformValue("viewportSize", glm::vec2(viewportSize.x, viewportSize.y));
    m_wireframeShader->setUniformValue("inverseViewportSize", glm::vec2(1.f / viewportSize.x, 1.f / viewportSize.y));
}
