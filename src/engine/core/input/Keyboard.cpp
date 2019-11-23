//
// Created by Maksym Maisak on 2019-02-12.
//

#include "Keyboard.h"
#include <unordered_map>
#include <algorithm>
#include <vector>
#include "Receiver.h"
#include "Window.h"

using namespace en;

namespace {

    auto wasHeldLastUpdate = std::vector<bool>(GLFW_KEY_LAST + 1, false);
    auto isHeldNow         = std::vector<bool>(GLFW_KEY_LAST + 1, false);

    int getKeyCode(const std::string& keyName) {

        static std::unordered_map<std::string, Keyboard::keyCode_t> nameToKey = [](){

            std::unordered_map<std::string, Keyboard::keyCode_t> map {
                {"UP",     GLFW_KEY_UP    },
                {"DOWN",   GLFW_KEY_DOWN  },
                {"LEFT",   GLFW_KEY_LEFT  },
                {"RIGHT",  GLFW_KEY_RIGHT },
                {"ESCAPE", GLFW_KEY_ESCAPE}
            };

            for (Keyboard::keyCode_t i = 0; i <= GLFW_KEY_LAST; ++i) {

                if (const char* nameCString = glfwGetKeyName(i, 0)) {
                    std::string name(nameCString);
                    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                    map.emplace(name, i);
                }
            }

            return map;
        }();

        std::string uppercaseKeyName = keyName;
        std::transform(uppercaseKeyName.begin(), uppercaseKeyName.end(), uppercaseKeyName.begin(), ::toupper);

        const auto it = nameToKey.find(uppercaseKeyName);
        if (it == nameToKey.end()) {
            return GLFW_KEY_UNKNOWN;
        }

        return it->second;
    }

    bool isValid(Keyboard::keyCode_t keyCode) {
        return keyCode >= 0 && keyCode <= GLFW_KEY_LAST;
    }

    class KeyboardEventReceiver : Receiver<Window::KeyEvent> {

    private:
        inline void receive(const Window::KeyEvent& info) override {

            if (isValid(info.key)) {
                isHeldNow.at(info.key) = info.action != GLFW_RELEASE;
            }
        }
    } keyboardEventReceiver;
}

bool Keyboard::isHeld(keyCode_t keyCode) {

    return isValid(keyCode) ? isHeldNow[keyCode] : false;
}

bool Keyboard::isUp(keyCode_t keyCode) {

    if (!isValid(keyCode)) {
        return false;
    }

    return wasHeldLastUpdate[keyCode] && !isHeldNow[keyCode];
}

bool Keyboard::isDown(keyCode_t keyCode) {

    if (!isValid(keyCode)) {
        return false;
    }

    return !wasHeldLastUpdate[keyCode] && isHeldNow[keyCode];
}

bool Keyboard::isHeld(const std::string& keyName) {
    return isHeld(getKeyCode(keyName));
}

bool Keyboard::isUp(const std::string& keyName) {
    return isUp(getKeyCode(keyName));
}

bool Keyboard::isDown(const std::string& keyName) {
    return isDown(getKeyCode(keyName));
}

void Keyboard::update() {
    std::copy(isHeldNow.begin(), isHeldNow.end(), wasHeldLastUpdate.begin());
}
