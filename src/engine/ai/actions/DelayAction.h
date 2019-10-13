//
// Created by Maksym Maisak on 13/10/19.
//

#ifndef ENGINE_DELAYACTION_H
#define ENGINE_DELAYACTION_H

#include <chrono>
#include <optional>
#include "Action.h"

namespace ai {

    class DelayAction : public Action {

    public:
        DelayAction(float duration);
        void reset() override;

    protected:
        ActionOutcome execute() override;

    private:
        float m_duration;
        std::optional<float> m_startTime;
    };
}

#endif //ENGINE_DELAYACTION_H
