//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef ENGINE_COLLISIONDETECTION_H
#define ENGINE_COLLISIONDETECTION_H

#include <optional>
#include "glm.hpp"
#include "Hit.h"

namespace en {

    struct SphereCollider;
    struct AABBCollider;

    namespace collisionDetection {

        std::optional<Hit> sphereVsSphere(SphereCollider& a, SphereCollider& b, const glm::vec3& movement);
        std::optional<Hit> AABBVsAABB    (AABBCollider&   a, AABBCollider&   b, const glm::vec3& movement);
        std::optional<Hit> sphereVsAABB  (SphereCollider& a, AABBCollider&   b, const glm::vec3& movement);
        std::optional<Hit> AABBVsSphere  (AABBCollider&   b, SphereCollider& a, const glm::vec3& movement);
    };
}

#endif //ENGINE_COLLISIONDETECTION_H
