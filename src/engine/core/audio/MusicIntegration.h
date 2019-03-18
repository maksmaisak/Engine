//
// Created by Maksym Maisak on 2019-03-04.
//

#ifndef SAXION_Y2Q2_RENDERING_MUSICINTEGRATION_H
#define SAXION_Y2Q2_RENDERING_MUSICINTEGRATION_H

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

        static std::shared_ptr<sf::Music> load(const std::string& filepath);
    };
}

#endif //SAXION_Y2Q2_RENDERING_MUSICINTEGRATION_H
