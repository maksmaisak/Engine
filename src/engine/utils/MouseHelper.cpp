//
// Created by Maksym Maisak on 2019-02-15.
//

#include "MouseHelper.h"
#include <array>

using namespace utils;

namespace {

    auto wasMouseButtonPressed = std::array<bool, sf::Mouse::ButtonCount>();

    float previousScrollDelta = 0.f;
    float currentScrollDelta = 0.f;
}

bool MouseHelper::isDown(sf::Mouse::Button button) {
    return !wasMouseButtonPressed[button] && isHeld(button);
}

bool MouseHelper::isHeld(sf::Mouse::Button button) {
    return sf::Mouse::isButtonPressed(button);
}

bool MouseHelper::isUp(sf::Mouse::Button button) {
    return wasMouseButtonPressed[button] && !isHeld(button);
}

glm::vec2 MouseHelper::getPosition(const sf::Window& window) {

    sf::Vector2i temp = sf::Mouse::getPosition(window);
    return {temp.x, window.getSize().y - temp.y};
}

float MouseHelper::getScrollDelta() {

    return previousScrollDelta;
}

void MouseHelper::update() {

    for (std::size_t i = 0; i < wasMouseButtonPressed.size(); ++i) {
        wasMouseButtonPressed[i] = isHeld((sf::Mouse::Button)i);
    }

    previousScrollDelta = currentScrollDelta;
    currentScrollDelta = 0.f;
}

void MouseHelper::receive(const sf::Event& info) {

    if (info.type == sf::Event::EventType::MouseWheelScrolled) {
        currentScrollDelta += info.mouseWheelScroll.delta;
    }
}
