
#include "DebugHud.hpp"
#include <iostream>
#include <sstream>
#include <GL/glew.h>

using namespace en;

DebugHud::DebugHud(Engine& engine, VertexRenderer& vertexRenderer) :
    m_engine(&engine),
    m_vertexRenderer(&vertexRenderer) {}

void DebugHud::setDebugInfo(const Info& info) {

    std::stringstream s;
    s << "fps: " << glm::iround(info.fps) << "\nrender time: " << info.frameTimeMicroseconds / 1000.0 << "ms";
    m_text.setString(s.str());
}

void DebugHud::draw() {

    glDisable(GL_DEPTH_TEST);

    const auto& vertices = m_text.getVertices();
    const glm::vec2& boundsMin = m_text.getBoundsMin();
    const glm::vec2& boundsMax = m_text.getBoundsMax();

    auto size = m_engine->getWindow().getSize();
    m_text.getMaterial()->use(
        m_engine,
        nullptr,
        glm::mat4(1.f),
        glm::mat4(1.f),
        glm::ortho(0.f, (float)size.x, 0.f, (float)size.y) * glm::translate(glm::vec3(-boundsMin, 0.f))
    );
    m_vertexRenderer->renderVertices(vertices);

    glEnable(GL_DEPTH_TEST);
}
