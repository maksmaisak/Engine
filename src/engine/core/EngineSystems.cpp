//
// Created by Maksym Maisak on 30/9/19.
//

#include "EngineSystems.h"

using namespace en;

EngineSystems::EngineSystems(Engine& engine) :
    m_behaviors(nullptr)
{
    m_systems.init(engine);
}

void EngineSystems::start() {

    m_systems.start();
}

void EngineSystems::update(float dt) {

    m_systems.update(dt);
}

void EngineSystems::draw() {

    m_systems.draw();
}