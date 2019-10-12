//
// Created by Maksym Maisak on 1/1/19.
//

#include "StringName.h"

using namespace en;

StringName& StringName::addFromLua(Actor& actor, LuaState& lua) {

    if (!lua.is<std::string>()) throw "Value of Name must be a string";

    return actor.add<StringName>(lua.to<std::string>());
}
