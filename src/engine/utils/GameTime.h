//
// Created by Maksym Maisak on 14/10/18.
//

#ifndef SAXION_Y2Q1_CPP_TIME_H
#define SAXION_Y2Q1_CPP_TIME_H

#include <SFML/Graphics.hpp>

class GameTime {

public:
    static sf::Time now();

private:
    static const sf::Clock m_clock;
};

#endif //SAXION_Y2Q1_CPP_TIME_H
