//
// Created by Maksym Maisak on 13/10/19.
//

#include "DelayAction.h"
#include "Engine.h"
#include "GameTime.h"

using namespace ai;

DelayAction::DelayAction(float duration) :
    m_duration(glm::max(0.f, duration))
{}

ActionOutcome DelayAction::execute() {

    // TODO add current tick number to avoid imprecision.
    // TODO make Scheduler return a handle through which it can be cancelled.
    if (!m_startTime) {
        m_startTime = GameTime::nowAsSeconds();
    }

    if (GameTime::nowAsSeconds() < *m_startTime + m_duration) {
        return ActionOutcome::InProgress;
    }

    return ActionOutcome::Success;
}

void DelayAction::reset() {

    m_startTime = std::nullopt;
}
