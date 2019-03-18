//
// Created by Maksym Maisak on 4/11/18.
//

#ifndef SAXION_Y2Q1_CPP_DESTROYSYSTEM_H
#define SAXION_Y2Q1_CPP_DESTROYSYSTEM_H

#include "Engine.h"
#include "System.h"

namespace en {

    class DestroySystem : public System {

    public:
        void update(float dt) override;
    };
}

#endif //SAXION_Y2Q1_CPP_DESTROYSYSTEM_H
