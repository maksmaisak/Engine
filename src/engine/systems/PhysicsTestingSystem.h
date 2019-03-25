//
// Created by Maksym Maisak on 2019-03-25.
//

#ifndef ENGINE_PHYSICSTESTINGSYSTEM_H
#define ENGINE_PHYSICSTESTINGSYSTEM_H

#include "System.h"
#include <chrono>

namespace en {

    using namespace std::literals::chrono_literals;

    class PhysicsTestingSystem : public System {

    public:
        void start() override;
        void update(float dt) override;

    private:
        void startNextTest();

        std::chrono::milliseconds m_testDuration = 5s;
        std::size_t m_nextTestIndex = 0;
        std::chrono::high_resolution_clock::duration m_timeForNextScene;
    };
}

#endif //ENGINE_PHYSICSTESTINGSYSTEM_H
