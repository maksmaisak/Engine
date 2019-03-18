//
// Created by Maksym Maisak on 1/1/19.
//

#include "Name.h"

using namespace en;

Name& Name::addFromLua(Actor& actor, LuaState& lua) {

    if (!lua.is<std::string>()) throw "Value of Name must be a string";

    return actor.add<Name>(lua.to<std::string>());
}
