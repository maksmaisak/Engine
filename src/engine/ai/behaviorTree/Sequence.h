//
// Created by Maksym Maisak on 7/10/19.
//

#ifndef ENGINE_SEQUENCE_H
#define ENGINE_SEQUENCE_H

#include "CompoundAction.h"

namespace ai {

    /// Sequence node of a behavior tree.
    /// Succeeds when all succeed.
    /// Fails when any fail.
    class Sequence : public CompoundAction {

    public:
        using CompoundAction::CompoundAction;
        void reset() override;

    protected:
        ActionOutcome execute() override;

    private:
        std::size_t m_currentIndex = 0;
    };
}

#endif //ENGINE_SEQUENCE_H
