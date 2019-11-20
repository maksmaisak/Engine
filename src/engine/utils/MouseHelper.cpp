//
// Created by Maksym Maisak on 2019-02-15.
//

#include "MouseHelper.h"
#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine.h"

using namespace utils;

namespace {

    constexpr std::size_t numMouseButtons = GLFW_MOUSE_BUTTON_LAST + 1;
    auto wasMouseButtonPressed = std::array<bool, numMouseButtons>();
    constexpr bool isValidMouseButton(int button) {
        return button >= 0 && button < numMouseButtons;
    }

    float previousScrollDelta = 0.f;
    float currentScrollDelta = 0.f;
}

bool MouseHelper::isDown(int button) {

    return isValidMouseButton(button) && !wasMouseButtonPressed[button] && isHeld(button);
}

bool MouseHelper::isHeld(int button) {

    return isValidMouseButton(button) && glfwGetMouseButton(en::Engine::get().getWindow().getUnderlyingWindow(), button);
}

bool MouseHelper::isUp(int button) {

    return isValidMouseButton(button) && wasMouseButtonPressed[button] && !isHeld(button);
}

glm::vec2 MouseHelper::getPosition(const en::Window& window) {

    double posX, posY;
    glfwGetCursorPos(window.getUnderlyingWindow(), &posX, &posY);
    return {posX, window.getSize().y - posY};
}

float MouseHelper::getScrollDelta() {

    return previousScrollDelta;
}

void MouseHelper::update() {

    for (int i = 0; i < numMouseButtons; ++i) {
        wasMouseButtonPressed[i] = isHeld(i);
    }

    previousScrollDelta = currentScrollDelta;
    currentScrollDelta = 0.f;
}

void MouseHelper::receive(const en::Window::MouseScroll& info) {

    currentScrollDelta += static_cast<float>(info.offsetY);
}
