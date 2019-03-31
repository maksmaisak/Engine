//
// Created by Maksym Maisak on 2019-03-26.
//

#include "CollisionDetectionTestScene.h"
#include "Camera.h"

using namespace en;

CollisionDetectionTestScene::CollisionDetectionTestScene() : m_bodyGenerator(glm::vec3(16.f)) {}

void CollisionDetectionTestScene::open() {

    m_bodyGenerator.setEngine(getEngine());
    m_bodyGenerator.setUpNonBodies();
    m_bodyGenerator.setUpBounds   ();

    Actor camera = getEngine().findByName("Camera");
    if (camera)
        camera.get<Transform>().setLocalPosition({-16, 16, 0});

    m_bodyGenerator.makeSphere({0.2f, 10.f, -3.f}).get<Rigidbody>().velocity = {0, 0, 0};
    {
        auto& rb = m_bodyGenerator.makeAABB({-1.1f, 2.f, -3.f}).get<Rigidbody>();
        rb.velocity = {0, 0, 0};
    }

    m_bodyGenerator.makeSphere({0.2f, 10.f, 0.f}).get<Rigidbody>().velocity = {0, 0, 0};
    {
        Actor cube = m_bodyGenerator.makeCube({-1.1f, 2.f, 0.f});
        cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
        auto& rb = cube.get<Rigidbody>();
        rb.velocity = {0, 0, 0};
    }

    {
        Actor cube = m_bodyGenerator.makeCube({0.2f, 10.f, 3.f});
        cube.get<Rigidbody>().velocity = {0, 0, 0};
        cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    }
    m_bodyGenerator.makeSphere({-1.1f,  2.f, 3.f}).get<Rigidbody>().velocity = {0, 0, 0};

    {
        Actor aabb = m_bodyGenerator.makeAABB({0.2f, 10.f, 6.f});
        aabb.get<Rigidbody>().velocity = {0, 0, 0};
        //cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    }
    {
        Actor cube = m_bodyGenerator.makeCube({-1.1f,  2.f, 6.f});
        cube.get<Rigidbody>().velocity = {0, 0, 0};
        cube.get<Transform>().rotate(glm::radians(20.f), {0, 0, 1});
    }

    {
        Actor cube = m_bodyGenerator.makeCube({0.f, 10.f, 9.f});
        cube.get<Rigidbody>().velocity = {0, 0, 10.f};
        cube.get<Transform>()
            .rotate(glm::radians(-30.f), {0, 0, 1})
            .rotate(glm::radians( 90.f), {0, 1, 0});
    }
    {
        Actor cube = m_bodyGenerator.makeCube({0.f, 4.f, 9.f});
        cube.get<Rigidbody>().velocity = {0, 0, 0};
        cube.get<Transform>()
            .rotate(glm::radians(45.f), {0, 0, 1})
            .rotate(glm::radians(45.f), {1, 0, 0});
    }
}

void CollisionDetectionTestScene::update(float dt) {

    if (utils::KeyboardHelper::isDown("r"))
        getEngine().getSceneManager().setCurrentSceneNextUpdate<CollisionDetectionTestScene>();
}
