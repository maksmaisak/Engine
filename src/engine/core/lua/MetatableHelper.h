//
// Created by Maksym Maisak on 3/1/19.
//

#ifndef ENGINE_METATABLE_HELPER_H
#define ENGINE_METATABLE_HELPER_H

#include "LuaState.h"
#include "LuaStack.h"
#include "ClosureHelper.h"
#include "Meta.h"
#include "Demangle.h"
#include <functional>
#include <vector>

namespace lua {

    namespace detail {

        /// The __index function: (table, key) -> value
        /// Try using a property getter from __getters, otherwise look it up in the metatable
        int indexFunction(lua_State* L);

        /// The __index function: (table, key, value) -> ()
        /// Try using a property setter from __setters, otherwise just rawset it
        int newindexFunction(lua_State* L);

        /// Same but just forward the call to a table at the first upvalue.
        int indexFunctionForward(lua_State* L);
        int newindexFunctionForward(lua_State* L);

        template<typename T>
        int onBeforeGarbageCollection(lua_State* L) {

            // If we can get it by reference (true for userdata), do that and call the destructor.
            if constexpr (std::is_reference_v<decltype(lua::check<T>(std::declval<lua_State*>(), 1))>) {

                T& userdata = lua::check<T>(L, 1);
                std::allocator<T> alloc;
                std::allocator_traits<std::allocator<T>>::destroy(alloc, &userdata);
                //std::cout << "Garbage collection: " << utils::demangle<T>() << std::endl;
            }

            return 0;
        }

        template<typename T, typename Owner>
        inline auto makeGetter(T Owner::* memberPtr) {

            using OwnerRef = en::ComponentReference<Owner>;
            return (std::function<T(OwnerRef)>)[memberPtr](OwnerRef owner) {
                return (*owner).*memberPtr;
            };
        }

        template<typename T, typename Owner>
        inline auto makeSetter(T Owner::* memberPtr) {

            using OwnerRef = en::ComponentReference<Owner>;
            return (std::function<T(OwnerRef, const T&)>)[memberPtr](OwnerRef owner, const T& value) {
                return (*owner).*memberPtr = value;
            };
        }
    }

    using InitializeMetatableFunction      = std::function<void(en::LuaState&)>;
    using InitializeEmptyMetatableFunction = std::function<void(en::LuaState&)>;

    template<typename T, typename = void>
    struct InitializeMetatable {

        inline static void initializeMetatable(en::LuaState&) {}
    };

    template<typename T>
    struct InitializeMetatable<std::shared_ptr<T>> {

        inline static void initializeMetatable(en::LuaState& lua) {

            const int ownMetatableIndex = lua_gettop(lua);

            getMetatable<T>(lua);

            lua_pushvalue(lua, -1);
            lua_pushcclosure(lua, detail::indexFunctionForward, 1);
            lua_setfield(lua, ownMetatableIndex, "__index");

            lua_pushvalue(lua, -1);
            lua_pushcclosure(lua, detail::newindexFunctionForward, 1);
            lua_setfield(lua, ownMetatableIndex, "__newindex");

            lua_pop(lua, 1);
        }
    };

    // If static T::initializeMetatable(LuaState&) exists
    template<typename T>
    struct InitializeMetatable<T, std::enable_if_t<std::is_convertible_v<decltype(&std::remove_pointer_t<utils::remove_cvref_t<T>>::initializeMetatable), InitializeMetatableFunction>>> {

        inline static void initializeMetatable(en::LuaState& lua) {

            int oldTop = lua_gettop(lua);
            std::remove_pointer_t<utils::remove_cvref_t<T>>::initializeMetatable(lua);
            int newTop = lua_gettop(lua);
            assert(oldTop == newTop);
        }
    };

    template<typename T, typename = void>
    struct InitializeEmptyMetatable {

        inline static void initializeEmptyMetatable(en::LuaState& lua) {

            const int metatableIndex = lua_gettop(lua);

            lua_newtable(lua);
            lua_pushvalue(lua, -1);
            lua_setfield(lua, metatableIndex, "__getters");
            lua_pushvalue(lua, metatableIndex);
            lua_pushcclosure(lua, &detail::indexFunction, 2);
            lua_setfield(lua, metatableIndex, "__index");

            lua_newtable(lua);
            lua_pushvalue(lua, -1);
            lua_setfield(lua, metatableIndex, "__setters");
            lua_pushcclosure(lua, &detail::newindexFunction, 1);
            lua_setfield(lua, metatableIndex, "__newindex");

            if constexpr (utils::is_equatable_v<const T>) {
                lua.setField("__eq", utils::equalityComparer<T>{}, metatableIndex);
            }

            lua.setField("__gc", &detail::onBeforeGarbageCollection<T>, metatableIndex);

            InitializeMetatable<T>::initializeMetatable(lua);
        }
    };

    // If static T::initializeEmptyMetatable(LuaState&) exists
    template<typename T>
    struct InitializeEmptyMetatable<T, std::enable_if_t<std::is_convertible_v<decltype(&std::remove_pointer_t<utils::remove_cvref_t<T>>::initializeEmptyMetatable), InitializeEmptyMetatableFunction>>> {

