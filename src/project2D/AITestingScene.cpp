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

namespace {

    const en::Name targetPositionName = "targetPosition";

    void findTarget(en::Actor& actor, ai::Blackboard* blackboard) {

        const auto* const transform = actor.tryGet<en::Transform>();
        if (!transform) {
            return;
        }

        const auto isObstacle = [&engine = actor.getEngine()](const en::GridPosition& pos) {
            return ai::Pathfinding::isObstacle(engine, pos);
        };

        ai::PathfindingParams params;
        params.allowObstacleGoal = true;
        const std::optional<ai::PathfindingPath> path = ai::Pathfinding::getPath(actor.getEngine(),
            transform->getWorldPosition(), isObstacle, ai::Pathfinding::nullHeuristic, params
        );

        if (path && !path->empty()) {
            blackboard->set<en::GridPosition>(targetPositionName, path->back());
        }
    }

    std::unique_ptr<ai::BehaviorTree> makeBehaviorTree() {

        using namespace ai;
        using std::make_unique;

        const auto hasValidTarget = [](en::Actor& actor, Blackboard* blackboard) {

            if (blackboard) {
                if (const auto targetPositionOptional = blackboard->get<en::GridPosition>(targetPositionName)) {
                    return Pathfinding::isObstacle(actor.getEngine(), *targetPositionOptional);
                }
            }

            return false;
        };

        const auto unsetTargetPosition = [](en::Actor& actor, Blackboard* blackboard) {

            if (blackboard) {
                blackboard->unset<en::GridPosition>(targetPositionName);
            }
        };

        const auto isOutsideShootingRange = [](en::Actor& actor, Blackboard* blackboard) {

            if (blackboard) {
                if (const auto targetPositionOptional = blackboard->get<en::GridPosition>(targetPositionName)) {
                    return 10.f * 10.f < glm::distance2(
                        glm::vec2(*targetPositionOptional),
                        glm::vec2(en::GridPosition(actor.get<en::Transform>().getWorldPosition()))
                    );
                }
            }

            return false;
        };

        auto behaviorTree = make_unique<BehaviorTree>(make_unique<Sequence>(
            make_unique<Selector>(
                make_unique<ConditionAction>(hasValidTarget),
                make_unique<InlineAction>(findTarget)
            ),
            make_unique<Selector>(
                make_unique<Sequence>(
                    make_unique<WhileDecorator>(
                        isOutsideShootingRange,
                        make_unique<MoveAction>(targetPositionName)
                    ),
                    make_unique<WhileDecorator>(
                        hasValidTarget,
                        make_unique<Sequence>(
                            make_unique<ShootAction>(targetPositionName),
                            make_unique<DelayAction>(0.1f)
                        )
                    ),
                    make_unique<InlineAction>(unsetTargetPosition)
                ),
                make_unique<InlineAction>(unsetTargetPosition)
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

    ai::AIController& aiController = ai::AIController::create(engine);
    aiController.setBehaviorTree(makeBehaviorTree());
}