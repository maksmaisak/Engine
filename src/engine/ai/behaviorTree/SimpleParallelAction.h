//
// Created by Maksym Maisak on 13/10/19.
//

#ifndef ENGINE_SIMPLEPARALLELACTION_H
#define ENGINE_SIMPLEPARALLELACTION_H

#include "Action.h"

namespace ai {

    /// Executes the main action while the background action loops in the background.
    class SimpleParallelAction : public Action {

    public:
        SimpleParallelAction(std::unique_ptr<Action> mainAction, std::unique_ptr<Action> backgroundAction);
        void reset() override;

    protected:
        ActionOutcome execute() override;

    private:
        std::unique_ptr<Action> m_mainAction;
        std::unique_ptr<Action> m_backgroundAction;
    };
}

#endif //ENGINE_SIMPLEPARALLELACTION_H
