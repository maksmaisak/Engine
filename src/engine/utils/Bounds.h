//
// Created by Maksym Maisak on 2019-03-22.
//

#ifndef ENGINE_BOUNDS_H
#define ENGINE_BOUNDS_H

#include "glm.hpp"

namespace utils {

    struct Bounds {
        glm::vec3 min;
        glm::vec3 max;
    };
}

#endif //ENGINE_BOUNDS_H
