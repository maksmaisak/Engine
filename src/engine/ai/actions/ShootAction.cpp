//
// Created by Maksym Maisak on 2019-09-28.
//

#include "ShootAction.h"
#include "Engine.h"
#include "Transform.h"
#include "Sprite.h"
#include "InlineBehavior.h"
#include "TileLayer.h"
#include "Blackboard.h"

using namespace ai;

ShootAction::ShootAction(const en::Name& targetPositionName) :
    m_targetPositionName(targetPositionName)
{}

ActionOutcome ShootAction::execute() {

    if (!m_blackboard) {
        return ActionOutcome::Fail;
    }

    en::GridPosition targetPosition;
    if (const auto targetPositionOptional = m_blackboard->get<en::GridPosition>(m_targetPositionName)) {
        targetPosition = *targetPositionOptional;
    } else {
        return ActionOutcome::Fail;
    }

    en::Actor bullet = m_actor.getEngine().makeActor("Bullet shot by " + m_actor.getName().getString());
    auto& bulletTransform = bullet.add<en::Transform>(m_actor.get<en::Transform>().getWorldTransform())
        .move(0.5f, 0.5f, 1.f)
        .setLocalScale(0.3f);

    auto& sprite = bullet.add<en::Sprite>();
    sprite.color = {1,0,0,1};

    constexpr float bulletSpeed = 10.f;
    const glm::vec2 delta = glm::vec2(targetPosition) - glm::vec2(bulletTransform.getWorldPosition());
    const glm::vec2 velocity = delta * bulletSpeed / (glm::length(delta) + glm::epsilon<float>());
    bullet.add<en::InlineBehavior>([velocity](en::Actor& bullet, float dt) {

        auto& transform = bullet.get<en::Transform>();
        transform.move(velocity * dt);

        en::EntityRegistry& registry = bullet.getEngine().getRegistry();
        const en::GridPosition currentTilePosition = transform.getWorldPosition();

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
