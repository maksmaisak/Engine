//
// Created by Maksym Maisak on 1/1/19.
//

#ifndef ENGINE_NAME_H
#define ENGINE_NAME_H

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

#endif //ENGINE_NAME_H
