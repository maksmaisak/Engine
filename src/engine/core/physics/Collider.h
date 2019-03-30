//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef ENGINE_COLLIDER_H
#define ENGINE_COLLIDER_H

#include <optional>
#include "glm.hpp"
#include "CollisionDetection.h"
#include "Transform.h"

namespace en {

    struct SphereCollider;
    struct AABBCollider;
    struct OBBCollider;

    struct Collider {

        virtual ~Collider() = default;
        virtual void updateTransform(const glm::mat4& transform) = 0;

        // Base case when the other is indeterminate
        virtual std::optional<Hit> collide(Collider& other, const glm::vec3& movement) = 0;
        virtual std::optional<Hit> collide(SphereCollider& other, const glm::vec3& movement) = 0;
        virtual std::optional<Hit> collide(AABBCollider&   other, const glm::vec3& movement) = 0;
        virtual std::optional<Hit> collide(OBBCollider&    other, const glm::vec3& movement) = 0;
    };
}

#endif //ENGINE_COLLIDER_H
