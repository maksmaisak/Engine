//
// Created by Maksym Maisak on 29/12/18.
//

#ifndef ENGINE_LUASTACK_H
#define ENGINE_LUASTACK_H

#include <type_traits>
#include <lua.hpp>
#include <string>
#include <optional>
#include <variant>
#include <iostream>

#include "Demangle.h"
#include "Meta.h"
#include "glm.h"
#include "Exception.h"

namespace lua {

    /// TODO Move the implementation of non-templates like this into a .cpp file
    inline std::string getAsString(lua_State* L, int index = -1) {

        int typeId = lua_type(L, index);
        switch (typeId) {
            case LUA_TSTRING:
                return std::string("\"") + lua_tostring(L, index) + "\"";
            case LUA_TBOOLEAN:
                return lua_toboolean(L, index) ? "true" : "false";
            case LUA_TNUMBER:
                return std::to_string(lua_tonumber(L, index));
            default:
                return lua_typename(L, typeId);
        }
    }

    inline void stackDump(lua_State *L) {

        int i = lua_gettop(L);
        printf(" ----------------  Stack Dump ----------------\n");
        while (i) {
            int t = lua_type(L, i);
            switch (t) {
                case LUA_TSTRING:
                    printf("%d:`%s'\n", i, lua_tostring(L, i));
                    break;
                case LUA_TBOOLEAN:
                    printf("%d: %s\n",i,lua_toboolean(L, i) ? "true" : "false");
                    break;
                case LUA_TNUMBER:
                    printf("%d: %g\n",  i, lua_tonumber(L, i));
                    break;
                default: printf("%d: %s\n", i, lua_typename(L, t)); break;
            }
            --i;
        }
        printf("--------------- Stack Dump Finished ---------------\n");
    }

    class PopperOnDestruct {

    public:
        inline PopperOnDestruct(lua_State* luaState) : L(luaState) {}
        inline ~PopperOnDestruct() {lua_pop(L, 1);}

    private:
        lua_State* L;
    };

    template<typename T>
    inline bool getMetatable(lua_State*);

    // Default case. Treat everything as userdata
    template<typename T, typename = void>
    struct TypeAdapter {

        inline static void push(lua_State* L, const T& value) {

            void* ptr = lua_newuserdata(L, sizeof(T));
            T* valuePtr = new(ptr) T(value); // copy-construct in place.

            getMetatable<T>(L);
            lua_setmetatable(L, -2);
        }

        inline static T& to(lua_State* L, int index = -1) {

            void* ptr = lua_touserdata(L, index);
            return *static_cast<T*>(ptr);
        }

        inline static T& check(lua_State* L, int index = -1) {

            void* ptr = luaL_checkudata(L, index, utils::demangle<T>().c_str());
            return *static_cast<T*>(ptr);
        }

        inline static bool is(lua_State* L, int index = -1) {

            return luaL_testudata(L, index, utils::demangle<T>().c_str()) != nullptr;
        }
    };

    template<typename T>
    inline void push(lua_State* L, T&& value) {TypeAdapter<utils::remove_cvref_t<T>>::push(L, std::forward<T>(value));}

    template<typename T>
    inline bool is(lua_State* L, int index = -1) {return TypeAdapter<utils::remove_cvref_t<T>>::is(L, index);}

    template<typename T>
    inline decltype(auto) to(lua_State* L, int index = -1) {return TypeAdapter<utils::remove_cvref_t<T>>::to(L, index);}

    template<typename T>
    inline decltype(auto) check(lua_State* L, int index = -1) {return TypeAdapter<utils::remove_cvref_t<T>>::check(L, index);}

    template<typename T>
    inline decltype(auto) get(lua_State* L) {

        auto p = PopperOnDestruct(L); // pop when the function returns
        return to<T>(L);
    }

    template<typename T>
    inline std::optional<T> tryGet(lua_State* L) {

        auto p = PopperOnDestruct(L); // pop when the function returns
        if (lua_isnil(L, -1) || !is<T>(L)) return std::nullopt;
        return to<T>(L);
    }

    template<typename T, typename TKey, typename = void>
    struct FieldGetter {

