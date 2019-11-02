//
// Created by Maksym Maisak on 2019-09-28.
//

#include "Action.h"
#include <imgui.h>

using namespace ai;

Action::Action() :
    m_blackboard(nullptr)
{}

ActionOutcome Action::execute(en::Actor& actor, Blackboard& blackboard) {

    m_actor = actor;
    m_blackboard = &blackboard;
    assert(m_blackboard);

    return execute();
}

void Action::display() {

    if (ImGui::TreeNode(this, "Action")) {
        ImGui::Text("Action::display() not implemented.");
        ImGui::TreePop();
    }
}
