//
// Created by Maksym Maisak on 2019-01-31.
//

#include "LuaReference.h"
#include <cassert>
#include <iostream>

using namespace lua;

LuaReference::LuaReference() {}
LuaReference::LuaReference(lua_State* L) : L(L), m_ref(luaL_ref(L, LUA_REGISTRYINDEX)) {
    //std::cout << "LuaReference: create ref: " << m_ref << std::endl;
}

LuaReference::~LuaReference() {
    unref();
}

LuaReference::LuaReference(LuaReference&& other) noexcept :
    L(other.L),
    m_ref(other.m_ref)
{
    other.L = nullptr;
    other.m_ref = LUA_NOREF;
}

LuaReference& LuaReference::operator=(LuaReference&& other) noexcept {

    if (*this == other)
        return *this;

    unref();
    L = other.L;
    m_ref = other.m_ref;

    other.L = nullptr;
    other.m_ref = LUA_NOREF;

    return *this;
}

int LuaReference::getRef() const {
    return m_ref;
}

lua_State* LuaReference::getLuaState() const {
    return L;
}

LuaReference::operator bool() const {
    return L && m_ref != LUA_NOREF;
}

void LuaReference::push() const {

    assert(*this);
    lua_rawgeti(L, LUA_REGISTRYINDEX, m_ref);
}

bool LuaReference::unref() {

    if (m_ref != LUA_NOREF) {

        //std::cout << "LuaReference: remove ref: " << m_ref << std::endl;

        luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        m_ref = LUA_NOREF;
        return true;
    }

    return false;
}

bool lua::operator==(const LuaReference& a, const LuaReference& b) {

    return a.L == b.L && a.m_ref == b.m_ref;
}
