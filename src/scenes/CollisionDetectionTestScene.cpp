//
// Created by Maksym Maisak on 2019-03-26.
//

#include "CollisionDetectionTestScene.h"

using namespace en;

void CollisionDetectionTestScene::open() {

    const glm::vec3 halfSize = glm::vec3(15.f);
    setUpNonBodies(halfSize);
    setUpBounds   (halfSize);

    //makeSphere({ 0.2f, 10.f, -3.f}).get<Rigidbody>().velocity = {0, 0, 0};
    //{
    //    auto& rb = makeAABB({-1.1f, 2.f, -3.f}).get<Rigidbody>();
    //    rb.velocity = {0, 0, 0};
    //    rb.useGravity = false;
    //}
//
    //makeSphere({0.2f, 10.f, 0.f}).get<Rigidbody>().velocity = {0, 0, 0};
    //{
    //    Actor cube = makeCube({-1.1f, 2.f, 0.f});
    //    cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    //    auto& rb = cube.get<Rigidbody>();
    //    rb.velocity = {0, 0, 0};
    //    rb.useGravity = false;
    //}
//
    //{
    //    Actor cube = makeCube({0.2f, 10.f, 3.f});
    //    cube.get<Rigidbody>().velocity = {0, 0, 0};
    //    cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    //}
    //makeSphere({-1.1f,  2.f, 3.f}).get<Rigidbody>().velocity = {0, 0, 0};

    {
        Actor cube = makeCube({ 0.2f, 10.f, 6.f});
        cube.get<Rigidbody>().velocity = {0, 0, 0};
        //cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    }
    {
        Actor cube = makeCube({-1.1f,  2.f, 6.f});
        cube.get<Rigidbody>().velocity = {0, 0, 0};
        cube.get<Rigidbody>().useGravity = false;
        cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    }

    {
        Actor cube = makeCube({ 0.2f, 10.f, 9.f});
        cube.get<Rigidbody>().velocity = {0, 0, 0};
        //cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    }
    {
        Actor cube = makeCube({-1.1f,  2.f, 9.f});
        cube.get<Rigidbody>().velocity = {0, 0, 0};
        cube.get<Rigidbody>().useGravity = false;
        cube.get<Transform>()
            .rotate(glm::radians(45.f), {0, 0, 1})
            .rotate(glm::radians(45.f), {1, 0, 0});
    }
}

void CollisionDetectionTestScene::update(float dt) {

    if (utils::KeyboardHelper::isDown("r"))
        getEngine().getSceneManager().setCurrentSceneNextUpdate<CollisionDetectionTestScene>();
}
