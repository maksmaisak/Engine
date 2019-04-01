//
// Created by Maksym Maisak on 2019-04-01.
//

#ifndef ENGINE_PHYSICSSYSTEMPRECHECKCOLLISIONS_H
#define ENGINE_PHYSICSSYSTEMPRECHECKCOLLISIONS_H

#include "PhysicsSystemBase.h"

namespace en {

    class PhysicsSystemPreCheckCollisions : public PhysicsSystemBase {

    public:
        void update(float dt) override;

    private:
        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities);
    };
}

#endif //ENGINE_PHYSICSSYSTEMPRECHECKCOLLISIONS_H
