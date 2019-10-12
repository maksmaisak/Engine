//
// Created by Maksym Maisak on 9/10/19.
//

#ifndef ENGINE_BEHAVIORTREE_H
#define ENGINE_BEHAVIORTREE_H

#include <memory>
#include "Action.h"

namespace en {
    class Actor;
}

namespace ai {

    class BehaviorTree final {

    public:
        BehaviorTree(std::unique_ptr<Action> root);
        void execute(en::Actor& actor);

    private:
        std::unique_ptr<Action> m_root;
    };
}

#endif //ENGINE_BEHAVIORTREE_H
