//
// Created by Maksym Maisak on 5/10/19.
//

#include "AITestingScene.h"
#include <random>
#include <SFML/Window.hpp>
#include "LineRenderer.h"
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
#include "Bounds.h"

namespace {

    const en::Name closestObstacleName = "closestObstacle";
    const en::Name targetItemName = "targetItem";
    const en::Name stockpileName = "stockpileCenter";
    const en::Name stockpileTargetLocationName = "stockpileLocation";
    const en::Name grabbedItemName = "grabbedItem";

    struct Item {
        bool isGrabbed = false;
    };

    struct Reservation {
        en::Actor reserver;
    };

    struct Zone {
        en::Bounds2DGrid area;
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

    bool findClosestItemNotInStockpile(en::Actor& aiActor, ai::Blackboard& blackboard) {

        en::Engine& engine = aiActor.getEngine();

        constexpr float infinity = std::numeric_limits<float>::infinity();

        const auto getDistanceToItem = [&, ownPosition = glm::vec2(aiActor.get<en::Transform>().getWorldPosition())](en::Entity e) {

            const en::Actor itemActor = engine.actor(e);

            if (itemActor.get<Item>().isGrabbed) {
                return infinity;
            }

            if (const auto* const reservation = itemActor.tryGet<Reservation>()) {
                if (reservation->reserver != aiActor) {
                    return infinity;
                }
            }

            const auto* const transform = itemActor.tryGet<en::Transform>();
            if (!transform) {
                return infinity;
            }
            const en::GridPosition position = transform->getGridPosition();

            if (const Zone* const stockpile = blackboard.getComponentChecked<Zone>(stockpileName)) {
                if (stockpile->area.contains(position)) {
                    return infinity;
                }
            }

            if (ai::Pathfinding::isObstacle(engine, position)) {
                return infinity;
            }

            return glm::distance2(ownPosition, glm::vec2(position));
        };

        // TODO Spatial partitioning of items. Something exposed like `getTrackedEntitiesAt(en::GridPosition);`
        const auto entitiesRange = engine.getRegistry().with<en::Transform, en::Sprite, Item>();
        const auto it = std::min_element(entitiesRange.begin(), entitiesRange.end(), [&](en::Entity a, en::Entity b) {
            return getDistanceToItem(a) < getDistanceToItem(b);
        });

        if (it != entitiesRange.end() && getDistanceToItem(*it) != infinity) {

            en::Actor itemActor = engine.actor(*it);
            assert(itemActor);

            if (en::Actor previousItemActor = blackboard.getActorChecked(targetItemName)) {
                previousItemActor.remove<Reservation>();
            }

            blackboard.set<en::Actor>(targetItemName, itemActor);
            itemActor.add<Reservation>(aiActor);
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

                const bool isInShootingRange = 10.f * 10.f > glm::distance2(
                    glm::vec2(*closestObstacleOptional),
                    glm::vec2(actor.get<en::Transform>().getGridPosition())
                );
                return isInShootingRange;
            }

            return false;
        };

        return make_unique<Sequence>(
            make_unique<Selector>(
                make_unique<ConditionAction>(hasValidClosestObstacle),
                make_unique<ConditionAction>(findClosestObstacle)
            ),
            make_unique<ConditionAction>(isClosestObstacleInShootingRange),
            make_unique<ShootAction>(closestObstacleName),
            makeUnset<en::GridPosition>(closestObstacleName),
            make_unique<DelayAction>(0.1f)
        );
    }

    std::unique_ptr<ai::BehaviorTree> makeBehaviorTree() {

        using namespace ai;
        using std::make_unique;

        const auto hasValidTargetItem = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            const en::Actor itemActor = blackboard.getActorChecked(targetItemName);
            if (!itemActor) {
                return false;
            }

            const bool isObstructed = Pathfinding::isObstacle(actor.getEngine(), itemActor.get<en::Transform>().getGridPosition());
            if (isObstructed) {
                return false;
            }

            const auto* const item = itemActor.tryGet<Item>();
            if (!item) {
                return false;
            }

            if (item->isGrabbed) {
                return false;
            }

            if (const auto* const reservation = itemActor.tryGet<Reservation>()) {
                if (reservation->reserver != actor) {
                    return false;
                }
            }

            return true;
        };

        const auto hasValidStockpileTarget = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            const auto targetStockpileTarget = blackboard.get<en::GridPosition>(stockpileTargetLocationName);
            if (!targetStockpileTarget) {
                return false;
            }

            if (Pathfinding::isObstacle(actor.getEngine(), *targetStockpileTarget)) {
                return false;
            }

            en::Engine& engine = actor.getEngine();
            const auto items = engine.getRegistry().with<en::Transform, en::Sprite, Item>();
            const bool isOccupied = std::any_of(items.begin(), items.end(), [&](en::Entity e) {

                const en::Actor itemActor = engine.actor(e);
                if (itemActor.get<Item>().isGrabbed) {
                    return false;
                }
                const en::GridPosition itemPosition = itemActor.get<en::Transform>().getGridPosition();
                return itemPosition == *targetStockpileTarget;
            });

