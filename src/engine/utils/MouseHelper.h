//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef SAXION_Y2Q2_RENDERING_MOUSEHELPER_H
#define SAXION_Y2Q2_RENDERING_MOUSEHELPER_H

#include <SFML/Window.hpp>
#include "glm.hpp"

namespace utils {
    class MouseHelper {

    public:
        static bool isDown(sf::Mouse::Button button);
        static bool isHeld(sf::Mouse::Button button);
        static bool isUp(sf::Mouse::Button button);
        static glm::vec2 getPosition(const sf::Window& window);

        static void update();
    };
}

#endif //SAXION_Y2Q2_RENDERING_MOUSEHELPER_H
