//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef ENGINE_SPHERECOLLIDER_H
#define ENGINE_SPHERECOLLIDER_H

#include "Collider.h"
#include "CollisionDetection.h"

namespace en {

    struct SphereCollider : public Collider {

        float radius = 1.f;
        glm::vec3 position;

        SphereCollider(float radius = 1.f);

        void updateTransform(const glm::mat4& transform) override;
        Bounds3D getBounds() override;
        BoundingSphere getBoundingSphere() override;

        std::optional<Hit> collide(Collider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(SphereCollider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(AABBCollider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(OBBCollider& other, const glm::vec3& movement) override;
    };
}

#endif //ENGINE_SPHERECOLLIDER_H
