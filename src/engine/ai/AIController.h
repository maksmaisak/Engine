//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_AICONTROLLER_H
#define ENGINE_AICONTROLLER_H

#include <queue>
#include "Behavior.h"
#include "BehaviorTree.h"

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

        BehaviorTree* getBehaviorTree();
        void setBehaviorTree(std::unique_ptr<BehaviorTree> behaviorTree);

    private:
        std::unique_ptr<BehaviorTree> m_behaviorTree;
    };
}

#endif //ENGINE_AICONTROLLER_H
