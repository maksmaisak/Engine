//
// Created by Maksym Maisak on 2019-03-25.
//

#ifndef ENGINE_PHYSICSTESTINGSYSTEM_H
#define ENGINE_PHYSICSTESTINGSYSTEM_H

#include "System.h"
#include <chrono>
#include "PhysicsSystemBase.h"

namespace en {

    using namespace std::literals::chrono_literals;

    class PhysicsStressTestingSystem : public System {

    public:
        PhysicsStressTestingSystem(const std::chrono::milliseconds& testDuration = 10s);
        void start() override;
        void update(float dt) override;
        void draw() override;

    private:
        void startTest();
        void writeDiagnosticsHeader();
        void outputDiagnosticsData();

        std::size_t m_currentSystemConfigIndex = 0;
        std::size_t m_currentTestSceneIndex    = 0;
        std::size_t m_numUpdatesInCurrentTest  = 0;
        std::size_t m_numUpdatesPerTest        = 0;
        bool m_isDone = false;

        std::string m_outputFilepath;

        std::unique_ptr<PhysicsSystemBase> m_physicsSystem;
    };
}

#endif //ENGINE_PHYSICSTESTINGSYSTEM_H
