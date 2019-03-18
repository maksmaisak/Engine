//
// Created by Maksym Maisak on 2019-02-12.
//

#include "KeyboardHelper.h"
#include <map>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>

using namespace utils;

namespace {

    std::vector<std::string> names = {
        "A",            ///< The A key
        "B",            ///< The B key
        "C",            ///< The C key
        "D",            ///< The D key
        "E",            ///< The E key
        "F",            ///< The F key
        "G",            ///< The G key
        "H",            ///< The H key
        "I",            ///< The I key
        "J",            ///< The J key
        "K",            ///< The K key
        "L",            ///< The L key
        "M",            ///< The M key
        "N",            ///< The N key
        "O",            ///< The O key
        "P",            ///< The P key
        "Q",            ///< The Q key
        "R",            ///< The R key
        "S",            ///< The S key
        "T",            ///< The T key
        "U",            ///< The U key
        "V",            ///< The V key
        "W",            ///< The W key
        "X",            ///< The X key
        "Y",            ///< The Y key
        "Z",            ///< The Z key
        "Num0",         ///< The 0 key
        "Num1",         ///< The 1 key
        "Num2",         ///< The 2 key
        "Num3",         ///< The 3 key
        "Num4",         ///< The 4 key
        "Num5",         ///< The 5 key
        "Num6",         ///< The 6 key
        "Num7",         ///< The 7 key
        "Num8",         ///< The 8 key
        "Num9",         ///< The 9 key
        "Escape",       ///< The Escape key
        "LControl",     ///< The left Control key
        "LShift",       ///< The left Shift key
        "LAlt",         ///< The left Alt key
        "LSystem",      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        "RControl",     ///< The right Control key
        "RShift",       ///< The right Shift key
        "RAlt",         ///< The right Alt key
        "RSystem",      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        "Menu",         ///< The Menu key
        "LBracket",     ///< The [ key
        "RBracket",     ///< The ] key
        "Semicolon",    ///< The ; key
        "Comma",        ///< The , key
        "Period",       ///< The . key
        "Quote",        ///< The ' key
        "Slash",        ///< The / key
        "Backslash",    ///< The \ key
        "Tilde",        ///< The ~ key
        "Equal",        ///< The = key
        "Hyphen",       ///< The - key (hyphen)
        "Space",        ///< The Space key
        "Enter",        ///< The Enter/Return keys
        "Backspace",    ///< The Backspace key
        "Tab",          ///< The Tabulation key
        "PageUp",       ///< The Page up key
        "PageDown",     ///< The Page down key
        "End",          ///< The End key
        "Home",         ///< The Home key
        "Insert",       ///< The Insert key
        "Delete",       ///< The Delete key
        "Add",          ///< The + key
        "Subtract",     ///< The - key (minus, usually from numpad)
        "Multiply",     ///< The * key
        "Divide",       ///< The / key
        "Left",         ///< Left arrow
        "Right",        ///< Right arrow
        "Up",           ///< Up arrow
        "Down",         ///< Down arrow
        "Numpad0",      ///< The numpad 0 key
        "Numpad1",      ///< The numpad 1 key
        "Numpad2",      ///< The numpad 2 key
        "Numpad3",      ///< The numpad 3 key
        "Numpad4",      ///< The numpad 4 key
        "Numpad5",      ///< The numpad 5 key
        "Numpad6",      ///< The numpad 6 key
        "Numpad7",      ///< The numpad 7 key
        "Numpad8",      ///< The numpad 8 key
        "Numpad9",      ///< The numpad 9 key
        "F1",           ///< The F1 key
        "F2",           ///< The F2 key
        "F3",           ///< The F3 key
        "F4",           ///< The F4 key
        "F5",           ///< The F5 key
        "F6",           ///< The F6 key
        "F7",           ///< The F7 key
        "F8",           ///< The F8 key
        "F9",           ///< The F9 key
        "F10",          ///< The F10 key
        "F11",          ///< The F11 key
        "F12",          ///< The F12 key
        "F13",          ///< The F13 key
        "F14",          ///< The F14 key
        "F15",          ///< The F15 key
        "Pause"         ///< The Pause key
    };

    class NameToKeyMap : public std::map<std::string, sf::Keyboard::Key> {

    public:
        NameToKeyMap() {

            for (std::size_t i = 0; i < names.size(); ++i) {
                std::string name = names[i];
                std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                emplace(name, (sf::Keyboard::Key) i);
            }
        }
    } nameToKey;

    auto wasHeldLastUpdate = std::vector<bool>(sf::Keyboard::KeyCount, false);
    auto isHeldNow         = std::vector<bool>(sf::Keyboard::KeyCount, false);

    sf::Keyboard::Key getKeyCode(const std::string& keyName) {

        std::string uppercaseKeyName = keyName;
        std::transform(uppercaseKeyName.begin(), uppercaseKeyName.end(), uppercaseKeyName.begin(), ::toupper);

        auto it = nameToKey.find(uppercaseKeyName);
        if (it == nameToKey.end())
            return sf::Keyboard::Key::Unknown;

        return it->second;
    }

    bool isValid(sf::Keyboard::Key keyCode) {
        return keyCode > -1 && keyCode < static_cast<sf::Keyboard::Key>(names.size());
    }
}

bool KeyboardHelper::isHeld(const std::string& keyName) {

    const sf::Keyboard::Key keyCode = getKeyCode(keyName);
    return isValid(keyCode) ? isHeldNow[keyCode] : false;
}

bool KeyboardHelper::isDown(const std::string& keyName) {

    const sf::Keyboard::Key keyCode = getKeyCode(keyName);
    if (!isValid(keyCode))
        return false;

    return !wasHeldLastUpdate[keyCode] && isHeldNow[keyCode];
}

bool KeyboardHelper::isUp(const std::string& keyName) {

    const sf::Keyboard::Key keyCode = getKeyCode(keyName);
    if (!isValid(keyCode))
        return false;

    return wasHeldLastUpdate[keyCode] && !isHeldNow[keyCode];
}

void KeyboardHelper::update() {
    std::copy(isHeldNow.begin(), isHeldNow.end(), wasHeldLastUpdate.begin());
}

void KeyboardHelper::receive(const sf::Event& info) {

    if (info.type == sf::Event::EventType::KeyPressed) {

        if (isValid(info.key.code))
            isHeldNow[info.key.code] = true;

    } else if (info.type == sf::Event::EventType::KeyReleased) {

        if (isValid(info.key.code))
            isHeldNow[info.key.code] = false;
    }
}
