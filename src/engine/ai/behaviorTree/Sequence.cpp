//
// Created by Maksym Maisak on 7/10/19.
//

#include "Sequence.h"

using namespace ai;

ActionOutcome Sequence::execute() {

    while (m_currentIndex < m_actions.size()) {

        std::unique_ptr<Action>& action = m_actions.at(m_currentIndex);
        assert(action);

        const ActionOutcome currentActionOutcome = action->execute(m_actor);
        switch (currentActionOutcome) {
            case ActionOutcome::InProgress:
                return ActionOutcome::InProgress;
            case ActionOutcome::Fail:
                reset();
                return ActionOutcome::Fail;
            case ActionOutcome::Success:
                ++m_currentIndex;
                break;
            default:
                assert(false && "Invalid ActionOutcome!");
                break;
        }
    }

    return ActionOutcome::Success;
}

void Sequence::reset() {

    CompoundAction::reset();
    m_currentIndex = 0;
}
