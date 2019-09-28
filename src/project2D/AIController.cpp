//
// Created by Maksym Maisak on 2019-09-28.
//

#include "AIController.h"
#include "Engine.h"
#include "Transform.h"
#include "Sprite.h"

void AIController::update(float dt) {

    Behavior::update(dt);

    m_actor.get<en::Transform>().move(glm::vec3(1, 1, 0) * dt);
}

AIController& AIController::create(class en::Engine& engine) {

    en::Actor actor = engine.makeActor("AI Actor");
    actor.add<en::Transform>();
    actor.add<en::Sprite>();
    return actor.add<AIController>();
}