//
// Created by Maksym Maisak on 2019-04-21.
//

#ifndef ENGINE_PHYSICSSYSTEMQUADTREE_H
#define ENGINE_PHYSICSSYSTEMQUADTREE_H

#include "PhysicsSystemBase.h"
#include <memory>
#include "ShaderProgram.h"
#include "ComponentPool.h"
#include "DebugVolumeRenderer.h"
#include "QuadtreeNode.h"

namespace en {

    class PhysicsSystemQuadtree : public PhysicsSystemBase {

    public:
        PhysicsSystemQuadtree();
        void update(float dt) override;
        void draw() override;
        void removeInvalidEntitiesFromTree();
        void updateTree(Entity entity, const Rigidbody& rb, const Transform& tf);

    private:
        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt);

        QuadtreeNode m_quadtreeRoot;
        ComponentPool<Bounds2D> m_previousBounds;
        DebugVolumeRenderer m_volumeRenderer;
    };
}



#endif //ENGINE_PHYSICSSYSTEMQUADTREE_H
