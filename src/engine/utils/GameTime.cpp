//
// Created by Maksym Maisak on 14/10/18.
//

#include "GameTime.h"

const sf::Clock GameTime::m_clock;

sf::Time GameTime::now() {
    return m_clock.getElapsedTime();
}