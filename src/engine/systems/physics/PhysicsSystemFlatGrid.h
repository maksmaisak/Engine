//
// Created by Maksym Maisak on 2019-04-01.
//

#ifndef ENGINE_PHYSICSSYSTEMFLATGRID_H
#define ENGINE_PHYSICSSYSTEMFLATGRID_H

#include "PhysicsSystemBase.h"
#include "ComponentPool.h"

namespace en {

    class PhysicsSystemFlatGrid : public PhysicsSystemBase {

    public:
        PhysicsSystemFlatGrid();
        void update(float dt) override;
        void updateGrid(Entity entity, const Rigidbody& rb, const Transform& tf);

    private:
        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt);

        std::vector<std::vector<Entity>> m_grid;
        ComponentPool<std::pair<glm::vec<3, std::size_t>, glm::vec<3, std::size_t>>> m_previousBounds;
    };
}

#endif //ENGINE_PHYSICSSYSTEMFLATGRID_H
