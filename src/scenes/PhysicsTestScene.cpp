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
#include "Texture.h"
#include "Resources.h"
#include "KeyboardHelper.h"
#include "Config.h"

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
                const glm::vec2 positionNormalized = {
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
                rb.isStatic = true;
                //rb.radius = radius;

                actor.add<en::RenderInfo>(model, material).isBatchingStatic = true;
            }
        }
    }
}

PhysicsTestScene::PhysicsTestScene(const Preset& preset) :
    m_preset(preset),
    m_bodyGenerator(preset.fieldHalfSize)
{}

void PhysicsTestScene::open() {

    m_bodyGenerator
        .setEngine(getEngine())
        .setRandomizeRotation(true);
    m_bodyGenerator.setUpNonBodies();
    m_bodyGenerator.setUpBounds  ();
    addStaticBodies();
    addDynamicBodies();
}

void PhysicsTestScene::update(float dt) {

    if (utils::KeyboardHelper::isDown("r"))
        getEngine().getSceneManager().setCurrentSceneNextUpdate<PhysicsTestScene>();
}

void PhysicsTestScene::addStaticBodies() {

    for (int i = 0; i < m_preset.numBodiesStatic; ++i)
        m_bodyGenerator.makeRandomBody(m_bodyGenerator.getRandomBodyPosition(), true);
}

void PhysicsTestScene::addDynamicBodies() {

    for (int i = 0; i < m_preset.numBodiesDynamic; ++i)
        m_bodyGenerator.makeRandomBody(m_bodyGenerator.getRandomBodyPosition(), false);
}
