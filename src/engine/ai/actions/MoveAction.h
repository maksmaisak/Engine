//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_MOVEACTION_H
#define ENGINE_MOVEACTION_H

#include <optional>
#include "Action.h"
#include "glm.h"
#include "PathfindingPath.h"

namespace ai {

    class MoveAction : public Action {

    public:
        MoveAction(const glm::i64vec2& targetPosition);
        ActionOutcome execute() override;

    private:
        glm::i64vec2 m_targetPosition;
        std::optional<PathfindingPath> m_pathfindingPath;
    };
}

#endif //ENGINE_MOVEACTION_H
