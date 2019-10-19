//
// Created by Maksym Maisak on 15/10/19.
//

#ifndef ENGINE_WHILEDECORATOR_H
#define ENGINE_WHILEDECORATOR_H

#include <functional>
#include "DecoratorAction.h"

namespace ai {

    class WhileDecorator : public DecoratorAction {

        using condition_t = std::function<bool(en::Actor&, Blackboard&)>;

    public:
        WhileDecorator(condition_t condition, std::unique_ptr<Action> child);

    protected:
        ActionOutcome execute() override;

    private:
        condition_t m_condition;
    };
}

#endif //ENGINE_WHILEDECORATOR_H
