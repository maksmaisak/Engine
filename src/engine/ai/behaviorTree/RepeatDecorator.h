//
// Created by Maksym Maisak on 13/10/19.
//

#ifndef ENGINE_REPEATDECORATOR_H
#define ENGINE_REPEATDECORATOR_H

#include "DecoratorAction.h"

namespace ai {

    /// Repeats its action forever, resets it if it succeeds or fails.
    class RepeatDecorator : public DecoratorAction {

    public:
        using DecoratorAction::DecoratorAction;

    protected:
        ActionOutcome execute() override;
    };
}

#endif //ENGINE_REPEATDECORATOR_H
