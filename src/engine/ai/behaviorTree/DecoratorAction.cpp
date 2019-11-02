//
// Created by Maksym Maisak on 12/10/19.
//

#include "DecoratorAction.h"
#include <imgui.h>

using namespace ai;

DecoratorAction::DecoratorAction(std::unique_ptr<Action> child) :
    m_child(std::move(child))
{}

void DecoratorAction::reset() {

    Action::reset();

    if (m_child) {
        m_child->reset();
    }
}

void DecoratorAction::display() {

    if (ImGui::TreeNode(this, "Decorator")) {

        if (!m_child) {
            ImGui::Text("No child");
        } else {
            ImGui::Bullet();
            m_child->display();
        }

        ImGui::TreePop();
    }
}
