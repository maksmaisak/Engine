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

    Behavior::update(dt);

    if (m_actionQueue.empty()) {
        return;
    }

    const ActionOutcome outcome = m_actionQueue.front()->execute();
    switch (outcome) {
        case ActionOutcome::InProgress:
            break;
        case ActionOutcome::Success:
            m_actionQueue.pop();
            break;
        case ActionOutcome::Fail:
            m_actionQueue = {};
            std::cout << "Action failed." << std::endl;
            break;
        default:
            assert(false);
    }
}

AIController& AIController::enqueueAction(std::unique_ptr<Action>&& action) {

    action->actor = getActor();
    m_actionQueue.push(std::move(action));

    return *this;
}
