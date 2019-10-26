//
// Created by Maksym Maisak on 2019-02-12.
//

#ifndef ENGINE_KEYBOARDHELPER_H
#define ENGINE_KEYBOARDHELPER_H

#include <string>
#include <SFML/Window.hpp>
#include "Receiver.h"

namespace utils {

    class KeyboardHelper : en::Receiver<sf::Event> {

    public:
        static bool isDown(sf::Keyboard::Key keyCode);
        static bool isHeld(sf::Keyboard::Key keyCode);
        static bool isUp(sf::Keyboard::Key keyCode);

        static bool isDown(const std::string& keyName);
        static bool isHeld(const std::string& keyName);
        static bool isUp(const std::string& keyName);

        static void update();

    private:
        void receive(const sf::Event& info) override;
    };
}

#endif //ENGINE_KEYBOARDHELPER_H
