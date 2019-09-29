//
// Created by Maksym Maisak on 2019-09-28.
//

#include "ShootAction.h"
#include "Engine.h"
#include "Transform.h"
#include "Sprite.h"
#include "InlineBehavior.h"
#include "TileLayer.h"

using namespace ai;

ShootAction::ShootAction(const glm::vec2& targetPosition) :
    m_targetPosition(targetPosition)
{}

ActionOutcome ShootAction::execute() {

    en::Actor bullet = actor.getEngine().makeActor("Bullet shot by " + actor.getName());
    bullet.add<en::Transform>(actor.get<en::Transform>().getWorldTransform())
        .move(0.5f, 0.5f, 1.f)
        .setLocalScale(0.3f);

    auto& sprite = bullet.add<en::Sprite>();
    sprite.color = {1,0,0,1};

    bullet.add<en::InlineBehavior>([velocity = glm::vec2(10.f, 2.f)](en::Actor& bullet, float dt) {

        auto& transform = bullet.get<en::Transform>();
        transform.move(velocity * dt);
        const en::TileLayer::Coordinates currentTilePosition = transform.getWorldPosition();

        en::EntityRegistry& registry = bullet.getEngine().getRegistry();

        bool isObstacle = false;
        for (en::Entity e : registry.with<en::TileLayer>()) {

            auto& tileLayer = registry.get<en::TileLayer>(e);
            if (tileLayer.at(currentTilePosition).isObstacle) {
                isObstacle = true;
                break;
            }
        }

        if (isObstacle) {
            bullet.destroy();
        }
    });

    return ActionOutcome::Success;
}
