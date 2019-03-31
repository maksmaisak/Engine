//
// Created by Maksym Maisak on 2019-03-26.
//

#ifndef ENGINE_COLLISIONDETECTIONTESTSCENE_H
#define ENGINE_COLLISIONDETECTIONTESTSCENE_H

#include "PhysicsTestBodyGenerator.h"

namespace en {

    class CollisionDetectionTestScene : public Scene {

    public:
        CollisionDetectionTestScene();

        void open() override;
        void update(float dt) override;

    private:
        PhysicsTestBodyGenerator m_bodyGenerator;
    };
}

#endif //ENGINE_COLLISIONDETECTIONTESTSCENE_H
