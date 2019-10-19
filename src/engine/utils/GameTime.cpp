//
// Created by Maksym Maisak on 14/10/18.
//

#include "GameTime.h"

using namespace std::chrono;

const GameTime::timePoint GameTime::m_start = std::chrono::high_resolution_clock::now();

// TODO add current tick number support to avoid imprecision

GameTime::duration GameTime::now() {

    return std::chrono::high_resolution_clock::now() - m_start;
}

sf::Time GameTime::nowSFTime() {

    return sf::microseconds(duration_cast<std::chrono::duration<sf::Uint64, std::micro>>(now()).count());
}

float GameTime::nowAsSeconds() {

    return duration_cast<std::chrono::duration<float>>(now()).count();
}
