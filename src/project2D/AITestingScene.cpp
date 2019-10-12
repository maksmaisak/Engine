//
// Created by Maksym Maisak on 5/10/19.
//

#include "AITestingScene.h"
#include "AIController.h"
#include "Camera.h"
#include "Transform.h"
#include "TileLayer.h"

#include "InlineBehavior.h"

#include "ConditionAction.h"
#include "Sequence.h"
#include "Selector.h"
#include "MoveAction.h"
#include "ShootAction.h"

void createPickupItems() {
    
    /*
    en::Entity entity = getEngine().getRegistry().with<en::TileLayer>().tryGetOne();
    if (!entity) {
        return;
    }

    for (int y = -100; y <= 100; ++y) {
        for (int x = -100; x <= 100; ++x) {

        }
    }*/
}

void AITestingScene::open() {

    en::Engine& engine = getEngine();

    engine.makeActor("layer default").add<en::TileLayer>();
    engine.makeActor("layer items").add<en::TileLayer>();

    en::Actor cameraActor = engine.makeActor("Test camera");
    cameraActor.add<en::Camera>().isOrthographic = true;
    cameraActor.add<en::Transform>().move({0, 0, 10.f});

    using namespace ai;
    using std::make_unique;
    AIController& aiController = AIController::create(engine);
    aiController.setBehaviorTree(make_unique<BehaviorTree>(make_unique<Sequence>(
        make_unique<MoveAction>(glm::i64vec2(12, 5)),
        make_unique<ShootAction>(glm::vec2(20.5f, 10.5f)),
        make_unique<MoveAction>(glm::i64vec2(12, 10)),
        make_unique<MoveAction>(glm::i64vec2(0 , 10))
    )));
    const en::Actor aiControllerActor = aiController.getActor();
    const glm::i64vec2 targetPosition {12, 5};
    static const auto isCloseToTarget = [targetPosition](en::Actor& actor) -> bool {

        const auto* transform = actor.tryGet<en::Transform>();
        if (!transform) {
            return false;
        }

        return 10.f * 10.f >=
            glm::distance2(glm::vec2(transform->getWorldPosition()), glm::vec2(targetPosition));
    };

    const auto behaviorTreeRoot = make_unique<Sequence>(
        make_unique<Selector>(
            std::make_unique<ConditionAction>(isCloseToTarget),
            std::make_unique<MoveAction>(targetPosition)
        ),
        make_unique<ShootAction>(glm::vec2(20.5f, 10.5f))
    );

    engine.makeActor("Test").add<en::InlineBehavior>([](en::Actor& actor, float dt){

    });
}