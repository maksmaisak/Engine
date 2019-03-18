//
// Created by Maksym Maisak on 2019-01-31.
//

#ifndef SAXION_Y2Q2_RENDERING_LUAREFERENCE_H
#define SAXION_Y2Q2_RENDERING_LUAREFERENCE_H

#include <lua.hpp>

namespace lua {

    /// An owning wrapper around a lua reference.
    /// Calls luaL_ref in constructor,
    /// calls luaL_unref in destructor.
    /// Non-copyable, only movable, same as std::unique_ptr.
    class LuaReference {

    public:
        LuaReference();
        LuaReference(lua_State*);
        virtual ~LuaReference();
        LuaReference(const LuaReference&) = delete;
        LuaReference& operator=(const LuaReference&) = delete;
        LuaReference(LuaReference&&) noexcept;
        LuaReference& operator=(LuaReference&&) noexcept;

        lua_State* getLuaState() const;
        int getRef() const;
        void push() const;
        operator bool() const;

        friend bool operator==(const LuaReference& a, const LuaReference& b);

    private:

        bool unref();

        lua_State* L = nullptr;
        int m_ref = LUA_NOREF;
    };
}

#endif //SAXION_Y2Q2_RENDERING_LUAREFERENCE_H
