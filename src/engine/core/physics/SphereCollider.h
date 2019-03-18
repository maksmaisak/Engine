//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef SAXION_Y2Q2_RENDERING_SPHERECOLLIDER_H
#define SAXION_Y2Q2_RENDERING_SPHERECOLLIDER_H

#include "Collider.h"
#include "CollisionDetectionDiscrete.h"

namespace en {

    struct SphereCollider : public Collider {

        float radius = 1.f;
        glm::vec3 position;

        SphereCollider(float radius = 1.f);

        void updateTransform(const glm::mat4& transform) override;

        std::optional<Hit> collide(Collider& other) override;
        std::optional<Hit> collide(SphereCollider& other) override;
        std::optional<Hit> collide(AABBCollider&   other) override;
    };
}

#endif //SAXION_Y2Q2_RENDERING_SPHERECOLLIDER_H
