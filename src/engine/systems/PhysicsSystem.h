//
// Created by Maksym Maisak on 20/10/18.
//

#ifndef SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H
#define SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H

#include <tuple>
#include "System.h"
#include "Engine.h"
#include "glm.hpp"
#include "Collision.h"

namespace en {

    class Transform;
    struct Rigidbody;
    struct Collision;

    class PhysicsSystem : public en::System {

    public:
        void update(float dt) override;
        PhysicsSystem& setGravity(const glm::vec3& gravity);

    private:

        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities);
        void addGravity(Entity entity, Transform& tf, Rigidbody& rb, float dt);

        glm::vec3 m_gravity;
        std::vector<Collision> m_detectedCollisions;
    };
}

#endif //SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H
