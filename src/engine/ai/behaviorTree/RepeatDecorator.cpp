//
// Created by Maksym Maisak on 13/10/19.
//

#include "RepeatDecorator.h"

using namespace ai;

ActionOutcome RepeatDecorator::execute() {

    if (!m_child) {
        return ActionOutcome::Fail;
    }

    const ActionOutcome outcome = m_child->execute(m_actor, *m_blackboard);
    switch (outcome) {

        case ActionOutcome::InProgress:
            return ActionOutcome::InProgress;
        case ActionOutcome::Success:
            reset();
            return ActionOutcome::InProgress;
        case ActionOutcome::Fail:
            reset();
            return ActionOutcome::InProgress;
    }
}
