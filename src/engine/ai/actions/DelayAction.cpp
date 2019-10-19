//
// Created by Maksym Maisak on 13/10/19.
//

#include "DelayAction.h"
#include <limits>
#include "Engine.h"
#include "GameTime.h"

using namespace ai;

DelayAction::DelayAction(float duration) :
    m_duration(glm::max(std::numeric_limits<float>::epsilon(), duration))
{}

ActionOutcome DelayAction::execute() {

    if (!m_timer.isAssigned()) {
        m_timer = m_actor.getEngine().getScheduler().delay(sf::seconds(m_duration));
    }

    if (m_timer.isInProgress()) {
        return ActionOutcome::InProgress;
    }

    return ActionOutcome::Success;
}

void DelayAction::reset() {

    m_timer.cancel();
    m_timer = {};
}
