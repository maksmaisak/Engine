//
// Created by Maksym Maisak on 2019-04-04.
//

#ifndef ENGINE_PHYSICSSYSTEMOCTREE_H
#define ENGINE_PHYSICSSYSTEMOCTREE_H

#include "PhysicsSystemBase.h"
#include <memory>
#include "ShaderProgram.hpp"
#include "ComponentPool.h"
#include "DebugVolumeRenderer.h"
#include "OctreeNode.h"

namespace en {

    class PhysicsSystemOctree : public PhysicsSystemBase {

    public:
        PhysicsSystemOctree();
        void update(float dt) override;
        void draw() override;
        void removeInvalidEntitiesFromTree();
        void updateGridCells(Entity entity, const Rigidbody& rb, const Transform& tf);

    private:
        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt);

        OctreeNode m_octreeRoot;
        ComponentPool<std::pair<glm::vec<3, std::int64_t>, glm::vec<3, std::int64_t>>> m_previousBounds;
        DebugVolumeRenderer m_volumeRenderer;
    };
}

#endif //ENGINE_PHYSICSSYSTEMOCTREE_H
