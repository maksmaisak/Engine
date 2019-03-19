//
// Created by Maksym Maisak on 17/10/18.
//

#ifndef SAXION_Y2Q1_CPP_RIGIDBODY_H
#define SAXION_Y2Q1_CPP_RIGIDBODY_H

#include <memory>
#include "glm.hpp"
#include "ComponentsToLua.h"
#include "Collider.h"

namespace en {

    struct Rigidbody {

        LUA_TYPE(Rigidbody)
        static void initializeMetatable(LuaState& lua);

        std::unique_ptr<Collider> collider;

        bool isKinematic = false;
        bool useGravity  = true;
        glm::vec3 velocity;
        float invMass    = 1.f;
        float bounciness = 0.98f;
    };
}

#endif //SAXION_Y2Q1_CPP_RIGIDBODY_H
