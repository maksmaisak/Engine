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
#include "GameTime.h"

using namespace en;

namespace {

    const std::vector<PhysicsTestScene::Preset> testPresets = {
        {100 , 100},
        {200 , 200},
        {400 , 200},
        //{400 , 400}
        //{1000, 1000}
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

PhysicsStressTestingSystem::PhysicsStressTestingSystem() : m_outputFilepath(generateOutputFilepath()) {}

void PhysicsStressTestingSystem::start() {

    writeDiagnosticsHeader();
    startNextTest();
}

void PhysicsStressTestingSystem::update(float dt) {

    if (m_physicsSystem)
        m_physicsSystem->update(dt);

    if (m_isDone)
        return;

    if (GameTime::now() >= m_timeForNextScene) {

        outputDiagnosticsData();

        if (m_nextTestIndex < testPresets.size())
            startNextTest();
        else
            m_isDone = true;
    }
}

void PhysicsStressTestingSystem::startNextTest() {

    const auto& preset = testPresets[m_nextTestIndex++];
    m_engine->getSceneManager().setCurrentSceneNextUpdate<PhysicsTestScene>(preset);

    m_physicsSystem = m_engine->makeSystem<PhysicsSystem>();
    m_physicsSystem->setGravity({0, -9.8f, 0});
    m_physicsSystem->start();

    m_timeForNextScene = GameTime::now() + m_testDuration;
}

void PhysicsStressTestingSystem::writeDiagnosticsHeader() {

    std::ofstream out(m_outputFilepath, std::ios_base::app);
    if (!out.is_open())
        return;

    out << "test,static bodies,dynamic bodies,min update time,avg update time,max update time\n";
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
        m_nextTestIndex << ',' <<
        testPresets[m_nextTestIndex - 1].numBodiesStatic  << ',' <<
        testPresets[m_nextTestIndex - 1].numBodiesDynamic << ',' <<
        duration_cast<ms>(info.updateTimeMin          ).count() << "ms," <<
        duration_cast<ms>(info.updateTimeAverage.get()).count() << "ms," <<
        duration_cast<ms>(info.updateTimeMax          ).count() << "ms\n";

    out.close();
}
