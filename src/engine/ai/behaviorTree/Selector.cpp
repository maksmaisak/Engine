//
// Created by Maksym Maisak on 7/10/19.
//

#include "Selector.h"

using namespace ai;

ActionOutcome Selector::execute() {

    while (m_currentIndex < m_actions.size()) {

        std::unique_ptr<Action>& action = m_actions.at(m_currentIndex);
        assert(action);

        const ActionOutcome currentActionOutcome = action->execute(m_actor);
        switch (currentActionOutcome) {
            case ActionOutcome::InProgress:
                return ActionOutcome::InProgress;
            case ActionOutcome::Success:
                reset();
                return ActionOutcome::Success;
            case ActionOutcome::Fail:
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
