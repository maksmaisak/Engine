//
// Created by Maksym Maisak on 27/9/18.
//

#include "Input.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine.h"

namespace en {

    glm::vec2 getMoveInput() {

        return {
            getAxisHorizontal(),
            getAxisVertical()
        };
    }

    float getAxisHorizontal() {

        constexpr int rightKeys[] { GLFW_KEY_D, GLFW_KEY_RIGHT };
        constexpr int leftKeys[]  { GLFW_KEY_A, GLFW_KEY_LEFT };

        const auto isKeyPressed = [window = Engine::get().getWindow().getUnderlyingWindow()](int keyCode) {
            return glfwGetKey(window, keyCode);
        };

        float result = 0.f;

        if (std::any_of(std::begin(rightKeys), std::end(rightKeys), isKeyPressed)) {
            result += 1.f;
        }

        if (std::any_of(std::begin(leftKeys), std::end(leftKeys), isKeyPressed)) {
            result -= 1.f;
        }

        return result;
    }

    float getAxisVertical() {

        constexpr int forwardKeys[] { GLFW_KEY_W, GLFW_KEY_UP };
        constexpr int backKeys[]    { GLFW_KEY_S, GLFW_KEY_DOWN };

        const auto isKeyPressed = [window = Engine::get().getWindow().getUnderlyingWindow()](int keyCode) {
            return glfwGetKey(window, keyCode);
        };

        float result = 0.f;

        if (std::any_of(std::begin(forwardKeys), std::end(forwardKeys), isKeyPressed)) {
            result += 1.f;
        }

        if (std::any_of(std::begin(backKeys), std::end(backKeys), isKeyPressed)) {
            result -= 1.f;
        }

        return result;
    }
}
