//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_MOVEACTION_H
#define ENGINE_MOVEACTION_H

#include <optional>
#include <string>
#include "Action.h"
#include "glm.h"
#include "PathfindingPath.h"

namespace ai {

    class MoveAction : public Action {

    public:
        MoveAction();
        explicit MoveAction(const std::string& targetPositionBlackboardKey);

    protected:
        ActionOutcome execute() override;

    private:
        void reset() override;
        ActionOutcome followPathfindingPath();
        void drawPathfindingPath();

        std::string m_targetPositionBlackboardKey;
        std::optional<PathfindingPath> m_pathfindingPath;
    };
}

#endif //ENGINE_MOVEACTION_H
