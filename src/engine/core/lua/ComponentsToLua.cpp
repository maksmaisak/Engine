//
// Created by Maksym Maisak on 31/12/18.
//

#include "ComponentsToLua.h"
#include <cassert>
#include <iostream>
#include "Transform.h"
#include "Name.h"

using namespace en;

namespace {

    bool pushComponentReferenceWithChildren(Actor& actor, LuaState& lua, const std::function<void(Actor& actor, LuaState& lua)>& push) {

        push(actor, lua);
        if (!lua_isnil(lua, -1)) {
            return true;
        }

        if (auto* transform = actor.tryGet<Transform>()) {

            Engine& engine = actor.getEngine();
            for (Entity child : transform->getChildren()) {
                Actor childActor = engine.actor(child);
                if (pushComponentReferenceWithChildren(childActor, lua, push)) {
                    return true;
                }
            }
        }

        return false;
    }
}

void ComponentsToLua::pushComponentReferenceByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName) {

    auto lua = LuaState(L);
    getTypeInfoByName(componentTypeName).pushFromActor(actor, lua);
}

void ComponentsToLua::pushComponentReferenceByTypeNameWithChildren(lua_State* L, Actor& actor, const std::string& componentTypeName) {

    auto lua = LuaState(L);
    const auto& push = getTypeInfoByName(componentTypeName).pushFromActor;
    pushComponentReferenceWithChildren(actor, lua, push);
}

void ComponentsToLua::addComponentByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName) {

    auto lua = LuaState(L);
    getTypeInfoByName(componentTypeName).addToActor(actor, lua);
}

void ComponentsToLua::addComponentByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName, int componentDefinitionIndex) {

    auto lua = LuaState(L);
    getTypeInfoByName(componentTypeName).addToActorFromDefinition(actor, lua, componentDefinitionIndex);
}

void ComponentsToLua::removeComponentByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName) {

    auto lua = LuaState(L);
    getTypeInfoByName(componentTypeName).removeFromActor(actor, lua);
}

ComponentsToLua::TypeInfo& ComponentsToLua::getTypeInfoByName(const std::string& typeName) {

    auto& map = getNameToTypeInfoMap();
    auto it = map.find(typeName);
    if (it == map.end()) {
        throw utils::Exception("Unknown component type: " + typeName);
    }

    return it->second;
}

void ComponentsToLua::printDebugInfo() {

    std::cout << std::endl << "ComponentsToLua:\nRegistered component types:" << std::endl;

    for (const auto& pair : getNameToTypeInfoMap()) {
        std::cout << pair.first << std::endl;
    }

    std::cout << std::endl;
}

std::vector<Actor> ComponentsToLua::makeEntities(lua_State* L, Engine& engine, int index) {

    index = lua_absindex(L, index);

    std::vector<Actor> actors;
    std::vector<int> refs;

    // Create entities and assign their names.
    lua_pushnil(L);
    while (lua_next(L, index)) {

        const auto popValue = lua::PopperOnDestruct(L);
        if (!lua_istable(L, -1)) {
            continue;
        }

        // Save a ref to the entity definition and the actor for adding components later.
        actors.push_back(makeEntity(L, engine, -1));
        lua_pushvalue(L, -1);
        refs.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
    }

    // Add all other components to the entities.
    for (std::size_t i = 0; i < actors.size(); ++i) {

        const auto pop = lua::PopperOnDestruct(L);
        lua_rawgeti(L, LUA_REGISTRYINDEX, refs[i]);

        const int oldTop = lua_gettop(L);
        addComponents(L, actors[i], -1);
        const int newTop = lua_gettop(L);
        assert(oldTop == newTop);
    }

    // Release the references
    for (int ref : refs) {
        luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }

    return actors;
}

Actor ComponentsToLua::makeEntity(lua_State* L, Engine& engine, int entityDefinitionIndex) {

    entityDefinitionIndex = lua_absindex(L, entityDefinitionIndex);
    assert(lua_istable(L, entityDefinitionIndex));

    en::Actor actor = engine.makeActor();

    // Get and assign the name
    lua_getfield(L, entityDefinitionIndex, "Name");
    const auto pop = lua::PopperOnDestruct(L);
    if (lua::is<std::string>(L)) {
        actor.add<en::Name>(lua::to<std::string>(L));
    }

    return actor;
}

void ComponentsToLua::addComponents(lua_State* L, Actor& actor, int entityDefinitionIndex) {

    entityDefinitionIndex = lua_absindex(L, entityDefinitionIndex);

    // Iterate over the entity definition.
    lua_pushnil(L);
    while (lua_next(L, entityDefinitionIndex)) {

        const auto popValue = lua::PopperOnDestruct(L);

        // -1: value (component definition)
        // -2: key   (component type name)
        auto componentTypeName = lua::to<std::string>(L, -2);
        if (componentTypeName != "Name") {
            makeComponent(L, actor, componentTypeName, -1);
        }
    }
}

void ComponentsToLua::makeComponent(lua_State* L, Actor& actor, const std::string& componentTypeName, int componentValueIndex) {

    componentValueIndex = lua_absindex(L, componentValueIndex);

    const auto& map = getNameToTypeInfoMap();
    const auto it = map.find(componentTypeName);
    if (it == map.end()) {
        std::cout << "Unknown component type: " << componentTypeName << std::endl;
        return;
    }
    const TypeInfo& typeInfo = it->second;

    const int oldTop = lua_gettop(L);

    lua_pushvalue(L, componentValueIndex);
    LuaState stateWrapper = LuaState(L);
    typeInfo.addFromLua(actor, stateWrapper);
    lua_pop(L, 1); // pop the component definition

    const int newTop = lua_gettop(L);
    assert(oldTop == newTop);

    if (lua_istable(L, -1)) {

        // TODO make the addFromLua function push the component pointer onto the stack to avoid a second string lookup here.
        pushComponentReferenceByTypeName(L, actor, componentTypeName);
        const auto popComponentPointer = PopperOnDestruct(L);
        const int componentPointerIndex = lua_gettop(L);

        lua_pushnil(L);
        while (lua_next(L, componentValueIndex)) {

            auto popValue = PopperOnDestruct(L);
            lua_pushvalue(L, -2);
            lua_pushvalue(L, -2);
            lua_settable(L, componentPointerIndex);
        }
    }
}
