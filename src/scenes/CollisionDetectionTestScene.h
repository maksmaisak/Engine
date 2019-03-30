//
// Created by Maksym Maisak on 2019-03-26.
//

#ifndef ENGINE_COLLISIONDETECTIONTESTSCENE_H
#define ENGINE_COLLISIONDETECTIONTESTSCENE_H

#include "PhysicsTestSceneBase.h"

namespace en {

    class CollisionDetectionTestScene : public PhysicsTestSceneBase {

    public:
        void open() override;
        void update(float dt) override;
    };
}

#endif //ENGINE_COLLISIONDETECTIONTESTSCENE_H
