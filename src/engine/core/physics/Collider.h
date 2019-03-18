//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef SAXION_Y2Q2_RENDERING_COLLIDER_H
#define SAXION_Y2Q2_RENDERING_COLLIDER_H

#include <optional>
#include "glm.hpp"
#include "CollisionDetectionDiscrete.h"
#include "Transform.h"

namespace en {

    struct SphereCollider;
    struct AABBCollider;

    struct Collider {

        virtual void updateTransform(const glm::mat4& transform) = 0;

        // Base case when the other is indeterminate
        virtual std::optional<Hit> collide(Collider& other) = 0;

        virtual std::optional<Hit> collide(SphereCollider& other) = 0;
        virtual std::optional<Hit> collide(AABBCollider& other)   = 0;
    };
}

#endif //SAXION_Y2Q2_RENDERING_COLLIDER_H