            return !isOccupied;
        };

        const auto findStockpileTarget = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            const Zone* const stockpile = blackboard.getComponentChecked<Zone>(stockpileName);
            if (!stockpile) {
                return false;
            }

            const en::GridPosition stockpileSize = stockpile->area.max + en::GridCoordinate(1) - stockpile->area.min;
            auto isOccupied = std::vector<bool>(stockpileSize.x * stockpileSize.y, false);

            en::Engine& engine = actor.getEngine();
            for (en::Entity e : engine.getRegistry().with<en::Transform, en::Sprite, Item>()) {
                const en::GridPosition itemGridPosition = engine.actor(e).get<en::Transform>().getGridPosition();
                if (stockpile->area.contains(itemGridPosition)) {
                    const en::GridPosition posInStockpile = itemGridPosition - stockpile->area.min;
                    isOccupied.at(posInStockpile.y * stockpileSize.x + posInStockpile.x) = true;
                }
            }
            const auto it = std::find_if_not(isOccupied.begin(), isOccupied.end(), [](bool isOccupied){return isOccupied;});
            if (it == isOccupied.end()) {
                return false;
            }

            const std::size_t index = it - isOccupied.begin();
            const en::GridPosition unoccupiedGridPosition = {index % stockpileSize.x, index / stockpileSize.x};
            blackboard.set<en::GridPosition>(stockpileTargetLocationName, unoccupiedGridPosition);
            return true;
        };

        const auto hasGrabbedItem = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            const en::Actor itemActor = blackboard.getActorChecked(grabbedItemName);
            return itemActor;
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

                        blackboard.set<en::Actor>(grabbedItemName, itemActor);
                        blackboard.unset<en::Actor>(targetItemName);

                        item.isGrabbed = true;
                        itemTransform.setParent(actor);
                        itemTransform.setLocalPosition({0.5f, 0.f, 0.f});

                        return true;
                    }
                }
            }

            return false;
        };

        const auto dropItem = [](en::Actor& actor, Blackboard& blackboard) -> bool {

            const auto getDropPosition = [&]() -> en::GridPosition {

                if (const auto gridPositionOptional = blackboard.get<en::GridPosition>(stockpileTargetLocationName)) {
                    return *gridPositionOptional;
                }

                return actor.get<en::Transform>().getGridPosition();
            };

            if (en::Actor grabbedItem = blackboard.getActorChecked(grabbedItemName)) {

                blackboard.unset<en::Actor>(grabbedItemName);

                grabbedItem.remove<Reservation>();
                grabbedItem.get<Item>().isGrabbed = false;
                grabbedItem.get<en::Transform>()
                    .setParent(en::nullEntity)
                    .setLocalPosition(glm::vec3(glm::vec2(getDropPosition()) + 0.5f, 0.f));

                return true;
            }

            return false;
        };

        auto behaviorTree = make_unique<BehaviorTree>(make_unique<Sequence>(

            // Ensure grabbed item
            make_unique<Selector>(
                make_unique<ConditionAction>(hasGrabbedItem),
                // Find and grab item
                make_unique<Sequence>(
                    // Ensure target item
                    make_unique<Selector>(
                        make_unique<ConditionAction>(hasValidTargetItem),
                        make_unique<ConditionAction>(findClosestItemNotInStockpile)
                    ),
                    make_unique<ConditionDecorator>(
                        hasValidTargetItem,
                        make_unique<Sequence>(
                            make_unique<MoveAction>(targetItemName),
                            make_unique<ConditionAction>(grabItem)
                        )
                    )
                )
            ),

            // Move item to stockpile
            make_unique<Selector>(
                make_unique<ConditionAction>(hasValidStockpileTarget),
                make_unique<ConditionAction>(findStockpileTarget)
            ),
            make_unique<MoveAction>(stockpileTargetLocationName),
            make_unique<ConditionDecorator>(
                hasValidStockpileTarget,
                make_unique<ConditionAction>(dropItem)
            )
        ));

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

    en::Actor zoneActor = engine.makeActor("Zone");
    zoneActor.add<Zone>(en::Bounds2DGrid({0, 0}, {10, 10}));

    engine.makeActor("ZoneRenderer").add<en::InlineBehavior>(en::InlineBehavior::Draw, [](en::Actor& actor) {

        en::LineRenderer& lineRenderer = en::LineRenderer::get(actor.getEngine());
        for (en::Entity e : actor.getEngine().getRegistry().with<Zone>()) {

            const Zone& zone = actor.getEngine().actor(e).get<Zone>();
            const en::Bounds2D bounds = {zone.area.min, zone.area.max + en::GridCoordinate(1)};
            lineRenderer.addAABB(bounds);
        }
    });

    for (int i = 0; i < 10; ++i) {

        ai::AIController& aiController = ai::AIController::create(engine);
        aiController.setBehaviorTree(makeBehaviorTree());
        aiController.getBehaviorTree()->getBlackboard().set<en::Actor>(stockpileName, zoneActor);

        // shared_ptr instead of unique_ptr because the lambda needs to be copyable to fit into std::function
        auto secondaryBehaviorTree = std::make_shared<ai::BehaviorTree>(makeShootAtClosestObstacleSubtree());
        aiController.getActor().add<en::InlineBehavior>([secondaryBehaviorTree = std::move(secondaryBehaviorTree)](en::Actor& actor, float dt) {
            secondaryBehaviorTree->execute(actor);
        });
    }

    engine.makeActor("TimescaleSetter").add<en::InlineBehavior>([](en::Actor& actor, float dt){

        const bool shouldIncrease = sf::Keyboard::isKeyPressed(sf::Keyboard::Period);
        const bool shouldDecrease = sf::Keyboard::isKeyPressed(sf::Keyboard::Comma);
        if (shouldIncrease ^ shouldDecrease) {

            constexpr float multiplierPerSecond = 2.f;

            en::Engine& engine = actor.getEngine();
            const float currentTimeScale = engine.getTimeScale();
            const float multiplier = glm::pow(multiplierPerSecond, engine.getDeltaTimeRealtime());
            const float newTimeScale = shouldIncrease ?
                currentTimeScale * multiplier :
                currentTimeScale / multiplier;

            const float newTimeScaleClamped = glm::clamp(newTimeScale, 0.01f, 20.f);
            engine.setTimeScale(newTimeScaleClamped);
        }
    });
}