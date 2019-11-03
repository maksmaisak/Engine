//
// Created by Maksym Maisak on 3/11/19.
//

#include "PostProcessingPass.h"
#include <imgui.h>

using namespace en;

PostProcessingPass::PostProcessingPass() :
    m_isEnabled(true)
{}

void PostProcessingPass::setUp(const glm::u32vec2& size) {}

void PostProcessingPass::displayImGui() {}

bool PostProcessingPass::getIsEnabled() const {
    return m_isEnabled;
}

bool PostProcessingPass::setIsEnabled(bool isEnabled) {
    return m_isEnabled = isEnabled;
}

bool PostProcessingPass::updateIsEnabled() {

    ImGui::PushID(this);
    ImGui::Checkbox("Enabled", &m_isEnabled);
    ImGui::PopID();

    return m_isEnabled;
}
