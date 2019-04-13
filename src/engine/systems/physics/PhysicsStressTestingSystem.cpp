//
// Created by Maksym Maisak on 2019-03-25.
//

#include "PhysicsStressTestingSystem.h"
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include "Engine.h"
#include "PhysicsTestScene.h"
#include "PhysicsSystem.h"
#include "PhysicsSystemBoundingSphereNarrowphase.h"
#include "PhysicsSystemFlatGrid.h"
#include "PhysicsSystemOctree.h"
#include "GameTime.h"

using namespace en;

namespace {

    struct SystemConfig {
        std::string name;
        std::function<std::unique_ptr<PhysicsSystemBase>(Engine&)> makeSystem;
    };

    const std::vector<SystemConfig> systemConfigs {
        {"No optimizations"          , [](Engine& engine){return engine.makeSystem<PhysicsSystem>();}},
        {"Bounding sphere pre-checks", [](Engine& engine){return engine.makeSystem<PhysicsSystemBoundingSphereNarrowphase>();}},
        {"Flat grid"                 , [](Engine& engine){return engine.makeSystem<PhysicsSystemFlatGrid>();}},
        {"Octree"                    , [](Engine& engine){return engine.makeSystem<PhysicsSystemOctree>();}}
    };

    const std::vector<PhysicsTestScene::Preset> scenePresets {
        {100 , 100 },
        {200 , 200 },
        {400 , 200 },
        {400 , 400 },
        {400 , 1000}
    };

    std::string generateOutputFilepath() {

        std::stringstream s;
        s << "output/test_";

        const std::time_t t = std::time(nullptr);
        char dateTimeStr[100];
        if (std::strftime(dateTimeStr, sizeof(dateTimeStr), "%Y_%m_%d_%H_%M_%S", std::localtime(&t)))
            s << dateTimeStr;

        s << ".csv";

        return s.str();
    }
}

PhysicsStressTestingSystem::PhysicsStressTestingSystem(const std::chrono::milliseconds& testDuration) :
    m_testDuration(testDuration),
    m_outputFilepath(generateOutputFilepath())
{}

void PhysicsStressTestingSystem::start() {

    writeDiagnosticsHeader();
    startTest();
}

void PhysicsStressTestingSystem::update(float dt) {

    if (m_physicsSystem)
        m_physicsSystem->update(dt);

    if (m_isDone)
        return;

    if (GameTime::now() < m_timeForNextScene)
        return;

    outputDiagnosticsData();

    m_currentSystemConfigIndex += 1;
    if (m_currentSystemConfigIndex >= systemConfigs.size()) {
        m_currentSystemConfigIndex = 0;
        m_currentTestSceneIndex += 1;
    }

    if (m_currentTestSceneIndex >= scenePresets.size()) {
        m_isDone = true;
        return;
    }

    startTest();
}

void PhysicsStressTestingSystem::draw() {

    if (m_physicsSystem)
        m_physicsSystem->draw();
}

void PhysicsStressTestingSystem::startTest() {

    const auto& preset = scenePresets[m_currentTestSceneIndex];
    m_engine->getSceneManager().setCurrentSceneNextUpdate<PhysicsTestScene>(preset);

    m_physicsSystem = systemConfigs[m_currentSystemConfigIndex].makeSystem(*m_engine);
    m_physicsSystem->setGravity({0, -9.8f, 0});
    m_physicsSystem->start();

    m_timeForNextScene = GameTime::now() + m_testDuration;
}

void PhysicsStressTestingSystem::writeDiagnosticsHeader() {

    std::ofstream out(m_outputFilepath, std::ios_base::app);
    if (!out.is_open())
        return;

    out << "test name,static bodies,dynamic bodies,min update time,avg update time,max update time\n";
    out.close();
}

void PhysicsStressTestingSystem::outputDiagnosticsData() {

    if (!m_physicsSystem)
        return;

    PhysicsSystem::DiagnosticsInfo info = m_physicsSystem->resetDiagnosticsInfo();

    using namespace std::chrono;
    using ms = duration<double, std::milli>;

    std::ofstream out(m_outputFilepath, std::ios_base::app);
    if (!out.is_open())
        return;

    // Ensure dot separator for floats.
    out.imbue(std::locale::classic());
    out <<
        systemConfigs[m_currentSystemConfigIndex].name << ',' <<
        scenePresets[m_currentTestSceneIndex].numBodiesStatic  << ',' <<
        scenePresets[m_currentTestSceneIndex].numBodiesDynamic << ',' <<
        duration_cast<ms>(info.updateTimeMin          ).count() << "ms," <<
        duration_cast<ms>(info.updateTimeAverage.get()).count() << "ms," <<
        duration_cast<ms>(info.updateTimeMax          ).count() << "ms,";

    for (const auto& updateTime : info.updateTimes)
        out << duration_cast<ms>(updateTime).count() << ",";
    out << std::endl;

    out.close();
}
