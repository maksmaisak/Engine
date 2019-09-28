//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_TILE_H
#define ENGINE_TILE_H

#include "glm.h"

namespace en {

    struct Tile final {

        using AtlasCoordinate = uint8_t;
        using AtlasCoordinates = glm::vec<2, AtlasCoordinate>;

        AtlasCoordinates atlasCoordinates = {0, 0};

        bool isObstacle = false;
    };
}

#endif //ENGINE_TILE_H
