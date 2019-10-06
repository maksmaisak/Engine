//
// Created by Maksym Maisak on 2019-03-22.
//

#ifndef ENGINE_BOUNDS_H
#define ENGINE_BOUNDS_H

#include "glm.h"

namespace utils {

    struct Bounds2D;

    struct Bounds {

        Bounds();
        Bounds(const glm::vec3& min, const glm::vec3& max);
        Bounds(const Bounds2D& other);

        template<typename TIterator>
        inline static Bounds from(TIterator begin, TIterator end) {

            Bounds bounds {
                glm::vec3(std::numeric_limits<float>::max()),
                glm::vec3(std::numeric_limits<float>::min())
            };

            for (TIterator it = begin; it != end; ++it) {
                 bounds.add(*it);
            }

            return bounds;
        }

        bool contains(const glm::vec3& position) const;
        bool intersects(const Bounds& other) const;

        glm::vec3 clamp(const glm::vec3& other) const;
        Bounds clamp(const Bounds& other) const;

        void add(const glm::vec3& point);
        void expandByMovement(const glm::vec3& movement);

        glm::vec3 min;
        glm::vec3 max;
    };

    struct Bounds2D {

        Bounds2D();
        Bounds2D(const glm::vec2& min, const glm::vec2& max);
        Bounds2D(const Bounds& other);

        template<typename TIterator>
        inline static Bounds2D from(TIterator begin, TIterator end) {

            Bounds2D bounds {
                glm::vec2(std::numeric_limits<float>::max()),
                glm::vec2(std::numeric_limits<float>::min())
            };

            for (TIterator it = begin; it != end; ++it) {
                bounds.add(*it);
            }

            return bounds;
        }

        bool contains(const glm::vec2& position) const;
        bool intersects(const Bounds2D& other) const;

        glm::vec2 clamp(const glm::vec2& other) const;
        Bounds2D clamp(const Bounds2D& other) const;

        void add(const glm::vec2& point);
        void expandByMovement(const glm::vec2& movement);

        glm::vec2 min;
        glm::vec2 max;
    };

    struct BoundingSphere {
        glm::vec3 position;
        float radius = 0.f;
    };
}

#endif //ENGINE_BOUNDS_H
