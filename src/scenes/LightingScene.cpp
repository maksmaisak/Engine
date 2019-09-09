//
// Created by Maksym Maisak on 2019-01-10.
//

#include "LightingScene.h"

#include <memory>
#include "config.hpp"
#include "Camera.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "Light.h"
#include "CameraOrbitBehavior.h"
#include "RotatingBehavior.hpp"
#include "LightPropertyAnimator.h"
#include "Resources.h"
#include "Texture.hpp"
#include "Material.h"
#include "glm.hpp"
#include <glm/gtx/euler_angles.hpp>

constexpr bool AnimateLightProperties = false;
constexpr int NumRotatingLights = 2;

void LightingScene::open() {

    m_renderSettings.ambientColor = glm::vec3(1.010478, 1.854524, 2.27060) * 0.5f;

    en::Engine& engine = getEngine();

    auto camera = engine.makeActor("Camera");
    camera.add<en::Camera>();
    camera.add<en::Transform>().move({0, 2, 7});
    camera.add<CameraOrbitBehavior>(7.f, -45.f, 89.f);

    auto ambientLight = engine.makeActor("AmbientLight");
    ambientLight.add<en::Transform>();
    {
        auto& l = ambientLight.add<en::Light>();
        l.color = {0,0,0};
        l.colorAmbient = glm::vec3(0.02f);
    }
    if (AnimateLightProperties) ambientLight.add<LightPropertyAnimator>();

    auto directionalLight = engine.makeActor("DirectionalLight");
    directionalLight.add<RotatingBehavior>(glm::vec3(0, 1, 0), glm::radians(10.f));
    directionalLight.add<en::Transform>()
        //.rotate(glm::radians(0.f), {1, 0, 0});
        .setLocalRotation(glm::toQuat(glm::orientate4(glm::radians(glm::vec3(45,0,0)))));
    {
        auto& l = directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
        l.intensity = 2.21f;
    }
    if (AnimateLightProperties) directionalLight.add<LightPropertyAnimator>();

    auto spotLight = engine.makeActor("SpotLight");
    spotLight.add<en::Transform>()
        .move({0, 0, 2});
    {
        glm::vec3 spotLightColor = {0, 1, 1};

        {
            auto child = engine.makeActor("SpotLightModel");
            child.add<en::Transform>()
                .setParent(spotLight)
                .scale(glm::vec3(0.2f))
                .rotate(glm::radians(-90.f), {1, 0, 0})
                .move({0, 0, 0.1});

            auto model = en::Models::get(config::MODEL_PATH + "cone_smooth.obj");
            auto material = std::make_shared<en::Material>("color");
            material->setUniformValue("diffuseColor", spotLightColor);
            child.add<en::RenderInfo>(model, material);
        }

        auto& l = spotLight.add<en::Light>(en::Light::Kind::SPOT);
        l.color = spotLightColor;
        l.intensity = 2.f;
        l.spotlightInnerCutoff = glm::cos(glm::radians(20.f));
        l.spotlightOuterCutoff = glm::cos(glm::radians(45.f));
    }
    spotLight.add<RotatingBehavior>(glm::vec3(1,0,0));
    if (AnimateLightProperties) spotLight.add<LightPropertyAnimator>();

    auto rotatingLights = engine.makeActor("RotatingLights");
    rotatingLights.add<en::Transform>();
    rotatingLights.add<RotatingBehavior>(glm::vec3(0,1,0));

    auto lightModel = en::Models::get(config::MODEL_PATH + "sphere.obj");
    for (int i = 0; i < NumRotatingLights; ++i) {

        auto light = engine.makeActor("Light");
        light.add<en::Transform>()
            .setParent(rotatingLights)
            .move(glm::rotate(glm::vec3(0,0,2), glm::two_pi<float>() * (i + 1.f) / NumRotatingLights, glm::vec3(0,1,0)))
            .scale(glm::vec3(0.1f));

        glm::vec3 lightColor = glm::abs(glm::make_vec3(glm::circularRand(1.f)));

        auto lightMaterial = std::make_shared<en::Material>("color");
        lightMaterial->setUniformValue("diffuseColor", lightColor);

        light.add<en::RenderInfo>(lightModel, lightMaterial);
        auto& l = light.add<en::Light>();
        l.color = lightColor;
        l.intensity = 5.f;
        if (AnimateLightProperties) light.add<LightPropertyAnimator>();
    }

    auto sphere = engine.makeActor("Sphere");
    sphere.add<en::Transform>().move({0, 0, 0});
    {
        auto model = en::Models::get(config::MODEL_PATH + "sphere.obj");
        auto material = std::make_shared<en::Material>("wobble");
        material->setUniformValue("diffuseTexture", en::Textures::white());
        material->setUniformValue("timeScale", 10.f);
        material->setUniformValue("phaseOffsetPerUnitDistance", 10.f);
        material->setUniformValue("wobbleMultiplierMin", 1.2f);
        material->setUniformValue("wobbleMultiplierMax", 0.8f);
        material->setUniformValue("transitionColor", glm::vec4(1.f, 1.f, 1.f, 0.7f));
        material->setUniformValue("transitionWobbleFactorMin", -0.2f);
        material->setUniformValue("transitionWobbleFactorMax",  0.2f);
        sphere.add<en::RenderInfo>(model, std::move(material));
    }
    camera.get<CameraOrbitBehavior>().setTarget(sphere);

    auto pillar = engine.makeActor("Pillar");
    pillar.add<en::Transform>().move({-4, -1, 0}).scale(glm::vec3(0.01f));
    {
        auto models = en::Models::get(config::MODEL_PATH + "Pillar_tall.obj");
        auto material = std::make_shared<en::Material>("pbr");
        material->setUniformValue("albedoMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/pillar/Pilars_AlbedoTransparency.png"));
        material->setUniformValue("metallicSmoothnessMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/pillar/Pilars_MetallicSmoothness.png", GL_RGBA));
        material->setUniformValue("normalMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/pillar/Pilars_Normal.png", GL_RGBA));
        material->setUniformValue("aoMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/pillar/Pilars_AO.png", GL_RGBA));
        material->setUniformValue("albedoColor"         , glm::vec4(1));
        material->setUniformValue("metallicMultiplier"  , 1.f);
        material->setUniformValue("smoothnessMultiplier", 1.f);
        material->setUniformValue("aoMultiplier"        , 1.f);
        pillar.add<en::RenderInfo>(models, std::move(material));
    }

    auto plane = engine.makeActor("Plane");
    plane.add<en::Transform>().move({0, -1, 0}).setLocalScale(glm::vec3(7));
    {
        auto model = en::Models::get(config::MODEL_PATH + "plane.obj");
        auto material = std::make_shared<en::Material>("pbr");
        material->setUniformValue("albedoMap"   , en::Textures::get(config::TEXTURE_PATH + "testPBR/oldTiledStone/tiledstone1_basecolor.png"));
        material->setUniformValue("metallicSmoothnessMap", en::Textures::get(config::TEXTURE_PATH + "testPBR/oldTiledStone/tiledstone1_metallicSmoothness.psd", GL_RGBA));
        material->setUniformValue("normalMap"   , en::Textures::get(config::TEXTURE_PATH + "testPBR/oldTiledStone/tiledstone1_normal.png", GL_RGBA));
        material->setUniformValue("aoMap"       , en::Textures::get(config::TEXTURE_PATH + "testPBR/oldTiledStone/tiledstone1_AO.png"    , GL_RGBA));
        material->setUniformValue("albedoColor"         , glm::vec4(1));
        material->setUniformValue("metallicMultiplier"  , 1.f);
        material->setUniformValue("smoothnessMultiplier", 1.f);
        material->setUniformValue("aoMultiplier"        , 1.f);
        plane.add<en::RenderInfo>(model, std::move(material));
    }
}
