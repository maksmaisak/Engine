//
// Created by Maksym Maisak on 29/9/19.
//

#ifndef ENGINE_PATHFINDINGPATH_H
#define ENGINE_PATHFINDINGPATH_H

#include <queue>
#include "TileLayer.h"

namespace ai {

    using PathfindingPath = std::queue<en::TileLayer::Coordinates>;
}

#endif //ENGINE_PATHFINDINGPATH_H
