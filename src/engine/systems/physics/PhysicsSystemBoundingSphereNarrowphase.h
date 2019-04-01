//
// Created by Maksym Maisak on 2019-04-01.
//

#ifndef ENGINE_PHYSICSSYSTEMBOUNDINGSPHERENARROWPHASE_H
#define ENGINE_PHYSICSSYSTEMBOUNDINGSPHERENARROWPHASE_H

#include "PhysicsSystemBase.h"

namespace en {

    class PhysicsSystemBoundingSphereNarrowphase : public PhysicsSystemBase {

    public:
        void update(float dt) override;

    private:
        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities);
    };
}

#endif //ENGINE_PHYSICSSYSTEMBOUNDINGSPHERENARROWPHASE_H