        inline static void initializeEmptyMetatable(en::LuaState& lua) {

            using TComponent = std::remove_pointer_t<utils::remove_cvref_t<T>>;

            int oldTop = lua_gettop(lua);
            TComponent::initializeEmptyMetatable(lua);
            int newTop = lua_gettop(lua);
            assert(oldTop == newTop);
        }
    };

    // Gets or adds a metatable for a given type.
    // Returns true if the metatable did not exist before.
    template<typename T>
    inline bool getMetatable(lua_State* L) {

        auto lua = en::LuaState(L);

        if (!luaL_newmetatable(lua, utils::demangle<T>().c_str()))
            return false;

        std::cout << "Created metatable for type " << utils::demangle<T>() << std::endl;
        InitializeEmptyMetatable<T>::initializeEmptyMetatable(lua);
        return true;
    }

    struct NoAccessor {};

    template<typename Getter, typename Setter>
    struct PropertyWrapper {

        inline static constexpr bool hasGetter = !std::is_same_v<Getter, NoAccessor>;
        inline static constexpr bool hasSetter = !std::is_same_v<Setter, NoAccessor>;

        // GetterT and SetterT are expected to be versions of Getter and Setter with various cref modifiers.
        template<typename GetterT, typename SetterT>
        inline PropertyWrapper(GetterT&& getter, SetterT&& setter) :
            m_getter(std::forward<GetterT>(getter)),
            m_setter(std::forward<SetterT>(setter))
        {}

        inline const Getter& getGetter() const {return m_getter;}
        inline const Setter& getSetter() const {return m_setter;}

    private:
        Getter m_getter = {};
        Setter m_setter = {};
    };

    // A type deduction guide.
    template<typename GetterT, typename SetterT>
    PropertyWrapper(GetterT&& getter, SetterT&& setter) ->
        PropertyWrapper<utils::remove_cvref_t<GetterT>, utils::remove_cvref_t<SetterT>>;

    /// Adds a getter and setter, if present, to the __getter and __setter tables in the table on top of stack.
    /// That table is assumed to be a metatable.
    template<typename G, typename S>
    inline void addProperty(lua_State* L, const std::string& name, const PropertyWrapper<G, S>& property) {

        if constexpr (PropertyWrapper<G, S>::hasGetter) {

            auto popGetters = PopperOnDestruct(L);
            lua_getfield(L, -1, "__getters");

            if constexpr (std::is_convertible_v<G, lua_CFunction>)
                lua_pushcfunction(L, property.getGetter());
            else
                ClosureHelper::makeClosure(L, property.getGetter());

            lua_setfield(L, -2, name.c_str());
        }

        if constexpr (PropertyWrapper<G, S>::hasSetter) {

            auto popSetters = PopperOnDestruct(L);
            lua_getfield(L, -1, "__setters");

            if constexpr (std::is_convertible_v<S, lua_CFunction>)
                lua_pushcfunction(L, property.getSetter());
            else
                ClosureHelper::makeClosure(L, property.getSetter());

            lua_setfield(L, -2, name.c_str());
        }
    }

    template<typename Getter, typename Setter>
    inline auto property(Getter&& getter, Setter&& setter) {

        using getterTraits = utils::functionTraits<Getter>;
        using setterTraits = utils::functionTraits<Setter>;
        static_assert(getterTraits::isFunction);
        static_assert(setterTraits::isFunction);
        return PropertyWrapper<typename getterTraits::FunctionType, typename setterTraits::FunctionType>(getter, setter);
    }

    template<typename T, typename Owner>
    inline auto property(T Owner::* memberPtr) {

        using OwnerRef = en::ComponentReference<Owner>;

        return property<std::function<T(OwnerRef)>, std::function<T(OwnerRef, const T&)>>(
            detail::makeGetter<T, Owner>(memberPtr),
            detail::makeSetter<T, Owner>(memberPtr)
        );
    }

    template<typename Getter>
    inline auto readonlyProperty(Getter&& getter) {

        using traits = utils::functionTraits<Getter>;
        static_assert(traits::isFunction);
        return PropertyWrapper<typename traits::FunctionType, NoAccessor>(getter, NoAccessor());
    }

    template<typename T, typename Owner>
    inline auto readonlyProperty(T Owner::* memberPtr) {

        return PropertyWrapper(detail::makeGetter<T, Owner>(memberPtr), NoAccessor());
    }

    template<typename Setter>
    inline auto writeonlyProperty(Setter&& setter) {

        using traits = utils::functionTraits<Setter>;
        static_assert(traits::isFunction);
        return PropertyWrapper<NoAccessor, typename traits::FunctionType>(NoAccessor(), setter);
    }

    template<typename T, typename Owner>
    inline auto writeonlyProperty(T Owner::* memberPtr) {
        return PropertyWrapper(NoAccessor(), detail::makeSetter<T, Owner>(memberPtr));
    }
}

#endif //ENGINE_METATABLE_HELPER_H
