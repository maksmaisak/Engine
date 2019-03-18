//
// Created by Maksym Maisak on 29/12/18.
//

#include "LuaState.h"
#include <lua.hpp>
#include <iostream>

using namespace en;

LuaState::LuaState() : L(luaL_newstate()), shouldCloseOnDestroy(true) {
    luaL_openlibs(L);
}

LuaState::LuaState(lua_State* existingL) : L(existingL), shouldCloseOnDestroy(false)
{}

LuaState::~LuaState() {

    if (shouldCloseOnDestroy) {
        std::cout << "Closing a lua state" << std::endl;
        lua_close(L);
    }
}

void LuaState::pop(int numToPop) {
    lua_pop(L, numToPop);
}

bool LuaState::loadFile(const std::string& filename) {

    int errorCode = luaL_loadfile(L, filename.c_str());
    if (errorCode != LUA_OK) {
        printError();
        return false;
    }
    return true;
}

int errorMessageHandler(lua_State* L) {

    luaL_traceback(L, L, lua_tostring(L, 1), 1);
    return 1;
}

bool LuaState::pcall(int numArgs, int numResults) {

    int oldTop = lua_gettop(L);

    // put the error message handler before the function and its arguments
    push(&errorMessageHandler);
    int errorMessageHandlerIndex = -(1 + (1 + numArgs));
    lua_insert(L, errorMessageHandlerIndex);
    errorMessageHandlerIndex = lua_absindex(L, errorMessageHandlerIndex);

    try {

        int errorCode = lua_pcall(L, numArgs, numResults, errorMessageHandlerIndex);
        if (errorCode != LUA_OK) {
            printError();
            lua_settop(L, oldTop - (1 + numArgs));
            return false;
        }
        lua_remove(L, errorMessageHandlerIndex);
        return true;

    } catch (std::exception& ex) {

        std::cerr << "An exception was thrown when executing lua code: " << ex.what() << std::endl;
        luaL_traceback(L, L, nullptr, 1);
        std::cerr << get<std::string>() << std::endl;

        lua_settop(L, oldTop - (1 + numArgs));
        return false;
    }
}

bool LuaState::doFile(const std::string& filename, int numResults) {

    if (!loadFile(filename)) {
        for (int i = 0; i < numResults; ++i)
            lua_pushnil(L);
        return false;
    }

    return pcall(0, numResults);
}

bool LuaState::doFileInNewEnvironment(const std::string& filename, int numResults) {

    makeEnvironment();

    if (!loadFile(filename)) return false;
    setEnvironment(-2);
    return pcall(0, numResults);
}

void LuaState::makeEnvironment() {

    // create table
    lua_newtable(L);
    int tableIndex = lua_gettop(L);

    // create metatable
    lua_newtable(L);
    int metatableIndex = lua_gettop(L);
    
    // set __index = _G
    lua_pushglobaltable(L);
    lua_setfield(L, metatableIndex, "__index");

    // assign metatable
    lua_setmetatable(L, tableIndex);
}

void LuaState::setEnvironment(int environmentIndex) {

    lua_pushvalue(L, environmentIndex);
    lua_setupvalue(L, -2, 1);
}

void LuaState::printError() {

    std::cerr << tryGet<std::string>().value_or("An error occured, but the error message could not be retrieved.") << std::endl;
}
