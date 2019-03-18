//
// Created by Maksym Maisak on 2019-03-08.
//

#include "CompoundSystem.h"

using namespace en;

void CompoundSystem::start() {

    startNotStartedSystems();
}

void CompoundSystem::update(float dt) {

    startNotStartedSystems();

    for (auto& system : m_systems)
        system->update(dt);
}

void CompoundSystem::draw() {

    for (auto& system : m_systems)
        system->draw();
}

void CompoundSystem::startNotStartedSystems() {

    for (auto* system : m_notStarted)
        system->start();

    m_notStarted.clear();
}
