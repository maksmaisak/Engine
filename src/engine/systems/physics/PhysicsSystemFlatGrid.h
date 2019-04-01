//
// Created by Maksym Maisak on 2019-04-01.
//

#ifndef ENGINE_PHYSICSSYSTEMFLATGRID_H
#define ENGINE_PHYSICSSYSTEMFLATGRID_H

#include "PhysicsSystemBase.h"

namespace en {

    class PhysicsSystemFlatGrid : public PhysicsSystemBase {

    public:
        void update(float dt) override;
    };
}

#endif //ENGINE_PHYSICSSYSTEMFLATGRID_H
