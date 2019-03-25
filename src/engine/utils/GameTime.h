//
// Created by Maksym Maisak on 14/10/18.
//

#ifndef SAXION_Y2Q1_CPP_TIME_H
#define SAXION_Y2Q1_CPP_TIME_H

#include <chrono>
#include <SFML/System.hpp>

class GameTime {

public:

    using clock = std::chrono::high_resolution_clock;
    using timePoint = std::chrono::time_point<clock>;
    using duration  = clock::duration;

    static duration now();
    static float nowAsSeconds();

    // Legacy, use now() instead.
    static sf::Time nowSFTime();

private:
    static const timePoint m_start;
};

#endif //SAXION_Y2Q1_CPP_TIME_H
