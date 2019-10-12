//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_ACTION_H
#define ENGINE_ACTION_H

#include "ActorDecl.h"

namespace ai {

    enum class ActionOutcome {
        InProgress,
        Success,
        Fail
    };

    class Action {

    public:
        Action();

        virtual ~Action() = default;
        ActionOutcome execute(en::Actor& actor, class Blackboard* blackboard);
        virtual inline void reset() {};

    protected:
        virtual ActionOutcome execute() = 0;

        en::Actor m_actor;
        Blackboard* m_blackboard;
    };
}

#endif //ENGINE_ACTION_H
