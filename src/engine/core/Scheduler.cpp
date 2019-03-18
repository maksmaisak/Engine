#include <utility>

//
// Created by Maksym Maisak on 4/11/18.
//

#include "Scheduler.h"
#include "GameTime.h"

void Scheduler::update(float dt) {

    while (!m_scheduled.empty() && m_scheduled.top().time <= GameTime::now()) {

        m_scheduled.top().func();
        m_scheduled.pop();
    }
}

void Scheduler::delay(sf::Time timeDelay, const Scheduler::func_t& func) {

    schedule(GameTime::now() + timeDelay, func);
}

void Scheduler::schedule(sf::Time time, const Scheduler::func_t& func) {

    m_scheduled.emplace(time, func);
}
