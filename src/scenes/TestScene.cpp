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
#include "config.hpp"

#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "RenderInfo.h"
#include "Rigidbody.h"
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

                auto& rb = actor.add<en::Rigidbody>(std::make_shared<en::SphereCollider>(radius));
                rb.isKinematic = true;
                //rb.radius = radius;

                actor.add<en::RenderInfo>(model, material).isBatchingStatic = true;
            }
        }
    }

    void addRingItems(
        en::Engine& engine,
        en::Entity parent,
        const std::shared_ptr<en::Material>& sphereMaterial,
        const std::shared_ptr<en::Material>& cubeMaterial,
        std::size_t numItems = 10,
        float radius = 3.5f
    ) {

        auto cubeModel   = en::Models::get(config::MODEL_PATH + "cube_flat.obj");
        auto sphereModel = en::Models::get(config::MODEL_PATH + "sphere_smooth.obj");

        for (std::size_t i = 0; i < numItems; ++i) {

            en::Actor object = engine.makeActor("RingItem");

            {
                const float angle = glm::two_pi<float>() * (float) i / numItems;
                const glm::vec3 offset = {
                    glm::cos(angle) * radius,
                    0,
                    glm::sin(angle) * radius
                };

                auto& tf = object.add<en::Transform>()
                    .setLocalPosition(offset)
                    .scale(glm::vec3(0.2f));

                //tf.setParent(parent);
            }

            {
                auto& rb = object.add<en::Rigidbody>();
                //rb.isKinematic = true;
                //rb.radius = 0.2f;
                rb.bounciness = 0.98f;
                rb.invMass = 1.f / 0.1f;
            }

            //object.add<RotatingBehavior>();

            if (i % 2 == 0) {

                //object.add<en::Light>().intensity = 2.f;
                object.add<en::RenderInfo>(sphereModel, sphereMaterial);
                object.get<en::Rigidbody>().collider = std::make_shared<en::SphereCollider>(0.2f);

            } else {

                object.add<en::RenderInfo>(cubeModel, cubeMaterial);
                object.get<en::Transform>().scale(glm::vec3(2.f));
                auto& rb = object.get<en::Rigidbody>();
                //rb.collider = std::make_shared<en::AABBCollider>(glm::vec3(radius * 2.f));
                rb.collider = std::make_shared<en::SphereCollider>(0.4f);
                rb.invMass /= 2.f;
            }
        }
    }
}

void TestScene::open() {

    en::Engine& engine = getEngine();

    m_renderSettings.ambientColor = glm::vec3(1.010478, 1.854524, 2.270603) * 0.5f;

    // models
    auto planeModel  = en::Models::get(config::MODEL_PATH + "plane.obj");
    auto cubeModel   = en::Models::get(config::MODEL_PATH + "cube_flat.obj");
    auto sphereModel = en::Models::get(config::MODEL_PATH + "sphere.obj");

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

    //en::Models::get(config::MODEL_PATH + "sphere3.obj");
    //en::Models::removeUnused();

    // SCENE SETUP

    // Add the camera using en::Actor,
    // a thin wrapper around en::Engine and en::Entity
    // Using it to add components also ensures that components
    // inheriting from en::Behavior actually have their update functions called.
    // TODO have behaviors work when added via registry too.
    en::Actor camera = engine.makeActor("Camera");
    camera.add<en::Camera>();
    camera.add<en::Transform>().move({0,0,10}).rotate(90.f, {0,1,0});
    auto& cameraOrbitBehavior = camera.add<CameraOrbitBehavior>(10.f, -15.f, 60.f);

    auto center = engine.makeActor();
    center.add<en::Transform>();
    cameraOrbitBehavior.setTarget(center);

    makeFloorFromSpheres(engine, 30, 20);

    // Add a directional light
    en::Actor directionalLight = engine.makeActor("DirectionalLight");
    directionalLight.add<en::Transform>()
        .setLocalRotation(glm::toQuat(glm::orientate3(glm::radians(glm::vec3(45, 0, 90)))));
    {
        auto& l = directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
        l.color = glm::vec3(1.f);
        l.intensity = 1.f;
    }

    // Add an empty rotating object.
    en::Actor ring = engine.makeActor("Ring");
    ring.add<en::Transform>();
    {
        auto& rb = ring.add<en::Rigidbody>(std::make_shared<en::SphereCollider>(2.5f));
        //rb.isKinematic = true;
        //rb.radius = 2.5f;
    }
    //ring.add<RotatingBehavior>();
    addRingItems(engine, ring, sphereMaterial, cubeMaterial, 20);

    en::Actor sphere = engine.makeActor("Main sphere");
    auto& tf = sphere.add<en::Transform>();
    tf.setParent(ring);
    tf.setLocalScale({2.5f, 2.5f, 2.5f});
    sphere.add<en::RenderInfo>(sphereModel, sphereMaterial);
}
