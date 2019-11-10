//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef ENGINE_MOUSEHELPER_H
#define ENGINE_MOUSEHELPER_H

#include "glm.h"
#include "Window.h"
#include "Receiver.h"

namespace utils {

    class MouseHelper : en::Receiver<en::Window::MouseScroll> {

    public:
        static bool isDown(int button);
        static bool isHeld(int button);
        static bool isUp(int button);
        static glm::vec2 getPosition(const en::Window& window);
        static float getScrollDelta();

        static void update();

    private:
        void receive(const en::Window::MouseScroll& info) override;
    };
}

#endif //ENGINE_MOUSEHELPER_H
