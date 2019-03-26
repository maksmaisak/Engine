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
    m_randomPosition(makeRandomVectorGeneratorCenterHalfSize({0, preset.fieldHalfSize.y, 0}, preset.fieldHalfSize))
{}

void PhysicsTestScene::open() {

    setUpNonBodies(m_preset.fieldHalfSize);
    setUpBounds(m_preset.fieldHalfSize);
    addStaticBodies();
    addDynamicBodies();
}

void PhysicsTestScene::update(float dt) {

    if (utils::KeyboardHelper::isDown("r"))
        getEngine().getSceneManager().setCurrentSceneNextUpdate<PhysicsTestScene>();
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
    const auto randomRadius = [&e = m_randomEngine]() {return std::uniform_real_distribution(0.5f, 2.f)(e);};
    const auto randomBool   = [&e = m_randomEngine]() {
        return std::uniform_int_distribution(0, 1)(e) == 1;
    };

    for (int i = 0; i < m_preset.numBodiesDynamic; ++i) {
        if (randomBool())
            makeCube(m_randomPosition(), randomHalfSize());
        else
            makeSphere(m_randomPosition(), randomRadius());
    }
}
