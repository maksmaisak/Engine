//
// Created by Maksym Maisak on 1/1/19.
//

#ifndef ENGINE_STRINGNAME_H
#define ENGINE_STRINGNAME_H

#include <string>
#include "ComponentsToLua.h"

namespace en {

    struct StringName {

        LUA_TYPE(StringName)

        static StringName& addFromLua(Actor& actor, LuaState& lua);

        std::string value;

        inline operator std::string() {return value;}
    };
}

#endif //ENGINE_STRINGNAME_H
