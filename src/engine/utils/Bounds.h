//
// Created by Maksym Maisak on 2019-03-22.
//

#ifndef ENGINE_BOUNDS_H
#define ENGINE_BOUNDS_H

#include "glm.h"
#include "Grid.h"

namespace en {

    template<glm::length_t L, typename T = float, glm::qualifier Q = glm::defaultp>
    struct Bounds {

        using vec_t = glm::vec<L, T, Q>;

        inline Bounds() :
            min(static_cast<T>(0)),
            max(static_cast<T>(0))
        {}

        inline Bounds(const vec_t& min, const vec_t& max) :
            min(min),
            max(max)
        {}

        template<glm::length_t LOther, typename TOther, glm::qualifier QOther = glm::defaultp>
        inline Bounds(const Bounds<LOther, TOther, QOther>& other) :
            min(other.min),
            max(other.max)
        {}

        template<typename TIterator>
        inline Bounds from(TIterator begin, TIterator end) {

            Bounds bounds {
                vec_t(std::numeric_limits<T>::max()),
                vec_t(std::numeric_limits<T>::lowest())
            };

            for (TIterator it = begin; it != end; ++it) {
                bounds.add(*it);
            }

            return bounds;
        }

        inline vec_t size() const {
            return max - min;
        }

        inline T area() const {

            T result = max[0] - min[0];
            for (glm::length_t i = 1; i < L; ++i) {
                result *= max[i] - min[i];
            }
            return result;
        }

        inline bool contains(const vec_t& position) const {

            for (glm::length_t i = 0; i < L; ++i) {
                if (position[i] > max[i] || position[i] < min[i]) {
                    return false;
                }
            }

            return true;
        }

        inline bool intersects(const Bounds& other) const {

            for (glm::length_t i = 0; i < L; ++i) {
                if (other.min[i] > max[i] || other.max[i] < min[i]) {
                    return false;
                }
            }

            return true;
        }

        inline vec_t clamp(const vec_t& point) const {

            return glm::clamp(point, min, max);
        }

        inline Bounds clamp(const Bounds& other) const {

            static_assert(
                std::is_floating_point_v<T>,
                "Bounds::clamp is only supported for floating-point values."
            );

            const vec_t otherHalfSize = (other.max - other.min) * static_cast<T>(0.5);
            const vec_t shrunkMin = min + otherHalfSize;
            const vec_t shrunkMax = max - otherHalfSize;
            const vec_t center = glm::clamp(other.max - otherHalfSize, shrunkMin, shrunkMax);

            return {center - otherHalfSize, center + otherHalfSize};
        }

        inline void add(const vec_t& point) {

            min = glm::min(min, point);
            max = glm::max(max, point);
        }

        inline void expandByMovement(const vec_t& movement) {

            for (glm::length_t i = 0; i < L; ++i) {
                if (movement[i] > static_cast<T>(0)) {
                    max[i] += movement[i];
                } else {
                    min[i] += movement[i];
                }
            }
        }

        vec_t min;
        vec_t max;
    };

    using Bounds2D = Bounds<2, float>;
    using Bounds3D = Bounds<3, float>;
    using Bounds2DGrid = Bounds<2, en::GridCoordinate>;

    struct BoundingSphere {
        glm::vec3 position;
        float radius = 0.f;
    };
}

#endif //ENGINE_BOUNDS_H
