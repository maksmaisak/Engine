
#include "DebugHud.h"
#include <iostream>
#include <GL/glew.h>
#include <imgui.h>
#include "Engine.h"
#include "VertexRenderer.h"

using namespace en;

void DebugHud::draw(const Info& info) {

    glDisable(GL_DEPTH_TEST);
    ImGui::Text("fps: %i, render time: %f ms", glm::iround(info.fps), info.frameTimeMicroseconds / 1000.0);
    glEnable(GL_DEPTH_TEST);
}
