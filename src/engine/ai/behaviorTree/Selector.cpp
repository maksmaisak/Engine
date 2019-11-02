//
// Created by Maksym Maisak on 7/10/19.
//

#include "Selector.h"
#include <imgui.h>

using namespace ai;

ActionOutcome Selector::execute() {

    while (m_currentIndex < m_actions.size()) {

        std::unique_ptr<Action>& action = m_actions.at(m_currentIndex);
        assert(action);
        assert(m_blackboard);
        const ActionOutcome currentActionOutcome = action->execute(m_actor, *m_blackboard);
        switch (currentActionOutcome) {
            case ActionOutcome::InProgress:
                return ActionOutcome::InProgress;
            case ActionOutcome::Success:
                reset();
                return ActionOutcome::Success;
            case ActionOutcome::Fail:
                action->reset();
                ++m_currentIndex;
                break;
            default:
                assert(false && "Invalid ActionOutcome!");
                break;
        }
    }

    reset();
    return ActionOutcome::Fail;
}

void Selector::reset() {

    CompoundAction::reset();
    m_currentIndex = 0;
}

void Selector::display() {

    if (ImGui::TreeNode(this, "Selector")) {

        for (std::size_t i = 0; i < m_actions.size(); ++i) {

            if (!m_actions[i]) {
                if (ImGui::TreeNode("Nullptr")) {
                    ImGui::TreePop();
                }
                continue;
            }

            if (i == m_currentIndex) {
                ImGui::Bullet();
            }
            m_actions[i]->display();
        }

        ImGui::TreePop();
    }
}
