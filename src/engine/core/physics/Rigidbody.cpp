//
// Created by Maksym Maisak on 2019-01-22.
//

#include "Rigidbody.h"

using namespace en;

void Rigidbody::initializeMetatable(LuaState& lua) {

    lua::addProperty(lua, "velocity",    lua::property(&Rigidbody::velocity));
    lua::addProperty(lua, "isStatic", lua::property(&Rigidbody::isStatic));
    lua::addProperty(lua, "useGravity",  lua::property(&Rigidbody::useGravity));
    lua::addProperty(lua, "bounciness",  lua::property(&Rigidbody::bounciness));
}
