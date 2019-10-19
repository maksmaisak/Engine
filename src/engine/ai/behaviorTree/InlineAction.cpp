//
// Created by Maksym Maisak on 12/10/19.
//

#include "InlineAction.h"

using namespace ai;

namespace {

    InlineAction::execute_t makeExecuteFunction(InlineAction::executeNoReturn_t innerExecuteFunction) {

        return [innerExecuteFunction = std::move(innerExecuteFunction)](en::Actor& actor, Blackboard& blackboard) {

            innerExecuteFunction(actor, blackboard);
            return ActionOutcome::Success;
        };
    }
}

InlineAction::InlineAction(execute_t executeFunction, reset_t resetFunction) :
    m_executeFunction(std::move(executeFunction)),
    m_resetFunction(std::move(resetFunction))
{}

InlineAction::InlineAction(executeNoReturn_t executeFunction, reset_t resetFunction) :
    InlineAction(makeExecuteFunction(std::move(executeFunction)), std::move(resetFunction))
{}

void InlineAction::reset() {

    Action::reset();

    if (m_resetFunction) {
        m_resetFunction(m_actor);
    }
}

ActionOutcome InlineAction::execute() {

    if (m_executeFunction) {
        return m_executeFunction(m_actor, *m_blackboard);
    }

    return ActionOutcome::Fail;
}
