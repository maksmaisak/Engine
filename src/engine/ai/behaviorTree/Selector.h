//
// Created by Maksym Maisak on 7/10/19.
//

#ifndef ENGINE_SELECTOR_H
#define ENGINE_SELECTOR_H

#include "CompoundAction.h"

namespace ai {

    /// Selector node of a behavior tree.
    /// Succeeds when any succeeds.
    /// Fails when all fail.
    class Selector : public CompoundAction {

    public:
        using CompoundAction::CompoundAction;
        void reset() override;
        void display() override;

    protected:
        ActionOutcome execute() override;

    private:
        std::size_t m_currentIndex = 0;
    };
}

#endif //ENGINE_SELECTOR_H
