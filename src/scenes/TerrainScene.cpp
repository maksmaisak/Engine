//
// Created by Maksym Maisak on 2019-01-16.
//

#include "TerrainScene.h"

#include <memory>
#include "Config.h"
#include "Camera.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "Light.h"
#include "CameraOrbitBehavior.h"
#include "RotatingBehavior.hpp"
#include "LightPropertyAnimator.h"
#include "Resources.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "glm.h"
#include <glm/gtx/euler_angles.hpp>

void TerrainScene::open() {

    auto& engine = getEngine();

    auto camera = engine.makeActor("Camera");
    camera.add<en::Camera>();
    camera.add<en::Transform>().move({0, 2, 5});
    camera.add<CameraOrbitBehavior>(5.f, -45.f, 89.f);

    auto directionalLight = engine.makeActor("DirectionalLight");
    directionalLight.add<en::Transform>()
        .setLocalRotation(glm::toQuat(glm::orientate4(glm::radians(glm::vec3(-45,0,-90)))));
        //.rotate(glm::radians(-180.f), glm::vec3(0, 1, 0));
        //.rotate(glm::radians(-90.f), glm::vec3(0, 1, 0));
    {
        auto& l = directionalLight.add<en::Light>(en::Light::Kind::DIRECTIONAL);
        l.colorAmbient = {0.1, 0.1, 0.1};
        l.intensity = 1.f;
    }
    //directionalLight.add<RotatingBehavior>(glm::vec3(0, 1, 0), glm::radians(45.f));

    auto terrain = engine.makeActor("Terrain");
    terrain.add<en::Transform>().scale({3, 3, 3});
    {
        auto mesh = en::Models::get(config::MODEL_PATH + "plane_8192.obj");
        auto material = std::make_shared<en::Material>("terrain");
        material->setUniformValue("heightmap", en::Textures::get(config::TEXTURE_PATH + "terrain/heightmap.png", GL_RGBA));
        material->setUniformValue("maxHeight", 1.f);
        material->setUniformValue("splatmap", en::Textures::get(config::TEXTURE_PATH + "terrain/splatmap.png", GL_RGBA));
        material->setUniformValue("diffuse1", en::Textures::get(config::TEXTURE_PATH + "terrain/diffuse1.jpg"));
        material->setUniformValue("diffuse2", en::Textures::get(config::TEXTURE_PATH + "terrain/water/water.jpg"));
        material->setUniformValue("diffuse3", en::Textures::get(config::TEXTURE_PATH + "terrain/diffuse3.jpg"));
        material->setUniformValue("diffuse4", en::Textures::get(config::TEXTURE_PATH + "terrain/diffuse4.jpg"));
        material->setUniformValue("diffuseColor" , glm::vec3(1.f));
        material->setUniformValue("specularColor", glm::vec3(0.02f));
        material->setUniformValue("specularMap", en::Textures::white());
        material->setUniformValue("shininess"  , 64.f);
        material->setUniformValue("noise", en::Textures::get(config::TEXTURE_PATH + "terrain/water/noise_c.jpg"));
        terrain.add<en::RenderInfo>(mesh, std::move(material));
    }
    //terrain.add<RotatingBehavior>(glm::vec3(0, 1, 0), glm::radians(45.f));
    camera.get<CameraOrbitBehavior>().setTarget(terrain);
}
