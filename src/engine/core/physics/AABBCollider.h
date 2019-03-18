//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef SAXION_Y2Q2_RENDERING_AABBCOLLIDER_H
#define SAXION_Y2Q2_RENDERING_AABBCOLLIDER_H

#include "Collider.h"

namespace en {

    struct AABBCollider : public Collider {

        glm::vec3 halfSize;
        glm::vec3 center;

        AABBCollider(const glm::vec3& halfSize = glm::vec3(1.f));

        void updateTransform(const glm::mat4& transform) override;

        std::optional<Hit> collide(Collider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(SphereCollider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(AABBCollider& other, const glm::vec3& movement) override;
    };
}

#endif //SAXION_Y2Q2_RENDERING_AABBCOLLIDER_H