        static std::optional<T> tryGetField(lua_State* L, TKey&& key, int tableIndex = -1) {

            if (!lua_istable(L, tableIndex))
                return std::nullopt;

            tableIndex = lua_absindex(L, tableIndex);

            push(L, std::forward<TKey>(key));
            lua_gettable(L, tableIndex);
            return tryGet<T>(L);
        }
    };

    template<typename T, typename TKey>
    struct FieldGetter<T, TKey, std::enable_if_t<utils::is_string_v<TKey>>> {

        static std::optional<T> tryGetField(lua_State* L, TKey&& key, int tableIndex = -1) {

            if (!lua_istable(L, tableIndex)) return std::nullopt;

            lua_getfield(L, tableIndex, &key[0]);
            return tryGet<T>(L);
        }
    };

    template<typename T, typename TKey>
    inline std::optional<T> tryGetField(lua_State* L, TKey&& key, int tableIndex = -1) {
        return FieldGetter<T, TKey>::tryGetField(L, std::forward<TKey>(key), tableIndex);
    }

    template<>
    struct TypeAdapter<bool> {
        static bool is   (lua_State* L, int index = -1) { return lua_isboolean(L, index); }
        static bool to   (lua_State* L, int index = -1) { return lua_toboolean(L, index); }
        static bool check(lua_State* L, int index = -1) { return luaL_checktype(L, index, LUA_TBOOLEAN), to(L, index); }
        static void push (lua_State* L, bool value) { lua_pushboolean(L, value); }
    };

    template<typename T>
    struct TypeAdapter<T, std::enable_if_t<std::is_integral_v<T>>> {
        static bool is   (lua_State* L, int index = -1) { return lua_isinteger(L, index); }
        static T    to   (lua_State* L, int index = -1) { return static_cast<T>(lua_tointeger(L, index)); }
        static T    check(lua_State* L, int index = -1) { return static_cast<T>(luaL_checkinteger(L, index)); }
        static void push (lua_State* L, T value) { lua_pushinteger(L, value); }
    };

    template<typename T>
    struct TypeAdapter<T, std::enable_if_t<std::is_floating_point_v<T>>> {
        static bool is   (lua_State* L, int index = -1) { return lua_isnumber(L, index); }
        static T    to   (lua_State* L, int index = -1) { return static_cast<T>(lua_tonumber(L, index)); }
        static T    check(lua_State* L, int index = -1) { return static_cast<T>(luaL_checknumber(L, index)); }
        static void push (lua_State* L, T value) { lua_pushnumber(L, value); }
    };

    template<typename T>
    struct TypeAdapter<T, std::enable_if_t<utils::is_string_v<T>>> {
        static bool        is   (lua_State* L, int index = -1) { return lua_isstring(L, index); }
        static std::string to   (lua_State* L, int index = -1) { return lua_tostring(L, index); }
        static std::string check(lua_State* L, int index = -1) { return luaL_checkstring(L, index); }
        static void        push (lua_State* L, const std::string& value) { lua_pushstring(L, value.c_str()); }
    };

    template<>
    struct TypeAdapter<lua_CFunction> {
        static bool          is  (lua_State* L, int index = -1) { return lua_iscfunction(L, index); }
        static lua_CFunction to  (lua_State* L, int index = -1) { return lua_tocfunction(L, index); }
        static lua_CFunction check(lua_State* L, int index = -1) {
            if (!is(L, index)) luaL_error(L, "Bad argument #%d, expected %s, got %s", index, "c function", luaL_typename(L, index));
            return to(L, index);
        }
        static void          push(lua_State* L, lua_CFunction func) { lua_pushcfunction(L, func); }
    };

    template<typename T>
    struct TypeAdapter<std::optional<T>> {

        static bool is(lua_State* L, int index = -1) {
            return lua_isnoneornil(L, index) || lua::is<T>(L, index);
        }
        static std::optional<T> to (lua_State* L, int index = -1) {
            return lua_isnoneornil(L, index) ? std::nullopt : std::make_optional<T>(lua::to<T>(L, index));
        }
        static std::optional<T> check(lua_State* L, int index = -1) {
            if (!is(L, index))
                luaL_error(L, "Bad argument #%d, expected %s or nil, got %s", index, utils::demangle<T>().c_str(), luaL_typename(L, index));
            return to(L, index);
        }
        static void push(lua_State* L, const std::optional<T>& value) {
            if (value)
                lua::push(L, *value);
            else
                lua_pushnil(L);
        }
    };

