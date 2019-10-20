//
// Created by Maksym Maisak on 2019-03-04.
//

#ifndef ENGINE_MUSICINTEGRATION_H
#define ENGINE_MUSICINTEGRATION_H

#include <memory>
#include <SFML/Audio.hpp>
#include "MetatableHelper.h"
#include "LuaState.h"
#include "Resources.h"

namespace lua {

    template<>
    struct InitializeMetatable<sf::Music> {

        static void initializeMetatable(en::LuaState& lua);
    };
}

namespace en {

    template<>
    struct ResourceLoader<sf::Music> {

        static std::shared_ptr<sf::Music> load(const Name& filepath);
    };
}

#endif //ENGINE_MUSICINTEGRATION_H
