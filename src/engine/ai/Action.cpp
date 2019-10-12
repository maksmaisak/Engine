//
// Created by Maksym Maisak on 2019-09-28.
//

#include "Action.h"

using namespace ai;

ActionOutcome Action::execute(en::Actor& actor) {

    m_actor = actor;
    return execute();
}
