//
// Created by Maksym Maisak on 12/10/19.
//

#ifndef ENGINE_INLINEACTION_H
#define ENGINE_INLINEACTION_H

#include <functional>
#include "Action.h"

namespace ai {

    /// A behavior tree action that can be made from a lambda.
    class InlineAction : public Action {

    public:
        using execute_t = std::function<ActionOutcome(en::Actor&, Blackboard*)>;
        using reset_t = std::function<void(en::Actor&)>;

        InlineAction(execute_t executeFunction, reset_t resetFunction = {});
        InlineAction(std::function<void(en::Actor&, Blackboard*)> executeFunction, reset_t resetFunction = {});
        void reset() override;

    protected:
        ActionOutcome execute() override;

    private:
        execute_t m_executeFunction;
        reset_t m_resetFunction;
    };
}

#endif //ENGINE_INLINEACTION_H
