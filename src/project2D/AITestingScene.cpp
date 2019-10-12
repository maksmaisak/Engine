//
// Created by Maksym Maisak on 5/10/19.
//

#include "AITestingScene.h"
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

void AITestingScene::open() {

    en::Engine& engine = getEngine();

    engine.makeActor("layerDefault").add<en::TileLayer>();

    en::Actor cameraActor = engine.makeActor("Test camera");
    cameraActor.add<en::Camera>().isOrthographic = true;
    cameraActor.add<en::Transform>().move({0, 0, 10.f});

    using namespace ai;

    const en::Name targetPositionName = "targetPosition";

    static const auto isFarFromTarget = [targetPositionName](en::Actor& actor, Blackboard* blackboard) -> bool {

        if (const auto* transform = actor.tryGet<en::Transform>()) {
            if (blackboard) {
                if (const auto targetPositionOptional = blackboard->get<en::GridPosition>(targetPositionName)) {
                    return 2.f * 2.f <= glm::distance2(glm::vec2(transform->getWorldPosition()), glm::vec2(*targetPositionOptional));
                }
            }
        }

        return true;
    };

    using std::make_unique;
    AIController& aiController = AIController::create(engine);
    aiController.setBehaviorTree(make_unique<BehaviorTree>(make_unique<Selector>(
        make_unique<ConditionDecorator>(
            isFarFromTarget,
            std::make_unique<MoveAction>(targetPositionName)
        ),
        make_unique<InlineAction>([targetPositionName](en::Actor& actor, Blackboard* blackboard) {
            if (blackboard) {
                if (const auto targetPosition = blackboard->get<en::GridPosition>(targetPositionName)) {
                    blackboard->set(targetPositionName, *targetPosition + en::GridPosition(5, 5));
                } else {
                    blackboard->set(targetPositionName, en::GridPosition(12, 10));
                }
            }
        })
        //make_unique<InlineAction>([targetPosition](en::Actor& actor){removeItemAt(actor.getEngine(), targetPosition);}),
        //make_unique<ShootAction>(glm::vec2(20.5f, 10.5f)),
    )));
}