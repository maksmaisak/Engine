//
// Created by Maksym Maisak on 31/12/18.
//

#ifndef ENGINE_COMPONENTSTOLUA_H
#define ENGINE_COMPONENTSTOLUA_H

#include <map>
#include <type_traits>
#include <cassert>
#include <functional>
#include "MetatableHelper.h"
#include "LuaState.h"
#include "Actor.h"
#include "ComponentReference.h"

// Calls en::ComponentsToLua::registerType<T>(#T); at dynamic initialization
// Put this in a class definition
#define LUA_TYPE(T) inline static const auto __componentTypeRequirement = en::LuaTypeRequirement<T>(#T);

#define LUA_REGISTER_TYPE(T) en::ComponentsToLua::registerType<T>(#T);

namespace en {

    namespace detail {

        using LuaComponentFactoryFunction = std::function<void(Actor&, LuaState&)>;

        template<typename T, typename = void>
        struct LuaComponentFactoryFunctionOf {

            inline static void addComponent(Actor& actor, LuaState&) {actor.add<T>();}
            inline static LuaComponentFactoryFunction get() {return addComponent;}
        };

        template<typename T>
        struct LuaComponentFactoryFunctionOf<T, std::enable_if_t<std::is_convertible_v<decltype(&T::addFromLua), LuaComponentFactoryFunction>>> {

            inline static LuaComponentFactoryFunction get() {return T::addFromLua;}
        };
    }

    /// Handles reading component definitions out of lua values.
    /// See makeComponent
    /// Use registerComponentType<T>(name) to map a type name to a component type
    class ComponentsToLua {

    public:

        template<typename TComponent>
        static void registerType(const std::string& name);

        /// Goes through the table at the given index, and makes entities out of its fields.
        /// First creates all the entities and assigns their names, if provided.
        /// Then adds all other components to them.
        /// This is necessary to make sure findByName works during component initialization.
        static std::vector<Actor> makeEntities(lua_State* L, Engine& engine, int index = -1);
        static Actor makeEntity(lua_State* L, Engine& engine, int index = -1);
        static void addComponents(lua_State* L, Actor& actor, int index = -1);

        /// Adds a component of a given type from a value at the given index in the lua stack
        static void makeComponent(lua_State* L, Actor& actor, const std::string& componentTypeName, int componentValueIndex = -1);

        static void pushComponentReferenceByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName);
        static void pushComponentReferenceByTypeNameWithChildren(lua_State* L, Actor& actor, const std::string& componentTypeName);
        static void addComponentByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName);
        static void addComponentByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName, int componentDefinitionIndex);
        static void removeComponentByTypeName(lua_State* L, Actor& actor, const std::string& componentTypeName);

        static void printDebugInfo();

    private:

        struct TypeInfo {
            std::function<void(Actor&, LuaState&)> addFromLua;
            std::function<void(Actor&, LuaState&)> pushFromActor;
            std::function<void(Actor&, LuaState&)> addToActor;
            std::function<void(Actor&, LuaState&, int)> addToActorFromDefinition;
            std::function<void(Actor&, LuaState&)> removeFromActor;
        };

        // Doing it this way instead of just having a static field makes sure the map is initialized whenever it's needed.
        inline static std::map<std::string, TypeInfo>& getNameToTypeInfoMap() {
            static std::map<std::string, TypeInfo> nameToTypeInfoMap;
            return nameToTypeInfoMap;
        }

        static TypeInfo& getTypeInfoByName(const std::string& typeName);
    };

    template<typename TComponent>
    class LuaTypeRequirement {

    public:

        explicit inline LuaTypeRequirement(const std::string& name) {

            if (isRegistered)
                return;

            ComponentsToLua::registerType<TComponent>(name);
            isRegistered = true;
        }

    private:

        inline static bool isRegistered = false;
    };


    template<typename T>
    inline void ComponentsToLua::registerType(const std::string& name) {

        auto& map = getNameToTypeInfoMap();
        auto it = map.find(name);
        if (it != map.end()) {
            std::cout << "Warning: type " << name << " already registered" << std::endl;
            return;
        }

        TypeInfo& entry = map[name] = {};

        entry.addFromLua = detail::LuaComponentFactoryFunctionOf<T>::get();

        entry.pushFromActor = [](Actor& actor, LuaState& lua) {

            T* componentPtr = actor.tryGet<T>();
            if (componentPtr) {
                lua::push(lua, ComponentReference<T>(actor.getEngine().getRegistry(), actor));
            } else {
                lua_pushnil(lua);
            }
        };

        entry.addToActor = [](Actor& actor, LuaState& lua) {

            if (actor.tryGet<T>()) {
                luaL_error(lua, "Actor %s already has a component of type %s", actor.getName().getString().c_str(), utils::demangle<T>().c_str());
            }

            actor.add<T>();
            lua::push(lua, ComponentReference<T>(actor.getEngine().getRegistry(), actor));
        };

        entry.addToActorFromDefinition = [](Actor& actor, LuaState& lua, int componentDefinitionIndex) {

            if (actor.tryGet<T>()) {
                luaL_error(lua, "Actor %s already has a component of type %s", actor.getName().getString().c_str(), utils::demangle<T>().c_str());
            }

            lua_pushvalue(lua, componentDefinitionIndex);
            detail::LuaComponentFactoryFunctionOf<T>::get()(actor, lua);
            lua::push(lua, ComponentReference<T>(actor.getEngine().getRegistry(), actor));
        };

        entry.removeFromActor = [](Actor& actor, LuaState& lua) {

            if (!actor.tryGet<T>()) {
                return;
            }

            actor.remove<T>();
        };
    }
}

#endif //ENGINE_COMPONENTSTOLUA_H
