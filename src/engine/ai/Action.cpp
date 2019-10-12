//
// Created by Maksym Maisak on 2019-09-28.
//

#include "Action.h"

using namespace ai;

Action::Action() :
    m_actor(),
    m_blackboard(nullptr)
{}

ActionOutcome Action::execute(en::Actor& actor, Blackboard* blackboard) {

    m_actor = actor;
    m_blackboard = blackboard;
    return execute();
}
