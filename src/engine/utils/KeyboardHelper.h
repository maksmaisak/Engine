//
// Created by Maksym Maisak on 2019-02-12.
//

#ifndef SAXION_Y2Q2_RENDERING_KEYBOARDHELPER_H
#define SAXION_Y2Q2_RENDERING_KEYBOARDHELPER_H

#include <string>
#include <SFML/Window.hpp>
#include "Receiver.h"

namespace utils {

    class KeyboardHelper : en::Receiver<sf::Event> {

    public:
        static bool isDown(const std::string& keyName);
        static bool isHeld(const std::string& keyName);
        static bool isUp(const std::string& keyName);
        static void update();

    private:
        void receive(const sf::Event& info) override;
    };
}

#endif //SAXION_Y2Q2_RENDERING_KEYBOARDHELPER_H
