//
// Created by Maksym Maisak on 2019-03-26.
//

#ifndef ENGINE_OBBCOLLIDER_H
#define ENGINE_OBBCOLLIDER_H

#include "Collider.h"

namespace en {

    struct OBBCollider : public Collider {

        glm::vec3 center;
        glm::vec3 halfSize;
        glm::mat3 rotation;

        OBBCollider(const glm::vec3& halfSize);

        void updateTransform(const glm::mat4& transform) override;
        utils::Bounds getBounds() override;
        utils::BoundingSphere getBoundingSphere() override;

        std::optional<Hit> collide(Collider& other, const glm::vec3& movement) override;

        std::optional<Hit> collide(SphereCollider& other, const glm::vec3& movement) override;
        std::optional<Hit> collide(AABBCollider&   other, const glm::vec3& movement) override;
        std::optional<Hit> collide(OBBCollider&    other, const glm::vec3& movement) override;
    };
}


#endif //ENGINE_OBBCOLLIDER_H
