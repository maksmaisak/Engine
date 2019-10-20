//
// Created by Maksym Maisak on 2019-09-28.
//

#include "ShootAction.h"
#include "Transform.h"
#include "Sprite.h"
#include "InlineBehavior.h"
#include "TileLayer.h"
#include "Blackboard.h"
#include "Bounds.h"
#include "LineRenderer.h"

using namespace ai;

namespace {

    en::Actor makeBullet(en::Actor& shooterActor, const glm::vec2& targetPosition) {

        en::Actor bullet = shooterActor.getEngine().makeActor("Bullet shot by " + shooterActor.getName().getString());
        auto& bulletTransform = bullet.add<en::Transform>(shooterActor.get<en::Transform>().getWorldTransform())
            .move(0.5f, 0.5f, 1.f)
            .setLocalScale(0.3f);

        auto& sprite = bullet.add<en::Sprite>();
        sprite.color = {1,0,0,1};

        constexpr float bulletSpeed = 20.f;
        const glm::vec2 delta = targetPosition - glm::vec2(bulletTransform.getWorldPosition());
        const glm::vec2 velocity = delta * bulletSpeed / (glm::length(delta) + glm::epsilon<float>());
        bullet.add<en::InlineBehavior>([velocity](en::Actor& bullet, float dt) {

            auto& transform = bullet.get<en::Transform>();
            transform.move(velocity * dt);

            const utils::Bounds2D spriteBounds = bullet.get<en::Sprite>().getAABB(transform.getWorldTransform());
            const en::GridPosition min = glm::floor(spriteBounds.min);
            const en::GridPosition max = glm::floor(spriteBounds.max);

            en::EntityRegistry& registry = bullet.getEngine().getRegistry();
            for (en::Entity e : registry.with<en::TileLayer>()) {
                auto& tileLayer = registry.get<en::TileLayer>(e);

                for (en::GridCoordinate y = min.y; y <= max.y; ++y) {
                    for (en::GridCoordinate x = min.x; x <= max.x; ++x) {

                        const utils::Bounds2D tileBounds = utils::Bounds2D({x, y}, {x + 1, y + 1});
                        if (spriteBounds.intersects(tileBounds)) {

                            en::Tile& tile = tileLayer.at({x, y});
                            if (tile.isObstacle) {
                                tile.atlasCoordinates = {0, 0};
                                tile.isObstacle = false;
                                bullet.destroy();
                                return;
                            }
                        }
                    }
                }
            }
        });

        return bullet;
    }
}

ShootAction::ShootAction(const en::Name& targetPositionName) :
    m_targetPositionName(targetPositionName)
{}

ActionOutcome ShootAction::execute() {

    en::GridPosition targetPosition;
    if (const auto targetPositionOptional = m_blackboard->get<en::GridPosition>(m_targetPositionName)) {
        targetPosition = *targetPositionOptional;
    } else {
        return ActionOutcome::Fail;
    }

    makeBullet(m_actor, glm::vec2(targetPosition));
    return ActionOutcome::Success;
}
