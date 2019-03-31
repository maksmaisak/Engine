//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef ENGINE_AABBCOLLIDER_H
#define ENGINE_AABBCOLLIDER_H

#include "Collider.h"

namespace en {

    struct AABBCollider : public Collider {

        glm::vec3 center;
        glm::vec3 halfSize;

        AABBCollider(const glm::vec3& halfSize = glm::vec3(1.f));


        void updateTransform(const glm::mat4& transform) override;

        std::optional<Hit> collide(Collider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(SphereCollider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(AABBCollider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(OBBCollider& other, const glm::vec3& movement) override;
    };
}

#endif //ENGINE_AABBCOLLIDER_H
