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
#include "ConditionDecorator.h"
#include "InlineAction.h"
#include "Sequence.h"
#include "Selector.h"
#include "MoveAction.h"
#include "ShootAction.h"
#include "ParallelAction.h"
#include "SimpleParallelAction.h"
#include "RepeatDecorator.h"
#include "DelayAction.h"

namespace {

    en::GridPosition findClosestItemPosition(en::Actor& actor) {

        std::default_random_engine randomEngine(std::random_device{}());
        std::uniform_int_distribution<> distribution(-20, 20);

        if (auto* transform = actor.tryGet<en::Transform>()) {
            return
                en::GridPosition(distribution(randomEngine), distribution(randomEngine)) +
                en::GridPosition(transform->getWorldPosition());
        }

        return {};
    }

    std::unique_ptr<ai::BehaviorTree> makeBehaviorTree() {

        using namespace ai;
        using std::make_unique;

        const en::Name targetPositionName = "targetPosition";
        const en::Name shootingTargetName = "shootingTarget";

        const auto isNotAtTarget = [targetPositionName](en::Actor& actor, Blackboard* blackboard) -> bool {

            if (const auto* transform = actor.tryGet<en::Transform>()) {
                if (blackboard) {
                    if (const auto targetPositionOptional = blackboard->get<en::GridPosition>(targetPositionName)) {
                        return *targetPositionOptional != en::GridPosition(transform->getWorldPosition());
                    }
                }
            }

            return true;
        };

        const auto unsetTargetPosition = [targetPositionName](en::Actor& actor, Blackboard* blackboard) {
            if (blackboard) {
                blackboard->unset<en::GridPosition>(targetPositionName);
            }
        };

        auto behaviorTree = make_unique<BehaviorTree>(make_unique<Sequence>(
            make_unique<InlineAction>([targetPositionName](en::Actor& actor, Blackboard* blackboard) {
                if (blackboard && !blackboard->get<en::GridPosition>(targetPositionName)) {
                    blackboard->set<en::GridPosition>(targetPositionName, findClosestItemPosition(actor));
                }
            }),
            make_unique<Selector>(
                make_unique<SimpleParallelAction>(
                    make_unique<MoveAction>(targetPositionName),
                    make_unique<Sequence>(
                        make_unique<DelayAction>(0.5f),
                        make_unique<ShootAction>(shootingTargetName)
                    )
                ),
                make_unique<InlineAction>(unsetTargetPosition)
            ),
            make_unique<InlineAction>(unsetTargetPosition)
        ));

        behaviorTree->getBlackboard().set<en::GridPosition>(shootingTargetName, {0, 0});

        return behaviorTree;
    }
}

void AITestingScene::open() {

    en::Engine& engine = getEngine();

    engine.makeActor("layerDefault").add<en::TileLayer>();

    en::Actor cameraActor = engine.makeActor("Test camera");
    cameraActor.add<en::Camera>().isOrthographic = true;
    cameraActor.add<en::Transform>().move({0, 0, 10.f});

    using namespace ai;
    AIController& aiController = AIController::create(engine);

    aiController.setBehaviorTree(makeBehaviorTree());
}