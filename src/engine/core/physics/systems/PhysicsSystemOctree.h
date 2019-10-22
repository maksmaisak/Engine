//
// Created by Maksym Maisak on 2019-04-04.
//

#ifndef ENGINE_PHYSICSSYSTEMOCTREE_H
#define ENGINE_PHYSICSSYSTEMOCTREE_H

#include "PhysicsSystemBase.h"
#include <memory>
#include "ShaderProgram.h"
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
        void updateTree(Entity entity, const Rigidbody& rb, const Transform& tf);

    private:
        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt);

        OctreeNode m_octreeRoot;
        ComponentPool<Bounds3D> m_previousBounds;
        DebugVolumeRenderer m_volumeRenderer;
    };
}

#endif //ENGINE_PHYSICSSYSTEMOCTREE_H
