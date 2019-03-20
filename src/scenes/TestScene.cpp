//
// Created by Maksym Maisak on 27/12/18.
//

#include "TestScene.h"
#include <memory>
#include <cmath>
#include "Engine.h"
#include "Model.h"
#include "Texture.hpp"
#include "Resources.h"
#include "KeyboardHelper.h"
#include "config.hpp"

#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "SphereCollider.h"
#include "AABBCollider.h"

#include "CameraOrbitBehavior.h"
#include "RotatingBehavior.hpp"

#include "Material.h"

namespace {

    void makeFloor(en::Engine& engine, float sideLength, int numSpheresPerSide) {

        const float diameter = 2.f * sideLength / numSpheresPerSide;
        const float radius = diameter * 0.5f;

        auto model = en::Models::get(config::MODEL_PATH + "sphere_smooth.obj");
        auto material = std::make_shared<en::Material>("pbr");
        material->setUniformValue("albedoMap", en::Textures::white());
        material->setUniformValue("metallicSmoothnessMap", en::Textures::white());
        material->setUniformValue("normalMap", en::Textures::defaultNormalMap());
        material->setUniformValue("aoMap", en::Textures::white());
        material->setUniformValue("albedoColor", glm::vec4(1));
        material->setUniformValue("metallicMultiplier", 0.f);
        material->setUniformValue("smoothnessMultiplier", 0.5f);
        material->setUniformValue("aoMultiplier", 1.f);

        auto floor = engine.makeActor("Floor");
        floor.add<en::Transform>().move({0, -6, 0}).scale({100, 1, 100});
        floor.add<en::RenderInfo>(en::Resources<en::Model>::get(config::MODEL_PATH + "cube_flat.obj"), material);
        floor.add<en::Rigidbody>(std::make_unique<en::AABBCollider>(glm::vec3(100.f, 1.f, 100.f))).isKinematic = true;

        for (int y = 0; y < numSpheresPerSide; ++y) {
            for (int x = 0; x < numSpheresPerSide; ++x) {

                en::Actor actor = engine.makeActor("Floor_" + std::to_string(x) + "_" + std::to_string(y));

                auto& tf = actor.add<en::Transform>();

                // from (-1, -1) to (1, 1) inclusive
                glm::vec2 positionNormalized = {
                    ((float) x / (numSpheresPerSide - 1) - 0.5f) * 2.f,
                    ((float) y / (numSpheresPerSide - 1) - 0.5f) * 2.f
                };
                tf.setLocalPosition({
                    sideLength * positionNormalized.x * 0.5f,
                    -10.f + std::sinf(glm::two_pi<float>() * glm::length(positionNormalized)) + 0.6f * glm::length(positionNormalized * glm::vec2(sideLength * 0.5f)),
                    sideLength * positionNormalized.y * 0.5f
                });
                tf.setLocalScale({radius, radius, radius});

                auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::SphereCollider>(radius));
                rb.isKinematic = true;
                //rb.radius = radius;

                actor.add<en::RenderInfo>(model, material).isBatchingStatic = true;
            }
        }
    }

    struct Volume {
        glm::vec3 boundsMin;
        glm::vec3 boundsMax;
        glm::vec<3, int> count;
    };
}

