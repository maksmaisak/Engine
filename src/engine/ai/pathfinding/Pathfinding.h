//
// Created by Maksym Maisak on 29/9/19.
//

#ifndef ENGINE_PATHFINDING_H
#define ENGINE_PATHFINDING_H

#include "PathfindingPath.h"
#include <optional>

namespace en {
    class Engine;
}

namespace ai {

    class Pathfinding {

    public:
        static std::optional<PathfindingPath> getPath(en::Engine& engine, const glm::i64vec2& start, const glm::i64vec2& end, std::size_t maxNumCheckedTiles = 100000);
    };
}

#endif //ENGINE_PATHFINDING_H
