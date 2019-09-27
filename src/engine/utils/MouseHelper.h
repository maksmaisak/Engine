//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef ENGINE_MOUSEHELPER_H
#define ENGINE_MOUSEHELPER_H

#include <SFML/Window.hpp>
#include "glm.h"

#include "Receiver.h"

namespace utils {

    class MouseHelper : en::Receiver<sf::Event> {

    public:
        static bool isDown(sf::Mouse::Button button);
        static bool isHeld(sf::Mouse::Button button);
        static bool isUp(sf::Mouse::Button button);
        static glm::vec2 getPosition(const sf::Window& window);
        static float getScrollDelta();

        static void update();

    private:
        void receive(const sf::Event& info) override;
    };
}

#endif //ENGINE_MOUSEHELPER_H
