//
// Created by Maksym Maisak on 12/10/19.
//

#ifndef ENGINE_CONDITIONDECORATOR_H
#define ENGINE_CONDITIONDECORATOR_H

#include "DecoratorAction.h"

namespace ai {

    /// Behavior tree decorator action that checks a condition every time before executing the child action.
    class ConditionDecorator : public DecoratorAction {

        using condition_t = std::function<bool(en::Actor&, Blackboard*)>;

    public:
        ConditionDecorator(condition_t condition, std::unique_ptr<Action> child);

    protected:
        ActionOutcome execute() override;

    private:
        condition_t m_condition;
    };
}


#endif //ENGINE_CONDITIONDECORATOR_H
