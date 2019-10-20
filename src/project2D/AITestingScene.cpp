//
// Created by Maksym Maisak on 5/10/19.
//

#include "AITestingScene.h"
#include <random>
#include "AIController.h"
#include "Camera.h"
#include "Transform.h"
#include "TileLayer.h"
#include "InlineBehavior.h"
#include "ConditionAction.h"
#include "ConditionDecorator.h"
#include "InlineAction.h"
#include "Sequence.h"
#include "Selector.h"
#include "MoveAction.h"
#include "ShootAction.h"
#include "ParallelAction.h"
#include "SimpleParallelAction.h"
#include "RepeatDecorator.h"
#include "WhileDecorator.h"
#include "DelayAction.h"
#include "Pathfinding.h"
#include "Sprite.h"

namespace {

    const en::Name closestObstacleName = "closestObstacleName";
    const en::Name targetItemName = "targetItem";
    const en::Name stockpileLocationName = "stockpileLocationName";
    const en::Name grabbedItemName = "grabbedItem";

    struct Item {
        bool isGrabbed = false;
    };

    void makeItems(en::Engine& engine) {

        std::default_random_engine r(0/*std::random_device{}()*/);
        std::uniform_int_distribution<int> distributionX(-30, 30);
        std::uniform_int_distribution<int> distributionY(-30, 30);
        const auto getRandomPosition = [&]() -> glm::vec3 {
            return {distributionX(r) + 0.5f, distributionY(r) + 0.5f, 1.f};
        };

        for (int i = 0; i < 50; ++i) {

            const glm::vec3 position = getRandomPosition();

            if (!ai::Pathfinding::isObstacle(engine, glm::floor(position))) {

                en::Actor actor = engine.makeActor("Item #" + std::to_string(i));
                actor.add<en::Transform>(position).scale(0.9f);
                actor.add<en::Sprite>().color = {0,1,0,1};
                actor.add<Item>();
            }
        }
    }

    std::optional<en::GridPosition> findClosest(en::Actor& actor, const ai::Pathfinding::goalPredicate_t& predicate) {

        if (const auto* const transform = actor.tryGet<en::Transform>()) {

            const en::GridPosition startPosition = transform->getWorldPosition();

            ai::PathfindingParams params;
            params.allowObstacleGoal = true;
            const std::optional<ai::PathfindingPath> path = ai::Pathfinding::getPath(actor.getEngine(),
                startPosition, predicate, ai::Pathfinding::nullHeuristic, params
            );

            if (path) {
                if (!path->empty()) {
                    return path->back();
                }

                return startPosition;
            }
        }

        return std::nullopt;
    }

    bool findClosestObstacle(en::Actor& actor, ai::Blackboard& blackboard) {

        const auto isObstacle = [&engine = actor.getEngine()](const en::GridPosition& pos) {
            return ai::Pathfinding::isObstacle(engine, pos);
        };

        if (const std::optional<en::GridPosition> position = findClosest(actor, isObstacle)) {
            blackboard.set<en::GridPosition>(closestObstacleName, *position);
            return true;
        }

        return false;
    }

    bool findClosestItem(en::Actor& actor, ai::Blackboard& blackboard) {

        en::Engine& engine = actor.getEngine();

        const auto getDistance = [&, ownPosition = glm::vec2(actor.get<en::Transform>().getWorldPosition())](en::Entity e) {

            if (const auto* const transform = engine.actor(e).tryGet<en::Transform>()) {
                const en::GridPosition position = glm::floor(transform->getWorldPosition());
                if (!ai::Pathfinding::isObstacle(engine, position)) {
                    return glm::distance2(ownPosition, glm::vec2(position));
                }
            }

            return std::numeric_limits<float>::infinity();
        };

        // TODO Spatial partitioning of items. Something exposed like `getTrackedEntitiesAt(en::GridPosition);`
        const auto entitiesRange = engine.getRegistry().with<en::Transform, en::Sprite, Item>();
        const auto it = std::min_element(entitiesRange.begin(), entitiesRange.end(), [&](en::Entity a, en::Entity b) {
            return getDistance(a) < getDistance(b);
        });

        if (it != entitiesRange.end()) {

            const en::Actor itemActor = engine.actor(*it);
            assert(itemActor);
            blackboard.set<en::Actor>(targetItemName, itemActor);
            return true;
        }

        return false;
    }

