//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef ENGINE_MOUSE_H
#define ENGINE_MOUSE_H

#include "glm.h"
#include "Window.h"
#include "Receiver.h"

namespace utils {

    class Mouse : en::Receiver<en::Window::MouseScroll> {

    public:
        static bool isDown(int button);
        static bool isHeld(int button);
        static bool isUp(int button);
        static glm::vec2 getPosition();
        static glm::vec2 getPosition(const en::Window& window);
        static float getScrollDelta();

        static void update();

    private:
        void receive(const en::Window::MouseScroll& info) override;
    };
}

#endif //ENGINE_MOUSE_H
