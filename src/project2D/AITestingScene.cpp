//
// Created by Maksym Maisak on 5/10/19.
//

#include "AITestingScene.h"
#include "AIController.h"
#include "Camera.h"
#include "Transform.h"
#include "TileLayer.h"
#include "MoveAction.h"
#include "ShootAction.h"

void AITestingScene::open() {

    en::Engine& engine = getEngine();

    engine.makeActor("Test tile layer").add<en::TileLayer>();

    en::Actor cameraActor = engine.makeActor("Test camera");
    cameraActor.add<en::Camera>().isOrthographic = true;
    cameraActor.add<en::Transform>().move({0, 0, 10.f});

    ai::AIController::create(engine)
        .enqueueAction(std::make_unique<ai::MoveAction>(glm::i64vec2(12, 5)))
        .enqueueAction(std::make_unique<ai::ShootAction>(glm::vec2(20.5f, 10.5f)))
        .enqueueAction(std::make_unique<ai::MoveAction>(glm::i64vec2(12, 10)))
        .enqueueAction(std::make_unique<ai::MoveAction>(glm::i64vec2(0 , 10)));
}
