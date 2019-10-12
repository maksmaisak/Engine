//
// Created by Maksym Maisak on 9/10/19.
//

#ifndef ENGINE_PARALLELACTION_H
#define ENGINE_PARALLELACTION_H

#include "CompoundAction.h"

namespace ai {

    /// Parallel action of a behavior tree.
    /// Executes multiple actions at the same time.
    /// Succeeds if all succeed.
    /// Fails if any fail.
    class ParallelAction : public CompoundAction {

    public:
        using CompoundAction::CompoundAction;

    protected:
        ActionOutcome execute() override;
    };
}

#endif //ENGINE_PARALLELACTION_H
