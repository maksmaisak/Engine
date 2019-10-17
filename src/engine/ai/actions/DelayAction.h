//
// Created by Maksym Maisak on 13/10/19.
//

#ifndef ENGINE_DELAYACTION_H
#define ENGINE_DELAYACTION_H

#include <chrono>
#include <optional>
#include "Action.h"
#include "Scheduler.h"

namespace ai {

    class DelayAction : public Action {

    public:
        DelayAction(float duration);
        void reset() override;

    protected:
        ActionOutcome execute() override;

    private:
        float m_duration;
        en::TimerHandle m_timer;
    };
}

#endif //ENGINE_DELAYACTION_H
