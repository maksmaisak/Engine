//
// Created by Maksym Maisak on 12/10/19.
//

#ifndef ENGINE_DECORATORACTION_H
#define ENGINE_DECORATORACTION_H

#include "Action.h"

namespace ai {

    class DecoratorAction : public Action {

    public:
        DecoratorAction(std::unique_ptr<Action> child);
        void reset() override;
        void display() override;

    protected:
        std::unique_ptr<Action> m_child;
    };
}

#endif //ENGINE_DECORATORACTION_H
