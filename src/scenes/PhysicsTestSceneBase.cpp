//
// Created by Maksym Maisak on 2019-03-26.
//

#include "PhysicsTestSceneBase.h"
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

using namespace en;

PhysicsTestSceneBase::PhysicsTestSceneBase() :
    m_cubeModel  (Models::get(config::MODEL_PATH + "cube_flat.obj"    )),
    m_sphereModel(Models::get(config::MODEL_PATH + "sphere_smooth.obj"))
{
    m_renderSettings.ambientColor = glm::vec3(1.010478, 1.854524, 2.270603) * 0.5f;
    cacheMaterials();
}

void PhysicsTestSceneBase::setUpNonBodies(const glm::vec3& halfSize) {

    en::Engine& engine = getEngine();

    en::Actor camera = engine.makeActor("Camera");
    camera.add<Camera>();
    camera.add<en::Transform>()
        .move({0, halfSize.y, 10})
        .rotate(90.f, {0,1,0});

    // TODO have behaviors work when added via registry too.
    auto& cameraOrbitBehavior = camera.add<CameraOrbitBehavior>(glm::length(halfSize) * 1.2f, -80.f, 80.f);
    auto center = engine.makeActor();
    center.add<en::Transform>().move({0, halfSize.y, 0});
    cameraOrbitBehavior.setTarget(center);

    en::Actor directionalLight = engine.makeActor("DirectionalLight");
    directionalLight.add<en::Transform>().rotate(45.f, {1, 0, 0}).rotate(20.f, {0, 1, 0});
    directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
}

void PhysicsTestSceneBase::setUpBounds(const glm::vec3& halfSize) {

    const glm::vec3 center = {0, halfSize.y, 0};
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

void PhysicsTestSceneBase::makeSphere(const glm::vec3& position, float radius, bool isStatic) {

    Actor actor = getEngine().makeActor();
    actor.add<Transform>().move(position).scale(glm::vec3(radius));

    std::uniform_int_distribution<std::size_t> materialIndexDistribution(0, m_materials.size() - 1);
    actor.add<RenderInfo>(
        m_sphereModel,
        isStatic ? m_staticBodyMaterial : m_materials.at(materialIndexDistribution(m_randomEngine))
    ).isBatchingStatic = isStatic;

    auto& rb = actor.add<en::Rigidbody>(std::make_unique<en::SphereCollider>(radius));
    rb.isKinematic = isStatic;
    if (!rb.isKinematic) {
        std::uniform_real_distribution<float> d(-1.f, 1.f);
        rb.velocity = {d(m_randomEngine), d(m_randomEngine), d(m_randomEngine)};
    }
    //rb.bounciness = 0.5f;
}

void PhysicsTestSceneBase::makeCube(const glm::vec3& position, const glm::vec3& halfSize, bool isStatic) {

    Actor actor = getEngine().makeActor();
    actor.add<Transform>().move(position).scale(halfSize);

    std::uniform_int_distribution<std::size_t> materialIndexDistribution(0, m_materials.size() - 1);
    actor.add<RenderInfo>(
        m_cubeModel,
        isStatic ? m_staticBodyMaterial : m_materials.at(materialIndexDistribution(m_randomEngine))
    ).isBatchingStatic = isStatic;

    auto& rb = actor.add<Rigidbody>(std::make_unique<AABBCollider>(halfSize));
    rb.isKinematic = isStatic;
    if (!rb.isKinematic) {
        std::uniform_real_distribution<float> d(-10.f, 10.f);
        rb.velocity = {d(m_randomEngine), d(m_randomEngine), d(m_randomEngine)};
    }
    //rb.bounciness = 0.5f;
}

void PhysicsTestSceneBase::cacheMaterials() {

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

glm::vec3 PhysicsTestSceneBase::getRandomVectorMinMax(const glm::vec3& min, const glm::vec3& max) {

    std::uniform_real_distribution<float> x(min.x, max.x);
    std::uniform_real_distribution<float> y(min.y, max.y);
    std::uniform_real_distribution<float> z(min.z, max.z);
    return {x(m_randomEngine), y(m_randomEngine), z(m_randomEngine)};
}

glm::vec3 PhysicsTestSceneBase::getRandomVectorCenterHalfSize(const glm::vec3& center, const glm::vec3& halfSize) {

    return getRandomVectorMinMax(center - halfSize, center + halfSize);
}
