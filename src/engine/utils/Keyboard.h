//
// Created by Maksym Maisak on 2019-02-12.
//

#ifndef ENGINE_KEYBOARD_H
#define ENGINE_KEYBOARD_H

#include <string>
#include "Receiver.h"
#include "Window.h"

namespace utils {

    class Keyboard : en::Receiver<en::Window::KeyEvent> {

    public:
        using keyCode_t = int;

        static bool isDown(keyCode_t keyCode);
        static bool isHeld(keyCode_t keyCode);
        static bool isUp(keyCode_t keyCode);

        static bool isDown(const std::string& keyName);
        static bool isHeld(const std::string& keyName);
        static bool isUp(const std::string& keyName);

        static void update();

    private:
        void receive(const en::Window::KeyEvent& info) override;
    };
}

#endif //ENGINE_KEYBOARD_H
