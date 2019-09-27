//
// Created by Maksym Maisak on 2019-02-14.
//

#include "UIRect.h"

using namespace en;

void UIRect::initializeMetatable(LuaState& lua) {

    lua::addProperty(lua, "anchorMin", lua::property(&UIRect::anchorMin));
    lua::addProperty(lua, "anchorMax", lua::property(&UIRect::anchorMax));
    lua::addProperty(lua, "offsetMin", lua::property(&UIRect::offsetMin));
    lua::addProperty(lua, "offsetMax", lua::property(&UIRect::offsetMax));
    lua::addProperty(lua, "isEnabled", lua::property(&UIRect::isEnabled));
    lua::addProperty(lua, "pivot"    , lua::property(&UIRect::pivot    ));

    lua::addProperty(lua, "computedSize", lua::readonlyProperty(&UIRect::computedSize));
    lua::addProperty(lua, "isMouseOver" , lua::readonlyProperty(&UIRect::isMouseOver ));
    lua::addProperty(lua, "wasMouseOver", lua::readonlyProperty(&UIRect::wasMouseOver));
}