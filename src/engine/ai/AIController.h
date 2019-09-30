//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_AICONTROLLER_H
#define ENGINE_AICONTROLLER_H

#include <queue>
#include "Behavior.h"
#include "Action.h"

namespace en {
    class Engine;
}

namespace ai {

    class AIController : public en::Behavior {

        using Behavior::Behavior;
    public:
        static AIController& create(class en::Engine& engine);

        void start() override;
        void update(float dt) override;

    private:

        void enqueueAction(std::unique_ptr<Action>&& action);

        std::queue<std::unique_ptr<Action>> m_actionQueue;
    };
}

#endif //ENGINE_AICONTROLLER_H
