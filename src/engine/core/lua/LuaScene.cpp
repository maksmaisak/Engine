//
// Created by Maksym Maisak on 30/12/18.
//

#include "LuaScene.h"
#include <iostream>
#include <lua.hpp>
#include <cassert>
#include <map>
#include <vector>
#include <optional>
#include <tuple>
#include "engine/Engine.h"
#include "engine/core/lua/LuaStack.h"
#include "engine/resources/Resources.h"

#include "engine/core/lua/ComponentsToLua.h"
#include "engine/components/Name.h"
#include "engine/components/Transform.h"
#include "engine/components/RenderInfo.h"
#include "engine/components/Rigidbody.h"
#include "engine/components/Light.h"
#include "engine/components/Camera.h"
#include "Material.h"

using namespace en;

LuaReference getTableByFilename(LuaState& lua, const std::string& filename) {

    std::cout << "Loading lua-defined scene " + filename + "..." << std::endl;

    if (lua.doFileInNewEnvironment(filename, 1))
        return LuaReference(lua);

    return {};
}

LuaScene::LuaScene(LuaState& lua, const std::string& filename) : m_self(getTableByFilename(lua, filename)) {}
LuaScene::LuaScene(LuaReference&& table) : m_self(std::move(table)) {}

void LuaScene::open() {

    LuaState& lua = getEngine().getLuaState();

    if (!pushTableOnStack())
        return;
    auto popTable = PopperOnDestruct(lua);

    {
        lua_getfield(lua, -1, "ambientLighting");
        auto popAmbientLighting = PopperOnDestruct(lua);
        if (!lua_isnil(lua, -1))
            m_renderSettings.ambientColor = lua.tryGetField<glm::vec3>("color").value_or(glm::vec3(0.f));
    }

    lua_getfield(lua, -1, "update");
    if (lua_isfunction(lua, -1))
        m_update = LuaReference(lua);
    else
        lua_pop(lua, 1);

    lua_getfield(lua, -1, "onCollision");
    if (lua_isfunction(lua, -1))
        m_onCollision = LuaReference(lua);
    else
        lua_pop(lua, 1);

    lua_getfield(lua, -1, "start");
    if (lua_isfunction(lua, -1)) {
        m_self.push();
        lua.pcall(1, 0);
    } else {
        lua_pop(lua, 1);
    }

    std::cout << "Finished loading lua-defined scene." << std::endl;
}

void LuaScene::update(float dt) {

    if (!m_update)
        return;

    LuaState& lua = getEngine().getLuaState();

    m_update.push();
    if (!lua_isfunction(lua, -1)) {
        lua_pop(lua, 1);
        return;
    }

    m_self.push();
    lua.push(dt);
    lua.pcall(2, 0);
}

void LuaScene::receive(const Collision& collision) {

    Engine& engine = getEngine();
    LuaState& lua = engine.getLuaState();

    m_onCollision.push();
    if (!lua_isfunction(lua, -1)) {
        lua_pop(lua, 1);
        return;
    }

    m_self.push();
    lua.push(engine.actor(collision.a));
    lua.push(engine.actor(collision.b));
    lua.pcall(3, 0);
}

bool LuaScene::pushTableOnStack() {

    LuaState& lua = getEngine().getLuaState();

    if (m_self) {
        m_self.push();
        return true;
    }

    return false;
}
