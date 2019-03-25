//
// Created by Maksym Maisak on 27/12/18.
//

#include "PhysicsTestScene.h"
#include <memory>
#include <cmath>
#include <algorithm>
#include <array>
#include <utility>
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
    m_cubeModel(en::Models::get(config::MODEL_PATH + "cube_flat.obj")),
    m_sphereModel(en::Models::get(config::MODEL_PATH + "sphere_smooth.obj")),
    m_randomPosition(makeRandomVectorGeneratorCenterHalfSize({0, preset.fieldHalfSize.y, 0}, preset.fieldHalfSize))
{
    m_renderSettings.ambientColor = glm::vec3(1.010478, 1.854524, 2.270603) * 0.5f;
    cacheMaterials();
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
    directionalLight.add<en::Transform>().rotate(45.f, {1, 0, 0}).rotate(20.f, {0, 1, 0});
    directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
}

void PhysicsTestScene::setUpBounds() {

    const glm::vec3 center   = {0, m_preset.fieldHalfSize.y, 0};
    const glm::vec3 halfSize = m_preset.fieldHalfSize;
    const auto make = [&engine = getEngine(), &model = m_cubeModel, &material = m_floorMaterial](const glm::vec3& center, const glm::vec3& halfSize, bool isVisible = true) {

        en::Actor actor = engine.makeActor("Wall");

        actor.add<en::Transform>().move(center).scale(halfSize);
        actor.add<en::Rigidbody>(std::make_unique<en::AABBCollider>(halfSize)).isKinematic = true;

        if (isVisible)
            actor.add<en::RenderInfo>(model, material).isBatchingStatic = true;

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
    const auto randomBool = [&e = m_randomEngine]() {
        return std::uniform_int_distribution(0, 1)(e) == 1;
    };

    for (int i = 0; i < m_preset.numBodiesStatic; ++i) {
        if (randomBool())
            makeCube(m_randomPosition(), randomHalfSize(), true);
        else
            makeSphere(m_randomPosition(), randomRadius(), true);
    }
}

void PhysicsTestScene::addDynamicBodies() {

    const auto randomHalfSize = makeRandomVectorGenerator(glm::vec3(0.5f), glm::vec3(2.f));
    const auto randomRadius = [&e = m_randomEngine](){return std::uniform_real_distribution(0.5f, 2.f)(e);};
    const auto randomBool = [&e = m_randomEngine]() {
        return std::uniform_int_distribution(0, 1)(e) == 1;
    };

    for (int i = 0; i < m_preset.numBodiesDynamic; ++i) {
        if (randomBool())
            makeCube(m_randomPosition(), randomHalfSize());
        else
            makeSphere(m_randomPosition(), randomRadius());
    }
}

void PhysicsTestScene::makeSphere(const glm::vec3& position, float radius, bool isStatic) {

    en::Actor actor = getEngine().makeActor();
    actor.add<en::Transform>().move(position).scale(glm::vec3(radius));

    std::uniform_int_distribution<std::size_t> d(0, m_materials.size() - 1);
    actor.add<en::RenderInfo>(
        m_sphereModel,
        isStatic ? m_staticBodyMaterial : m_materials.at(d(m_randomEngine))
    ).isBatchingStatic = isStatic;

    auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::SphereCollider>(radius));
    rb.isKinematic = isStatic;
    if (!rb.isKinematic) {
        std::uniform_real_distribution<float> d(-1.f, 1.f);
        rb.velocity = {d(m_randomEngine), d(m_randomEngine), d(m_randomEngine)};
    }
    //rb.bounciness = 0.5f;
}

void PhysicsTestScene::makeCube(const glm::vec3& position, const glm::vec3& halfSize, bool isStatic) {

    en::Actor actor = getEngine().makeActor();
    actor.add<en::Transform>().move(position).scale(halfSize);

    std::uniform_int_distribution<std::size_t> d(0, m_materials.size() - 1);
    actor.add<en::RenderInfo>(
        m_cubeModel,
        isStatic ? m_staticBodyMaterial : m_materials.at(d(m_randomEngine))
    ).isBatchingStatic = isStatic;

    auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::AABBCollider>(halfSize));
    rb.isKinematic = isStatic;
    if (!rb.isKinematic) {
        std::uniform_real_distribution<float> d(-10.f, 10.f);
        rb.velocity = {d(m_randomEngine), d(m_randomEngine), d(m_randomEngine)};
    }
    //rb.bounciness = 0.5f;
}

void PhysicsTestScene::cacheMaterials() {

    static const std::array<glm::vec3, 6> colors = {
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(1, 1, 0),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1)
    };

    static auto makeMaterial = [](const glm::vec3& color) {

        auto material = std::make_shared<en::Material>("pbr");
        material->setUniformValue("albedoMap"            , en::Textures::white());
        material->setUniformValue("metallicSmoothnessMap", en::Textures::white());
        material->setUniformValue("normalMap"            , en::Textures::defaultNormalMap());
        material->setUniformValue("aoMap"                , en::Textures::white());
        material->setUniformValue("albedoColor", glm::vec4(color, 1.f));
        material->setUniformValue("metallicMultiplier"  , 0.f);
        material->setUniformValue("smoothnessMultiplier", 0.5f);
        material->setUniformValue("aoMultiplier", 1.f);

        return material;
    };

    std::transform(colors.begin(), colors.end(), std::back_inserter(m_materials), makeMaterial);
    m_staticBodyMaterial = makeMaterial(glm::vec3(0.5f));
    m_floorMaterial      = makeMaterial(glm::vec3(1.0f));
}