    template<typename TValue>
    std::unique_ptr<ai::InlineAction> makeUnset(const en::Name& key) {
        return std::make_unique<ai::InlineAction>([key](en::Actor&, ai::Blackboard& blackboard) {
            blackboard.unset<TValue>(key);
        });
    }

    std::unique_ptr<ai::Sequence> makeShootAtClosestObstacleSubtree() {

        using namespace ai;
        using std::make_unique;

        const auto hasValidClosestObstacle = [](en::Actor& actor, Blackboard& blackboard) {

            if (const auto targetPositionOptional = blackboard.get<en::GridPosition>(closestObstacleName)) {
                const bool isObstacle = Pathfinding::isObstacle(actor.getEngine(), *targetPositionOptional);
                return isObstacle;
            }

            return false;
        };

        const auto isClosestObstacleInShootingRange = [](en::Actor& actor, Blackboard& blackboard) {

            if (const auto closestObstacleOptional = blackboard.get<en::GridPosition>(closestObstacleName)) {
                return 10.f * 10.f > glm::distance2(
                    glm::vec2(*closestObstacleOptional),
                    glm::vec2(glm::floor(actor.get<en::Transform>().getWorldPosition()))
                );
            }

            return false;
        };

        return make_unique<Sequence>(
            make_unique<Selector>(
                make_unique<ConditionAction>(hasValidClosestObstacle),
                make_unique<ConditionAction>(findClosestObstacle)
            ),
            make_unique<DelayAction>(0.1f),
            make_unique<ConditionAction>(isClosestObstacleInShootingRange),
            make_unique<ShootAction>(closestObstacleName),
            makeUnset<en::GridPosition>(closestObstacleName)
        );
    }

    std::unique_ptr<ai::BehaviorTree> makeBehaviorTree() {

        using namespace ai;
        using std::make_unique;

        const auto hasValidTargetItem = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            if (const en::Actor itemActor = blackboard.getActorChecked(targetItemName)) {
                const bool isObstructed = Pathfinding::isObstacle(actor.getEngine(), glm::floor(itemActor.get<en::Transform>().getWorldPosition()));
                return !isObstructed;
            }

            return false;
        };

        const auto grabItem = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            if (blackboard.getActorChecked(grabbedItemName)) {
                return false;
            }

            if (en::Actor itemActor = blackboard.getActorChecked(targetItemName)) {

                const auto& ownTransform = actor.get<en::Transform>();
                auto& itemTransform = itemActor.get<en::Transform>();

                if (2.f * 2.f > glm::distance2(ownTransform.getWorldPosition(), itemTransform.getWorldPosition())) {

                    auto& item = itemActor.get<Item>();
                    if (!item.isGrabbed) {

                        item.isGrabbed = true;
                        itemTransform.setParent(actor);
                        itemTransform.setLocalPosition({0.5f, 0.f, 0.f});
                        blackboard.set<en::Actor>(grabbedItemName, itemActor);
                        return true;
                    }
                }
            }

            return false;
        };

        const auto dropItem = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            if (en::Actor grabbedItem = blackboard.getActorChecked(grabbedItemName)) {

                grabbedItem.destroy();
                blackboard.unset<en::Actor>(grabbedItemName);
                return true;
            }

            return false;
        };

        auto behaviorTree = make_unique<BehaviorTree>(make_unique<SimpleParallelAction>(
            make_unique<Sequence>(
                make_unique<Selector>(
                    make_unique<ConditionAction>(hasValidTargetItem),
                    make_unique<ConditionAction>(findClosestItem)
                ),
                make_unique<Selector>(
                    make_unique<Sequence>(
                        make_unique<MoveAction>(targetItemName),
                        make_unique<ConditionAction>(grabItem),
                        make_unique<MoveAction>(stockpileLocationName),
                        make_unique<ConditionAction>(dropItem)
                    ),
                    makeUnset<en::Actor>(targetItemName)
                )
            ),
            makeShootAtClosestObstacleSubtree()
        ));

        behaviorTree->getBlackboard().set<en::GridPosition>(stockpileLocationName, {0, 0});

        return behaviorTree;
    }
}

void AITestingScene::open() {

    en::Engine& engine = getEngine();
    engine.makeActor("layerDefault").add<en::TileLayer>();

    en::Actor cameraActor = engine.makeActor("Test camera");
    cameraActor.add<en::Camera>().isOrthographic = true;
    cameraActor.add<en::Transform>().move({0, 0, 10.f});

    makeItems(engine);

    ai::AIController& aiController = ai::AIController::create(engine);
    aiController.setBehaviorTree(makeBehaviorTree());
}