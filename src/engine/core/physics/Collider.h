//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef ENGINE_COLLIDER_H
#define ENGINE_COLLIDER_H

#include <optional>
#include <utility>
#include "glm.h"
#include "CollisionDetection.h"
#include "Transform.h"
#include "Bounds.h"

namespace en {

    struct Collider {

        virtual ~Collider() = default;
        virtual void updateTransform(const glm::mat4& transform) = 0;
        virtual Bounds3D getBounds() = 0;
        virtual BoundingSphere getBoundingSphere() = 0;

        // Base case when the other is indeterminate
        virtual std::optional<Hit> collide(Collider& other, const glm::vec3& movement) = 0;
        virtual std::optional<Hit> collide(struct SphereCollider& other, const glm::vec3& movement) = 0;
        virtual std::optional<Hit> collide(struct AABBCollider&   other, const glm::vec3& movement) = 0;
        virtual std::optional<Hit> collide(struct OBBCollider&    other, const glm::vec3& movement) = 0;
    };
}

#endif //ENGINE_COLLIDER_H
