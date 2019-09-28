//
// Created by Maksym Maisak on 2019-09-28.
//

#include "ShootAction.h"
#include "Engine.h"
#include "Transform.h"
#include "Sprite.h"

using namespace ai;

ShootAction::ShootAction(const glm::vec2& targetPosition) :
    m_targetPosition(targetPosition)
{}

ActionOutcome ShootAction::execute() {

    en::Actor bullet = actor.getEngine().makeActor("Bullet shot by " + actor.getName());
    bullet.add<en::Transform>(actor.get<en::Transform>().getWorldTransform()).move(0.5f, 0.5f, 1.f).setLocalScale(0.5f);
    bullet.add<en::Sprite>().color = {1,0,0,1};

    /*
    bullet.add<en::InlineBehavior>([velocity = glm::vec2(1.f, 1.f)](en::Actor& bullet, float dt) {
        bullet.get<en::Transform>().move(velocity * dt);
    });*/

    return ActionOutcome::Success;
}
