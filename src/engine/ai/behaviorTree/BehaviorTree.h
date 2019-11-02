//
// Created by Maksym Maisak on 9/10/19.
//

#ifndef ENGINE_BEHAVIORTREE_H
#define ENGINE_BEHAVIORTREE_H

#include <memory>
#include "Action.h"
#include "Blackboard.h"

namespace en {
    class Actor;
}

namespace ai {

    class BehaviorTree final {

    public:
        BehaviorTree(std::unique_ptr<Action> root, std::unique_ptr<Blackboard> blackboard = nullptr);
        void execute(en::Actor& actor);

        Blackboard& getBlackboard();
        const Blackboard& getBlackboard() const;

        /// Display the current state of the behavior tree using imgui
        void display() const;

    private:

        std::unique_ptr<Blackboard> m_blackboard;
        std::unique_ptr<Action> m_root;
    };
}

#endif //ENGINE_BEHAVIORTREE_H
