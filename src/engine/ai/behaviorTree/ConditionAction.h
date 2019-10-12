//
// Created by Maksym Maisak on 8/10/19.
//

#ifndef ENGINE_CONDITIONACTION_H
#define ENGINE_CONDITIONACTION_H

#include <functional>
#include "Action.h"

namespace en {
    class Actor;
}

namespace ai {

    class ConditionAction : public Action {

    public:
        using condition_t = std::function<bool(en::Actor&)>;
        ConditionAction(condition_t condition);

    protected:
        ActionOutcome execute() override;

    private:
        condition_t m_condition;
    };
}

#endif //ENGINE_CONDITIONACTION_H
