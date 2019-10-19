//
// Created by Maksym Maisak on 12/10/19.
//

#include "ConditionDecorator.h"

using namespace ai;

ConditionDecorator::ConditionDecorator(condition_t condition, std::unique_ptr<Action> child) :
    DecoratorAction(std::move(child)),
    m_condition(std::move(condition))
{}

ActionOutcome ConditionDecorator::execute() {

    assert(m_blackboard);
    if (m_condition && !m_condition(m_actor, *m_blackboard)) {
        return ActionOutcome::Fail;
    }

    if (!m_child) {
        return ActionOutcome::Fail;
    }

    return m_child->execute(m_actor, *m_blackboard);
}
