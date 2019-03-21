//
// Created by Maksym Maisak on 27/12/18.
//

#include "PhysicsTestScene.h"
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

    void makeFloorFromSpheres(en::Engine& engine, float sideLength, int numSpheresPerSide) {

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

    std::function<glm::vec3()> makeRandomVectorGenerator(const glm::vec3& min, const glm::vec3& max) {

        std::uniform_real_distribution<float> x(min.x, max.x);
        std::uniform_real_distribution<float> y(min.y, max.y);
        std::uniform_real_distribution<float> z(min.z, max.z);

        // `mutable` because the call operator of the distributions is non-const, so captured objects mustn't be const
        return [e = std::default_random_engine(0), x, y, z]() mutable -> glm::vec3 {
            return {x(e), y(e), z(e)};
        };
    }

    std::function<glm::vec3()> makeRandomVectorGeneratorCenterHalfSize(const glm::vec3& center, const glm::vec3& halfSize) {

        return makeRandomVectorGenerator(center - halfSize, center + halfSize);
    }
}

PhysicsTestScene::PhysicsTestScene(const Preset& preset) :
    m_preset(preset),
    m_randomPosition(makeRandomVectorGeneratorCenterHalfSize({0, preset.fieldHalfSize.y, 0}, preset.fieldHalfSize))
{
    m_renderSettings.ambientColor = glm::vec3(1.010478, 1.854524, 2.270603) * 0.5f;
    cacheRenderInfos();
}

void PhysicsTestScene::open() {

    en::Engine& engine = getEngine();

    setUpNonBodies();
    setUpBounds();
    addStaticBodies();
    addDynamicBodies();
}

void PhysicsTestScene::update(float dt) {

    if (utils::KeyboardHelper::isDown("r"))
        getEngine().getSceneManager().setCurrentSceneNextUpdate<PhysicsTestScene>();
}

void PhysicsTestScene::setUpNonBodies() {

    en::Engine& engine = getEngine();

    en::Actor camera = engine.makeActor("Camera");
    camera.add<en::Camera>();
    camera.add<en::Transform>()
        .move({0, m_preset.fieldHalfSize.y, 10})
        .rotate(90.f, {0,1,0});

    // TODO have behaviors work when added via registry too.
    auto& cameraOrbitBehavior = camera.add<CameraOrbitBehavior>(glm::length(m_preset.fieldHalfSize) * 1.2f, -80.f, 80.f);
    auto center = engine.makeActor();
    center.add<en::Transform>().move({0, m_preset.fieldHalfSize.y, 0});
    cameraOrbitBehavior.setTarget(center);

    en::Actor directionalLight = engine.makeActor("DirectionalLight");
    directionalLight.add<en::Transform>().rotate(45.f, {1, 0, 0});
    directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
}

void PhysicsTestScene::setUpBounds() {

    const glm::vec3 center   = {0, m_preset.fieldHalfSize.y, 0};
    const glm::vec3 halfSize = m_preset.fieldHalfSize;
    const auto make = [&engine = getEngine(), renderInfo = m_floorRenderInfo](const glm::vec3& center, const glm::vec3& halfSize, bool isVisible = true) {

        auto actor = engine.makeActor();

        actor.add<en::Transform>().move(center).scale(halfSize);
        actor.add<en::Rigidbody>(std::make_unique<en::AABBCollider>(halfSize)).isKinematic = true;

        if (isVisible)
            actor.add<en::RenderInfo>(renderInfo).isBatchingStatic = true;

        return actor;
    };

    make(center + glm::vec3(0, -halfSize.y, 0), {halfSize.x, 1, halfSize.z});
    make(center + glm::vec3(0, +halfSize.y, 0), {halfSize.x, 1, halfSize.z}, false);

    make(center + glm::vec3(-halfSize.x, 0, 0), {1, halfSize.y, halfSize.z}, false);
    make(center + glm::vec3(+halfSize.x, 0, 0), {1, halfSize.y, halfSize.z}, false);

    make(center + glm::vec3(0, 0, -halfSize.z), {halfSize.x, halfSize.y, 1}, false);
    make(center + glm::vec3(0, 0, +halfSize.z), {halfSize.x, halfSize.y, 1}, false);
}

void PhysicsTestScene::addStaticBodies() {

    const auto randomHalfSize = makeRandomVectorGenerator(glm::vec3(0.5f), glm::vec3(2.f));
    const auto randomRadius = [&e = m_randomEngine](){return std::uniform_real_distribution(0.5f, 2.f)(e);};

    for (int i = 0; i < m_preset.numBodiesStatic; ++i) {
        if (m_randomEngine() % 2)
            makeCube(m_randomPosition(), randomHalfSize(), true);
        else
            makeSphere(m_randomPosition(), randomRadius(), true);
    }
}

void PhysicsTestScene::addDynamicBodies() {

    for (int x = -3; x <= 3; x += 1)
        for (int z = -3; z <= 3; z += 1)
            makeCube(glm::vec3(x, 5.f, z));

    for (int x = -2; x <= 2; x += 1)
        for (int z = -2; z <= 2; z += 1)
            makeSphere(glm::vec3(x, 10.f, z));
}

void PhysicsTestScene::makeSphere(const glm::vec3& position, float radius, bool isStatic) {

    en::Actor actor = getEngine().makeActor();
    actor.add<en::Transform>().move(position).scale(glm::vec3(radius));
    actor.add<en::RenderInfo>(m_sphereRenderInfo).isBatchingStatic = isStatic;

    auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::SphereCollider>(radius)).isKinematic = isStatic;
    //rb.bounciness = 0.5f;
}

void PhysicsTestScene::makeCube(const glm::vec3& position, const glm::vec3& halfSize, bool isStatic) {

    en::Actor actor = getEngine().makeActor();
    actor.add<en::Transform>().move(position).scale(halfSize);
    actor.add<en::RenderInfo>(m_cubeRenderInfo).isBatchingStatic = isStatic;

    auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::AABBCollider>(halfSize)).isKinematic = isStatic;
    //rb.bounciness = 0.5f;
}

void PhysicsTestScene::cacheRenderInfos() {

    // models
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
    m_floorRenderInfo  = {cubeModel  , planeMaterial };
}
