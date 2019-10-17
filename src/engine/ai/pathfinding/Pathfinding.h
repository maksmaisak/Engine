//
// Created by Maksym Maisak on 29/9/19.
//

#ifndef ENGINE_PATHFINDING_H
#define ENGINE_PATHFINDING_H

#include <optional>
#include "PathfindingPath.h"

namespace en {
    class Engine;
}

namespace ai {

    struct PathfindingParams {

        PathfindingParams();

        std::size_t maxNumCheckedTiles;
        bool allowObstacleGoal;
    };

    class Pathfinding {

    public:

        static std::optional<PathfindingPath> getPath(en::Engine& engine, const en::GridPosition& start,
            const en::GridPosition& goal,
            const PathfindingParams& params = {}
        );

        static std::optional<PathfindingPath> getPath(en::Engine& engine, const en::GridPosition& start,
            const std::function<bool(const en::GridPosition&)>& isGoal,
            const std::function<float(const en::GridPosition&)>& heuristic = nullHeuristic,
            const PathfindingParams& params = {}
        );

        static bool isObstacle(en::Engine& engine, const en::GridPosition& tileCoordinates);

        static inline float nullHeuristic(const en::GridPosition& position) {return 0.f;}
    };
}

#endif //ENGINE_PATHFINDING_H
