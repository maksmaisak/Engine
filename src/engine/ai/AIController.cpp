//
// Created by Maksym Maisak on 2019-09-28.
//

#include "AIController.h"
#include <cassert>
#include "Engine.h"
#include "Transform.h"
#include "Sprite.h"

using namespace ai;

AIController& AIController::create(class en::Engine& engine) {

    en::Actor actor = engine.makeActor("AI Actor");
    actor.add<en::Transform>();
    actor.add<en::Sprite>().pivot = glm::vec2(0.f);
    return actor.add<AIController>();
}

void AIController::start() {

}

void AIController::update(float dt) {

    if (m_behaviorTree) {
        m_behaviorTree->execute(getActor());
    }
}

BehaviorTree* AIController::getBehaviorTree() {
    return m_behaviorTree.get();
}

void AIController::setBehaviorTree(std::unique_ptr<BehaviorTree> behaviorTree) {
    m_behaviorTree = std::move(behaviorTree);
}
