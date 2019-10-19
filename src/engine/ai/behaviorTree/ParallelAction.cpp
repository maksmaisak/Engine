//
// Created by Maksym Maisak on 9/10/19.
//

#include "ParallelAction.h"

using namespace ai;

ActionOutcome ParallelAction::execute() {

    bool anyInProgress = false;
    bool anyFailed = false;
    for (std::unique_ptr<Action>& action : m_actions) {

        assert(action);
        if (action) {
            switch(action->execute(m_actor, m_blackboard)) {
                case ActionOutcome::InProgress:
                    anyInProgress = true;
                    break;
                case ActionOutcome::Fail:
                    anyFailed = true;
                    break;
                case ActionOutcome::Success:
                    break;
            }
        }
    }

    if (anyFailed) {
        return ActionOutcome::Fail;
    }

    if (anyInProgress) {
        return ActionOutcome::InProgress;
    }

    return ActionOutcome::Success;
}
