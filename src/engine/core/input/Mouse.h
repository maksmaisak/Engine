//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef ENGINE_MOUSE_H
#define ENGINE_MOUSE_H

#include "glm.h"

namespace en {

    class Mouse {

    public:
        static bool isDown(int button);
        static bool isHeld(int button);
        static bool isUp(int button);
        static glm::vec2 getPosition();
        static glm::vec2 getPosition(const class Window& window);
        static float getScrollDelta();

        static void update();
    };
}

#endif //ENGINE_MOUSE_H
