//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef SAXION_Y2Q2_RENDERING_COLLISIONDETECTIONDISCRETE_H
#define SAXION_Y2Q2_RENDERING_COLLISIONDETECTIONDISCRETE_H

#include <optional>
#include "glm.hpp"

namespace en {

    struct Hit {
        glm::vec3 normal;
    };

    struct SphereCollider;
    struct AABBCollider;

    namespace collisionDetectionDiscrete {

        std::optional<Hit> sphereVsSphere(SphereCollider& a, SphereCollider& b);
        std::optional<Hit> AABBVsAABB    (AABBCollider&   a, AABBCollider&   b);
        std::optional<Hit> sphereVsAABB  (SphereCollider& a, AABBCollider&   b);
    };
}

#endif //SAXION_Y2Q2_RENDERING_COLLISIONDETECTIONDISCRETE_H
