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
        using goalPredicate_t = std::function<bool(const en::GridPosition&)>;
        using heuristic_t = std::function<float(const en::GridPosition&)>;

        static std::optional<PathfindingPath> getPath(en::Engine& engine, const en::GridPosition& start,
            const en::GridPosition& goal,
            const PathfindingParams& params = {}
        );

        static std::optional<PathfindingPath> getPath(en::Engine& engine, const en::GridPosition& start,
            const goalPredicate_t& isGoal,
            const heuristic_t& heuristic = nullHeuristic,
            const PathfindingParams& params = {}
        );

        static bool isObstacle(en::Engine& engine, const en::GridPosition& tileCoordinates);

        static inline float nullHeuristic(const en::GridPosition& position) {return 0.f;}
    };
}

#endif //ENGINE_PATHFINDING_H
