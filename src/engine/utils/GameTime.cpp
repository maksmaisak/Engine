//
// Created by Maksym Maisak on 14/10/18.
//

#include "GameTime.h"

using namespace en;

namespace {
    const TimePoint applicationStart = Clock::now();
}

// TODO add current tick number support to avoid imprecision

Duration GameTime::sinceAppStart() {
    return Clock::now() - applicationStart;
}

float GameTime::asSeconds(const Duration& duration) {
    return std::chrono::duration_cast<DurationFloat>(duration).count();
}
