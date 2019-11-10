//
// Created by Maksym Maisak on 10/11/19.
//

#include "GLTestScene.h"
#include "Engine.h"
#include "Camera.h"
#include "Transform.h"
#include "InlineBehavior.h"

void GLTestScene::open() {

    m_renderSettings.useSkybox = false;

    en::Actor camera = en::Engine::get().makeActor("Camera");
    camera.add<en::Camera>();
    camera.add<en::Transform>().move({0, 2, 7});
    //camera.add<CameraOrbitBehavior>(7.f, -45.f, 89.f);

    en::Engine::get().makeActor("Drawer").add<en::InlineBehavior>(en::InlineBehavior::Draw, [](en::Actor&){

    });
}
