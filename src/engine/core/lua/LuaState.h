//
// Created by Maksym Maisak on 29/12/18.
//

#ifndef ENGINE_LUASTATE_H
#define ENGINE_LUASTATE_H

#include <lua.hpp>
#include <memory>
#include <cassert>
#include <string>
#include <optional>

#include "LuaStack.h"
#include "ClosureHelper.h"

namespace en {

    using namespace lua;

    /// A wrapper around a lua_State*. Can be implicitly cast to that.
    class LuaState final {

    public:

        LuaState();
        LuaState(lua_State* existingL);
        ~LuaState();

        bool pcall(int numArgs = 0, int numResults = 0);
        bool loadFile(const std::string& filename);
        bool doFile(const std::string& filename, int numResults = 0);

        /// Creates a new environment table that inherits from the global one.
        /// Pushes it onto the stack.
        void makeEnvironment();

        /// Sets the environment of the closure on top of the stack to the value
        /// at the given index.
        void setEnvironment(int environmentIndex);

        /// Loads a file and runs it in a new environment (see makeEnvironment)
        /// Pushes the environment table onto the stack.
        bool doFileInNewEnvironment(const std::string& filename, int numResults = 0);

        inline operator lua_State* () {return L;}

        /*
        template<typename TResult, typename... TArgs>
        std::optional<TResult> protectedCall(TArgs&&... args) {

            (lua::push<TArgs>(args), ...);
            lua_pcall(L, sizeof...(TArgs), 1, 0);
            return get<TResult>();
            //return {lua::to<TResults>(L, index), ...};
        }*/

        template<typename T>
        inline std::optional<T> tryGetGlobal(const std::string& name) {

            lua_getglobal(L, name.c_str());
            return tryGet<T>();
        }

        template<typename T>
        inline std::optional<T> tryGetField(const std::string& name, int tableIndex = -1) {
            return lua::tryGetField<T>(L, name, tableIndex);
        }

        template<typename T>
        inline void setField(const std::string& name, T&& value, int tableIndex = -1) {

            tableIndex = lua_absindex(L, tableIndex);
            luaL_checktype(L, tableIndex, LUA_TTABLE);

            if constexpr (utils::functionTraits<utils::remove_cvref_t<T>>::isFunction && !std::is_convertible_v<T, lua_CFunction>) {
                ClosureHelper::makeClosure(L, std::forward<T>(value));
            } else {
                lua::push(L, std::forward<T>(value));
            }

            lua_setfield(L, tableIndex, name.c_str());
        }

        template<typename TResult, typename TOwner, typename... Args>
        inline void setField(const std::string& name, TResult(TOwner::*memberFunctionPtr)(Args...), TOwner* ownerPtr, int tableIndex = -1) {

            luaL_checktype(L, tableIndex, LUA_TTABLE);
            tableIndex = lua_absindex(L, tableIndex);

            ClosureHelper::makeClosure(L, memberFunctionPtr, ownerPtr);

            lua_setfield(L, tableIndex, name.c_str());
        }

        template<typename T>
        inline decltype(auto) get() {return lua::get<T>(L);}

        /// Pops and returns the value on top of the stack.
        template<typename T>
        inline std::optional<T> tryGet() {return lua::tryGet<T>(L);}

        template<typename T>
        inline bool is(int index = -1) const {return lua::is<T>(L, index);}

        template<typename T>
        inline decltype(auto) to(int index = -1) const {return lua::to<T>(L, index);}

        template<typename T>
        inline decltype(auto) check(int index = -1) const {return lua::check<T>(L, index);}

        template<typename T>
        inline void push(T&& value) {return lua::push<T>(L, std::forward<T>(value));}

        void pop(int numToPop = 1);

    private:

        void printError();

        bool shouldCloseOnDestroy = true;
        lua_State* L;
    };
}

#endif //ENGINE_LUASTATE_H
