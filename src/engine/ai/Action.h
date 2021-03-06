//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_ACTION_H
#define ENGINE_ACTION_H

#include "ActorDecl.h"

namespace ai {

    enum class ActionOutcome {
        InProgress = 0,
        Success,
        Fail
    };

    class Action {

    public:
        virtual ~Action() = default;
        virtual ActionOutcome execute() = 0;

        en::Actor actor;
    };
}

#endif //ENGINE_ACTION_H
