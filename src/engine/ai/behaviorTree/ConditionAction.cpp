//
// Created by Maksym Maisak on 8/10/19.
//

#include "ConditionAction.h"
#include <utility>

using namespace ai;

ConditionAction::ConditionAction(condition_t condition) :
    m_condition(std::move(condition))
{}

ActionOutcome ConditionAction::execute() {

    return (m_condition ? m_condition(m_actor) : false) ? ActionOutcome::Success : ActionOutcome::Fail;
}
