//
// Created by Maksym Maisak on 25/10/18.
//

#ifndef SAXION_Y2Q1_CPP_DESTROYBYTIMERSYSTEM_H
#define SAXION_Y2Q1_CPP_DESTROYBYTIMERSYSTEM_H

#include <vector>
#include "System.h"
#include "Entity.h"
#include "DestroyTimer.h"

namespace en {

    /// Destroys entities with a DestroyTimer when the time runs out
    class DestroyByTimerSystem : public System {

    public:
        void update(float dt) override;

    private:
        std::vector<en::Entity> m_entitiesToDestroy;
    };
}

#endif //SAXION_Y2Q1_CPP_DESTROYBYTIMERSYSTEM_H
