//
// Created by Maksym Maisak on 2019-03-31.
//

#include "PhysicsTestBodyGenerator.h"
#include <cmath>
#include <cassert>
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
#include "OBBCollider.h"

#include "CameraOrbitBehavior.h"
#include "RotatingBehavior.hpp"

#include "Material.h"

using namespace en;

PhysicsTestBodyGenerator::PhysicsTestBodyGenerator(const glm::vec3& halfSize, const std::default_random_engine& randomEngine) :
    m_halfSize    (halfSize),
    m_randomEngine(randomEngine),
    m_cubeModel  (Models::get(config::MODEL_PATH + "cube_flat.obj"    )),
    m_sphereModel(Models::get(config::MODEL_PATH + "sphere_smooth.obj"))
{
    cacheMaterials();
}

void PhysicsTestBodyGenerator::setUpNonBodies() {

    assert(m_engine);

    if (Scene* scene = m_engine->getSceneManager().getCurrentScene()) {
        scene->getRenderSettings().ambientColor = glm::vec3(0.505239, 0.927262, 1.1353015);
    }

    en::Actor camera = m_engine->makeActor("Camera");
    camera.add<Camera>();
    camera.add<en::Transform>()
        .move({0, m_halfSize.y, 10})
        .rotate(90.f, {0,1,0});

    // TODO have behaviors work when added via registry too.
    auto& cameraOrbitBehavior = camera.add<CameraOrbitBehavior>(glm::length(m_halfSize) * 1.2f, -80.f, 80.f);
    auto center = m_engine->makeActor();
    center.add<en::Transform>().move({0, m_halfSize.y, 0});
    cameraOrbitBehavior.setTarget(center);

    en::Actor directionalLight = m_engine->makeActor("DirectionalLight");
    directionalLight.add<en::Transform>().rotate(45.f, {1, 0, 0}).rotate(20.f, {0, 1, 0});
    directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
}

void PhysicsTestBodyGenerator::setUpBounds() {

    assert(m_engine);

    const glm::vec3 center = {0, m_halfSize.y, 0};
    const auto make = [&engine = *m_engine, &model = m_cubeModel, &material = m_floorMaterial](const glm::vec3& center, const glm::vec3& halfSize, bool isVisible = true) {

        en::Actor actor = engine.makeActor("Wall");

        actor.add<en::Transform>().move(center).scale(halfSize);
        actor.add<en::Rigidbody>(std::make_unique<en::AABBCollider>(halfSize)).isKinematic = true;

        if (isVisible)
            actor.add<en::RenderInfo>(model, material).isBatchingStatic = true;

        return actor;
    };

    make(center + glm::vec3(0, -m_halfSize.y, 0), {m_halfSize.x, 1, m_halfSize.z});
    make(center + glm::vec3(0, +m_halfSize.y, 0), {m_halfSize.x, 1, m_halfSize.z}, false);

    make(center + glm::vec3(-m_halfSize.x, 0, 0), {1, m_halfSize.y, m_halfSize.z}, false);
    make(center + glm::vec3(+m_halfSize.x, 0, 0), {1, m_halfSize.y, m_halfSize.z}, false);

    make(center + glm::vec3(0, 0, -m_halfSize.z), {m_halfSize.x, m_halfSize.y, 1}, false);
    make(center + glm::vec3(0, 0, +m_halfSize.z), {m_halfSize.x, m_halfSize.y, 1}, false);
}

Actor PhysicsTestBodyGenerator::makeSphere(const glm::vec3& position, float radius, bool isStatic) {

    assert(m_engine);

    Actor actor = m_engine->makeActor();
    actor.add<Transform>().move(position).scale(glm::vec3(radius));

    actor.add<RenderInfo>(
        m_sphereModel,
        isStatic ? m_staticBodyMaterial : getRandomBodyMaterial()
    ).isBatchingStatic = isStatic;

    auto& rb = actor.add<Rigidbody>(std::make_unique<SphereCollider>(radius));
    rb.invMass = 1.f / (4.f / 3.f * glm::pi<float>() * radius * radius * radius);
    rb.isKinematic = isStatic;
    if (!rb.isKinematic) {
        std::uniform_real_distribution<float> d(-1.f, 1.f);
        rb.velocity = {d(m_randomEngine), d(m_randomEngine), d(m_randomEngine)};
    }
    //rb.bounciness = 0.5f;

    return actor;
}

Actor PhysicsTestBodyGenerator::makeAABB(const glm::vec3& position, const glm::vec3& halfSize, bool isStatic) {

    assert(m_engine);

    Actor actor = m_engine->makeActor();
    actor.add<Transform>().move(position).scale(halfSize);

    actor.add<RenderInfo>(
        m_cubeModel,
        isStatic ? m_staticBodyMaterial : getRandomBodyMaterial()
    ).isBatchingStatic = isStatic;

    auto& rb = actor.add<Rigidbody>(std::make_unique<AABBCollider>(halfSize));
    rb.invMass = 1.f / (8.f * halfSize.x * halfSize.y * halfSize.z);
    rb.isKinematic = isStatic;
    if (!rb.isKinematic) {
        std::uniform_real_distribution<float> d(-10.f, 10.f);
        rb.velocity = {d(m_randomEngine), d(m_randomEngine), d(m_randomEngine)};
    }
    //rb.bounciness = 0.5f;

    return actor;
}

Actor PhysicsTestBodyGenerator::makeCube(const glm::vec3& position, const glm::vec3& halfSize, bool isStatic) {

    assert(m_engine);

    Actor actor = m_engine->makeActor();
    actor.add<Transform>().move(position).scale(halfSize);

    actor.add<RenderInfo>(
        m_cubeModel,
        isStatic ? m_staticBodyMaterial : getRandomBodyMaterial()
    ).isBatchingStatic = isStatic;

    auto& rb = actor.add<Rigidbody>(std::make_unique<OBBCollider>(halfSize));
    rb.invMass = 1.f / (8.f * halfSize.x * halfSize.y * halfSize.z);
    rb.isKinematic = isStatic;
    if (!rb.isKinematic) {
        std::uniform_real_distribution<float> d(-10.f, 10.f);
        rb.velocity = {d(m_randomEngine), d(m_randomEngine), d(m_randomEngine)};
    }
    //rb.bounciness = 0.5f;

    return actor;
}

void PhysicsTestBodyGenerator::cacheMaterials() {

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

glm::vec3 PhysicsTestBodyGenerator::getRandomVectorMinMax(const glm::vec3& min, const glm::vec3& max) {

    std::uniform_real_distribution<float> x(min.x, max.x);
    std::uniform_real_distribution<float> y(min.y, max.y);
    std::uniform_real_distribution<float> z(min.z, max.z);
    return {x(m_randomEngine), y(m_randomEngine), z(m_randomEngine)};
}

glm::vec3 PhysicsTestBodyGenerator::getRandomVectorCenterHalfSize(const glm::vec3& center, const glm::vec3& halfSize) {

    return getRandomVectorMinMax(center - halfSize, center + halfSize);
}

std::shared_ptr<Material> PhysicsTestBodyGenerator::getRandomBodyMaterial() {

    std::uniform_int_distribution<std::size_t> d(0, m_materials.size() - 1);
    return m_materials[d(m_randomEngine)];
}

void PhysicsTestBodyGenerator::setEngine(Engine& engine) {

    m_engine = &engine;
}

std::default_random_engine& PhysicsTestBodyGenerator::getRandomEngine() {

    return m_randomEngine;
}
