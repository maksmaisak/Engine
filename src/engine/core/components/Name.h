//
// Created by Maksym Maisak on 1/1/19.
//

#ifndef SAXION_Y2Q2_RENDERING_NAME_H
#define SAXION_Y2Q2_RENDERING_NAME_H

#include <string>
#include "ComponentsToLua.h"

namespace en {

    struct Name {

        LUA_TYPE(Name)

        static Name& addFromLua(Actor& actor, LuaState& lua);

        std::string value;

        inline operator std::string() {return value;}
    };
}

#endif //SAXION_Y2Q2_RENDERING_NAME_H
