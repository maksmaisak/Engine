//
// Created by Maksym Maisak on 2019-02-12.
//

#ifndef ENGINE_KEYBOARD_H
#define ENGINE_KEYBOARD_H

#include "Name.h"

namespace en {

    class Keyboard {

    public:
        using keyCode_t = int;

        static bool isDown(keyCode_t keyCode);
        static bool isHeld(keyCode_t keyCode);
        static bool isUp(keyCode_t keyCode);

        static bool isDown(const Name& keyName);
        static bool isHeld(const Name& keyName);
        static bool isUp(const Name& keyName);

        static void update();
    };
}

#endif //ENGINE_KEYBOARD_H
