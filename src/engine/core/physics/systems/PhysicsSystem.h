//
// Created by Maksym Maisak on 20/10/18.
//

#ifndef SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H
#define SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H

#include "PhysicsSystemBase.h"

namespace en {

    class PhysicsSystem : public PhysicsSystemBase {

    public:
        void update(float dt) override;
    };
}

#endif //SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H
