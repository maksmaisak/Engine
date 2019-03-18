//
// Created by Maksym Maisak on 2019-03-04.
//

#ifndef SAXION_Y2Q2_RENDERING_TWEENSYSTEM_H
#define SAXION_Y2Q2_RENDERING_TWEENSYSTEM_H

#include "System.h"

namespace en {

    class TweenSystem : public System {

    public:
        void update(float dt) override;
    };
}

#endif //SAXION_Y2Q2_RENDERING_TWEENSYSTEM_H
