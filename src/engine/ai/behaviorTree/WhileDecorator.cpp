//
// Created by Maksym Maisak on 15/10/19.
//

#include "WhileDecorator.h"

using namespace ai;

WhileDecorator::WhileDecorator(condition_t condition, std::unique_ptr<Action> child) :
    DecoratorAction(std::move(child)),
    m_condition(std::move(condition))
{}

ActionOutcome WhileDecorator::execute() {

    assert(m_blackboard);
    if (!m_condition || !m_condition(m_actor, *m_blackboard)) {
        return ActionOutcome::Success;
    }

    if (!m_child) {
        return ActionOutcome::Fail;
    }

    return m_child->execute(m_actor, *m_blackboard);
}
