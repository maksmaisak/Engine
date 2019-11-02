//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_MOVEACTION_H
#define ENGINE_MOVEACTION_H

#include <optional>
#include "Action.h"
#include "glm.h"
#include "PathfindingPath.h"
#include "Name.h"

namespace ai {

    class MoveAction : public Action {

    public:
        MoveAction();
        /// The key may refer to either a GridPosition or an Actor
        explicit MoveAction(const en::Name& targetBlackboardKey);

        void reset() override;
        void display() override;

    protected:
        ActionOutcome execute() override;

    private:
        ActionOutcome followPathfindingPath();
        void drawPathfindingPath();

        en::Name m_targetBlackboardKey;
        std::optional<PathfindingPath> m_pathfindingPath;
    };
}

#endif //ENGINE_MOVEACTION_H
