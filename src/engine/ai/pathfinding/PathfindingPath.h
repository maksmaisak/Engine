//
// Created by Maksym Maisak on 29/9/19.
//

#ifndef ENGINE_PATHFINDINGPATH_H
#define ENGINE_PATHFINDINGPATH_H

#include <deque>
#include "TileLayer.h"

namespace ai {

    using PathfindingPath = std::deque<en::TileLayer::Coordinates>;
}

#endif //ENGINE_PATHFINDINGPATH_H