TestScene::TestScene() {

    m_renderSettings.ambientColor = glm::vec3(1.010478, 1.854524, 2.270603) * 0.5f;

    // models
    auto planeModel  = en::Models::get(config::MODEL_PATH + "plane.obj");
    auto cubeModel   = en::Models::get(config::MODEL_PATH + "cube_flat.obj");
    auto sphereModel = en::Models::get(config::MODEL_PATH + "sphere_smooth.obj");

    // materials
    auto sphereMaterial = std::make_shared<en::Material>("pbr");
    sphereMaterial->setUniformValue("albedoMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/rust/albedo.png"));
    sphereMaterial->setUniformValue("metallicSmoothnessMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/rust/metallic_smoothness.psd", GL_RGBA));
    sphereMaterial->setUniformValue("normalMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/rust/normal.png", GL_RGBA));
    sphereMaterial->setUniformValue("aoMap", en::Textures::white());
    sphereMaterial->setUniformValue("albedoColor"         , glm::vec4(1));
    sphereMaterial->setUniformValue("metallicMultiplier"  , 1.f);
    sphereMaterial->setUniformValue("smoothnessMultiplier", 1.f);
    sphereMaterial->setUniformValue("aoMultiplier"        , 1.f);

    auto cubeMaterial = std::make_shared<en::Material>("pbr");
    cubeMaterial->setUniformValue("albedoMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/rust/albedo.png"));
    cubeMaterial->setUniformValue("metallicSmoothnessMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/rust/metallic_smoothness.psd", GL_RGBA));
    cubeMaterial->setUniformValue("normalMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/rust/normal.png", GL_RGBA));
    cubeMaterial->setUniformValue("aoMap", en::Textures::white());
    cubeMaterial->setUniformValue("albedoColor"         , glm::vec4(1));
    cubeMaterial->setUniformValue("metallicMultiplier"  , 1.f);
    cubeMaterial->setUniformValue("smoothnessMultiplier", 1.f);
    cubeMaterial->setUniformValue("aoMultiplier"        , 1.f);

    auto planeMaterial = std::make_shared<en::Material>("pbr");
    planeMaterial->setUniformValue("albedoMap", en::Textures::white());
    planeMaterial->setUniformValue("metallicSmoothnessMap", en::Textures::white());
    planeMaterial->setUniformValue("normalMap", en::Textures::defaultNormalMap());
    planeMaterial->setUniformValue("aoMap", en::Textures::white());
    planeMaterial->setUniformValue("albedoColor", glm::vec4(1));
    planeMaterial->setUniformValue("metallicMultiplier", 0.f);
    planeMaterial->setUniformValue("smoothnessMultiplier", 0.5f);
    planeMaterial->setUniformValue("aoMultiplier", 1.f);

    m_sphereRenderInfo = {sphereModel, sphereMaterial};
    m_cubeRenderInfo   = {cubeModel  , cubeMaterial  };
    m_floorRenderInfo  = {planeModel , planeMaterial };
}

void TestScene::open() {

    en::Engine& engine = getEngine();

    setUpNonBodies();

    makeFloor(engine, 30, 20);

    for (int x = -3; x <= 3; x += 1)
        for (int z = -3; z <= 3; z += 1)
            makeCube(glm::vec3(x, 0.f, z));

    for (int x = -2; x <= 2; x += 1)
        for (int z = -2; z <= 2; z += 1)
            makeSphere(glm::vec3(x, 5.f, z));
}

void TestScene::setUpNonBodies() {

    en::Engine& engine = getEngine();

    en::Actor camera = engine.makeActor("Camera");
    camera.add<en::Camera>();
    camera.add<en::Transform>().move({0,0,10}).rotate(90.f, {0,1,0});

    // TODO have behaviors work when added via registry too.
    auto& cameraOrbitBehavior = camera.add<CameraOrbitBehavior>(10.f, -15.f, 60.f);
    auto center = engine.makeActor();
    center.add<en::Transform>();
    cameraOrbitBehavior.setTarget(center);

    en::Actor directionalLight = engine.makeActor("DirectionalLight");
    directionalLight.add<en::Transform>().rotate(45.f, {1, 0, 0});
    directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
}

void TestScene::update(float dt) {

    if (utils::KeyboardHelper::isDown("r"))
        getEngine().getSceneManager().setCurrentSceneNextUpdate<TestScene>();
}

void TestScene::makeSphere(const glm::vec3& position) {

    en::Actor actor = getEngine().makeActor();
    actor.add<en::Transform>().move(position).scale(glm::vec3(0.2f));
    actor.add<en::RenderInfo>(m_sphereRenderInfo);

    auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::SphereCollider>(0.2f));
    //rb.bounciness = 0.5f;
}

void TestScene::makeCube(const glm::vec3& position) {

    en::Actor actor = getEngine().makeActor();
    actor.add<en::Transform>().move(position).scale(glm::vec3(0.4f));
    actor.add<en::RenderInfo>(m_cubeRenderInfo);

    auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::AABBCollider>(glm::vec3(0.4f)));
    //rb.bounciness = 0.5f;
}
