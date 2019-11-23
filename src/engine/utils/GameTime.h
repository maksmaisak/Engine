//
// Created by Maksym Maisak on 14/10/18.
//

#ifndef ENGINE_TIME_H
#define ENGINE_TIME_H

#include <chrono>

namespace en {

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = Clock::duration;
    using DurationFloat = std::chrono::duration<float>;

    class GameTime {

    public:
        static Duration sinceAppStart();
        static float asSeconds(const Duration& duration);
    };
}

#endif //ENGINE_TIME_H
