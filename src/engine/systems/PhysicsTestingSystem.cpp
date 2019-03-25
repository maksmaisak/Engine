//
// Created by Maksym Maisak on 2019-03-25.
//

#include "PhysicsTestingSystem.h"
#include <vector>
#include <chrono>
#include "Engine.h"
#include "PhysicsTestScene.h"
#include "GameTime.h"

using namespace en;

namespace {

    const std::vector<PhysicsTestScene::Preset> testPresets = {
        {100 , 100 },
        {200 , 200 },
        {500 , 500 }
        //{1000, 1000}
    };
}

void PhysicsTestingSystem::start() {

    startNextTest();
}

void PhysicsTestingSystem::update(float dt) {

    if (m_nextTestIndex >= testPresets.size())
        return;

    if (GameTime::now() < m_timeForNextScene)
        return;

    startNextTest();
}

void PhysicsTestingSystem::startNextTest() {

    const auto& preset = testPresets[m_nextTestIndex++];
    m_engine->getSceneManager().setCurrentSceneNextUpdate<PhysicsTestScene>(preset);
    m_timeForNextScene = GameTime::now() + m_testDuration;
}
