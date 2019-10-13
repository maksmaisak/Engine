//
// Created by Maksym Maisak on 13/10/19.
//

#include "SimpleParallelAction.h"

using namespace ai;

SimpleParallelAction::SimpleParallelAction(std::unique_ptr<Action> mainAction, std::unique_ptr<Action> backgroundAction) :
    m_mainAction(std::move(mainAction)),
    m_backgroundAction(std::move(backgroundAction))
{}

void SimpleParallelAction::reset() {

    Action::reset();

    if (m_mainAction) {
        m_mainAction->reset();
    }

    if (m_backgroundAction) {
        m_backgroundAction->reset();
    }
}

ActionOutcome SimpleParallelAction::execute() {

    if (m_backgroundAction) {
        const ActionOutcome outcome = m_backgroundAction->execute(m_actor, m_blackboard);
        if (outcome != ActionOutcome::InProgress) {
            m_backgroundAction->reset();
        }
    }

    if (m_mainAction) {
        return m_mainAction->execute(m_actor, m_blackboard);
    }

    return ActionOutcome::Fail;
}
