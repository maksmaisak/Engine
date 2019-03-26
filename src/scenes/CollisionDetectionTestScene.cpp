//
// Created by Maksym Maisak on 2019-03-26.
//

#include "CollisionDetectionTestScene.h"

using namespace en;

void CollisionDetectionTestScene::open() {

    const glm::vec3 halfSize = glm::vec3(10.f);
    setUpNonBodies(halfSize);
    setUpBounds   (halfSize);

    makeSphere({0.2f, 10.f, 0.f}, 1.f).get<Rigidbody>().velocity = {0, 0, 0};
    //makeSphere({0.f , 1.5f, 0.f}, 1.f).get<Rigidbody>().velocity = {0, 0, 0};
    makeCube  ({-1.1f , 1.5f, 0.f}, glm::vec3(1.f)).get<Rigidbody>().velocity = {0, 0, 0};
}

void CollisionDetectionTestScene::update(float dt) {

    if (utils::KeyboardHelper::isDown("r"))
        getEngine().getSceneManager().setCurrentSceneNextUpdate<CollisionDetectionTestScene>();
}
