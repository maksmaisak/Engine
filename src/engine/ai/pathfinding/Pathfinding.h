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

    class Pathfinding {

    public:
        static std::optional<PathfindingPath> getPath(en::Engine& engine, const en::GridPosition& start, const en::GridPosition& end, std::size_t maxNumCheckedTiles = 100000);
    };
}

#endif //ENGINE_PATHFINDING_H
