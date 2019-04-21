//
// Created by Maksym Maisak on 2019-03-22.
//

#ifndef ENGINE_BOUNDS_H
#define ENGINE_BOUNDS_H

#include "glm.hpp"

namespace utils {

    struct Bounds2D;

    struct Bounds {

        glm::vec3 min;
        glm::vec3 max;

        Bounds() = default;
        Bounds(const glm::vec3& min, const glm::vec3& max);
        Bounds(const Bounds2D& other);

        bool intersect(const Bounds& other) const;

        glm::vec3 clamp(const glm::vec3& other) const;
        Bounds clamp(const Bounds& other) const;

        void expandByMovement(const glm::vec3& vec);
    };

    struct Bounds2D {

        glm::vec2 min;
        glm::vec2 max;

        Bounds2D() = default;
        Bounds2D(const glm::vec2& min, const glm::vec2& max);
        Bounds2D(const Bounds& other);

        bool intersect(const Bounds2D& other) const;

        glm::vec2 clamp(const glm::vec2& other) const;
        Bounds2D clamp(const Bounds2D& other) const;

        void expandByMovement(const glm::vec2& vec);
    };

    struct BoundingSphere {
        glm::vec3 position;
        float radius = 0.f;
    };
}

#endif //ENGINE_BOUNDS_H
