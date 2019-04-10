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

        bool intersect(const Bounds& other) const;

        glm::vec3 clamp(const glm::vec3& other) const;
        Bounds clamp(const Bounds& other) const;

        void expandByMovement(const glm::vec3& vec);
    };

    struct BoundingSphere {
        glm::vec3 position;
        float radius = 0.f;
    };
}

#endif //ENGINE_BOUNDS_H
