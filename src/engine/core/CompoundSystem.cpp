//
// Created by Maksym Maisak on 2019-03-08.
//

#include "CompoundSystem.h"

using namespace en;

void CompoundSystem::update(float dt) {

    appendNewSystems();

    for (std::unique_ptr<System>& system : m_systems) {
        if (system) {
            system->update(dt);
        }
    }
}

void CompoundSystem::draw() {

    appendNewSystems();

    for (std::unique_ptr<System>& system : m_systems) {
        if (system) {
            system->draw();
        }
    }
}

void CompoundSystem::appendNewSystems() {

    for (std::unique_ptr<System>& system : m_newSystems) {
        m_systems.push_back(std::move(system));
    }

    m_newSystems.clear();
}