    template<typename... Types>
    struct TypeAdapter<std::variant<Types...>> {

        using V = std::variant<Types...>;
        using VSize = std::variant_size<V>;

        static bool is(lua_State* L, int index = -1) {
            return (lua::is<Types>(L, index) || ...);
        }

        template <std::size_t I>
        static V toImpl(std::integral_constant<std::size_t, I>, lua_State* L, int index) {

            using PotentialT = std::variant_alternative_t<I - 1, V>;
            if (lua::is<PotentialT>(L, index))
                return V(std::in_place_index<I - 1>, lua::to<PotentialT>(L, index));

            if constexpr (I - 1 > 0)
                return toImpl(std::integral_constant<std::size_t, I - 1>(), L, index);

            return check(L, index);
        }

        static V to(lua_State* L, int index = -1) {
            return toImpl(std::integral_constant<std::size_t, VSize::value>(), L, index);
        }

        static V check(lua_State* L, int index = -1) {
            if (!is(L, index))
                luaL_error(L, "Bad argument #%d, expected one of %s, got %s", index, ((utils::demangle<Types>() + ", ") + ...).c_str(), luaL_typename(L, index));
            return to(L, index);
        }

        template <std::size_t I>
        static void pushImpl(std::integral_constant<std::size_t, I>, lua_State* L, const V& variant) {

            using ValueT = std::variant_alternative_t<I - 1, V>;
            if (auto* value = std::get_if<ValueT>(variant)) {
                lua::push<ValueT>(&value);
                return;
            }

            if constexpr (I - 1 > 0)
                pushImpl(std::integral_constant<std::size_t, I - 1>(), L, index);
        }

        static void push(lua_State* L, const V& variant) {
            pushImpl(std::integral_constant<std::size_t, VSize::value>(), L, index);
        }
    };

    template<glm::length_t Length, typename T, glm::qualifier Q>
    struct TypeAdapter<glm::vec<Length, T, Q>> {

        using TVec = glm::vec<Length, T, Q>;

        struct Keys {
            std::string key1;
            std::string key2;
            int key3 = 0;
        };

        inline static const Keys keys[] = {
            {"x", "r", 1},
            {"y", "g", 2},
            {"z", "b", 3},
            {"w", "a", 4}
        };

        static bool is(lua_State* L, int index = -1) {
            return lua_istable(L, index);
        }

        static TVec check(lua_State* L, int index = -1) {
            if (!is(L, index)) luaL_error(L, "Bad argument #%d, expected %s, got %s", index, utils::demangle<glm::vec3>().c_str(), luaL_typename(L, index));
            return to(L, index);
        }

        static T tryGetValue(lua_State* L, const Keys& keys, int tableIndex) {

            auto opt1 = lua::tryGetField<T>(L, keys.key1, tableIndex);
            if (opt1)
                return *opt1;

            auto opt2 = lua::tryGetField<T>(L, keys.key2, tableIndex);
            if (opt2)
                return *opt2;

            auto opt3 = lua::tryGetField<T>(L, keys.key3, tableIndex);
            if (opt3)
                return *opt3;

            return static_cast<T>(0);
        }

        static TVec to(lua_State* L, int index = -1) {

            TVec vec;
            for (std::size_t i = 0; i < Length; ++i) {
                vec[i] = tryGetValue(L, keys[i], index);
            }

            //std::cout << "received: (" << result.x << ", " << result.y << ", " << result.z << ")\n";

            return vec;
        }

        static void push(lua_State* L, const TVec& value) {

            lua_createtable(L, 0, Length);

            for (std::size_t i = 0; i < Length; ++i) {
                lua::push(L, value[i]);
                lua_setfield(L, -2, keys[i].key1.c_str());
            }
        }
    };
}

#endif //ENGINE_LUASTACK_H
