//
// Created by Maksym Maisak on 3/1/19.
//

#ifndef ENGINE_LUACLOSURE_H
#define ENGINE_LUACLOSURE_H

#include "LuaStack.h"
#include <type_traits>
#include <functional>

#include "Demangle.h"
#include "Meta.h"

namespace en {

    template<typename T>
    class ComponentReference;
}

namespace lua {

    template<typename TResult, typename TOwner, typename... TArgs>
    using memberFunctionPtr = TResult(TOwner::*)(TArgs...);

    template<typename TResult, typename... TArgs>
    using functionPtr = TResult(*)(TArgs...);

    /// TODO This needs refactoring.
    /// Handling functions expecting pointers versus references,
    /// same handling for both function pointers and lambdas.
    class ClosureHelper {

    public:

        /// Pushes a C closure out of a given std::function
        /// Handles getting the arguments out of the lua stack and pushing the result onto it.
        template<typename F>
        static inline void makeClosure(lua_State* l, const F& function);

        template<typename TResult, typename... TArgs>
        static inline void makeClosure(lua_State* l, const std::function<TResult(TArgs...)>& function);

        /// Pushes a C closure out of a given function pointer onto the lua stack.
        /// Handles getting the arguments out of the lua stack and pushing the result onto it.
        template<typename TResult, typename... TArgs>
        static inline void makeClosure(lua_State* l, functionPtr<TResult, TArgs...> freeFunction);

        /// Pushes a C closure out of a given member function pointer onto the lua stack.
        /// Handles getting the arguments out of the lua stack and pushing the result onto it.
        template<typename TResult, typename TOwner, typename... TArgs>
        static inline void makeClosure(lua_State* l, memberFunctionPtr<TResult, TOwner, TArgs...> memberFunction, TOwner* typeInstance);

        /// Pushes a C closure out of a given member function pointer onto the lua stack, while getting the *this pointer from the stack.
        /// Handles getting the arguments out of the lua stack and pushing the result onto it.
        template<typename TResult, typename TOwner, typename... TArgs>
        static inline void makeClosure(lua_State* l, memberFunctionPtr<TResult, TOwner, TArgs...> memberFunction);

        template<typename TResult, typename TOwner, typename... TArgs>
        static inline void makeClosure(lua_State* l, TResult(TOwner::*memberFunctionConst)(TArgs...) const) {
            makeClosure(l, const_cast<memberFunctionPtr<TResult, TOwner, TArgs...>>(memberFunctionConst));
        }

        template<typename TResult, typename TOwner, typename... TArgs>
        static inline void makeClosure(lua_State* l, memberFunctionPtr<TResult, TOwner, TArgs...> const memberFunctionConst, const TOwner* typeInstanceConst) {
            makeClosure(l, const_cast<memberFunctionPtr<TResult, TOwner, TArgs...>>(memberFunctionConst), const_cast<TOwner*>(typeInstanceConst));
        }

    private:

        template<typename TResult, typename... TArgs>
        static inline int call(lua_State* l);

        template<typename TResult, typename TOwner, typename... TArgs>
        static inline int callMember(lua_State* l);

        template<typename TResult, typename TOwner, typename... TArgs>
        static inline int callMemberFromStack(lua_State* l);

        template<typename TResult, typename... TArgs>
        static inline int callStdFunction(lua_State* l);

        template<typename F, typename... TArgs>
        static inline int callWithArgsFromStackAndPushResult(lua_State* l, F&& f, utils::types<TArgs...>, int startIndex = 1);

        template<typename F, typename... TArgs>
        static inline decltype(auto) callWithArgsFromStack(lua_State* l, F&& f, int startIndex = 1);

        template<typename F, typename... TArgs, std::size_t... I>
        static inline decltype(auto) callWithArgsFromStackImpl(lua_State* l, F&& f, int startIndex, std::index_sequence<I...>);

        template<typename T>
        static inline bool checkArgType(lua_State* l, int index);

        template<typename TResult>
        static inline void pushResult(lua_State* l, TResult&& result);
    };

    template<typename F>
    void ClosureHelper::makeClosure(lua_State* l, const F& func) {

        using traits = utils::functionTraits<decltype(&utils::remove_cvref_t<F>::operator())>;
        static_assert(traits::isFunction);
        std::function<typename traits::Signature> function = func;
        makeClosure(l, function);
    }

    template<typename TResult, typename... TArgs>
    void ClosureHelper::makeClosure(lua_State* l, const std::function<TResult(TArgs...)>& function) {

        lua::push(l, function);
        lua_pushcclosure(l, &callStdFunction<TResult, TArgs...>, 1);
    }

    template<typename TResult, typename... TArgs>
    void ClosureHelper::makeClosure(lua_State* l, functionPtr<TResult, TArgs...> freeFunction) {

        lua_pushlightuserdata(l, (void*)freeFunction);
        lua_pushcclosure(l, &call<TResult, TArgs...>, 1);
    }

    template<typename TResult, typename TOwner, typename... TArgs>
    void ClosureHelper::makeClosure(lua_State* l, memberFunctionPtr<TResult, TOwner, TArgs...> memberFunction, TOwner* typeInstance) {

        // Put the member function pointer into a full userdata instead of a light userdata
        // because member function pointers for some types may be bigger than a void*.
        lua::push(l, memberFunction);
        lua_pushlightuserdata(l, typeInstance);
        lua_pushcclosure(l, &callMember<TResult, TOwner, TArgs...>, 2);
    }

    template<typename TResult, typename TOwner, typename... TArgs>
    void ClosureHelper::makeClosure(lua_State* l, memberFunctionPtr<TResult, TOwner, TArgs...> memberFunction) {

        lua::push(l, memberFunction);
        lua_pushcclosure(l, &callMemberFromStack<TResult, TOwner, TArgs...>, 1);
    }

    template<typename TResult, typename... TArgs>
    int ClosureHelper::callStdFunction(lua_State* l) {

        using TFunction = std::function<TResult(TArgs...)>;
        auto& function = lua::check<TFunction>(l, lua_upvalueindex(1));
        return callWithArgsFromStackAndPushResult(l, std::forward<TFunction>(function), utils::types<TArgs...>{});
    }

    template<typename TResult, typename... TArgs>
    int ClosureHelper::call(lua_State* l) {

        auto* function = (functionPtr<TResult, TArgs...>)lua_touserdata(l, lua_upvalueindex(1));
        return callWithArgsFromStackAndPushResult(l, function, utils::types<TArgs...>{});
    }

    template<typename TResult, typename TOwner, typename... TArgs>
    int ClosureHelper::callMember(lua_State* l) {

        void* userdataVoidPtr = lua_touserdata(l, lua_upvalueindex(1));
        auto memberFunction = *static_cast<memberFunctionPtr<TResult, TOwner, TArgs...>*>(userdataVoidPtr);
        auto* owner = (TOwner*)lua_touserdata(l, lua_upvalueindex(2));

        using F = std::function<TResult(TArgs...)>;
        F function = std::bind(memberFunction, owner);
        return callWithArgsFromStackAndPushResult(l, std::forward<F>(function), utils::types<TArgs...>{});
    }

    template<typename TResult, typename TOwner, typename... TArgs>
    int ClosureHelper::callMemberFromStack(lua_State* l) {

        void* userdataVoidPtr = lua_touserdata(l, lua_upvalueindex(1));
        auto memberFunction = *static_cast<memberFunctionPtr<TResult, TOwner, TArgs...>*>(userdataVoidPtr);

        TOwner* owner = nullptr;
        if (lua::is<TOwner*>(l, 1)) {
            owner = lua::to<TOwner*>(l, 1);
        } else {
            owner = static_cast<TOwner*>(lua_touserdata(l, 1));
        }

        using F = std::function<TResult(TArgs...)>;
        F function = std::bind(memberFunction, owner);
        return callWithArgsFromStackAndPushResult(l, std::forward<F>(function), utils::types<TArgs...>{}, 2);
    }

    template<typename F, typename... TArgs>
    int ClosureHelper::callWithArgsFromStackAndPushResult(lua_State* l, F&& f, utils::types<TArgs...>, int startIndex) {

        using traits = utils::functionTraits<utils::remove_cvref_t<F>>;
        static_assert(traits::isFunction);

        if constexpr (std::is_void_v<typename traits::Result>) {
            callWithArgsFromStack<F, TArgs...>(l, std::forward<F>(f), startIndex);
            return 0;
        } else {
            pushResult(l, callWithArgsFromStack<F, TArgs...>(l, std::forward<F>(f), startIndex));
            return 1;
        }
    }

    template<typename F, typename... TArgs, std::size_t... I>
    decltype(auto) ClosureHelper::callWithArgsFromStackImpl(lua_State* l, F&& f, int startIndex, std::index_sequence<I...>) {

        (checkArgType<TArgs>(l, startIndex + (int)I), ...);
        return f(lua::to<TArgs>(l, startIndex + (int)I)...);
    }

    template<typename F, typename... TArgs>
    decltype(auto) ClosureHelper::callWithArgsFromStack(lua_State* l, F&& f, int startIndex) {

        return callWithArgsFromStackImpl<F, TArgs...>(l, std::forward<F>(f), startIndex, std::make_index_sequence<sizeof...(TArgs)>{});
    }

    template<typename T>
    bool ClosureHelper::checkArgType(lua_State* l, int index) {

        if (lua::is<T>(l, index))
            return true;

        //std::cerr << "Invalid argument #" + std::to_string(index) + ": expected " + utils::demangle<T>() << std::endl;
        luaL_error(l, "Invalid argument #%d: expected %s", index, utils::demangle<T>().c_str());
        return false;
    }

    template<typename TResult>
    void ClosureHelper::pushResult(lua_State* l, TResult&& result) {
        lua::push(l, std::forward<TResult>(result));
    }
}

#endif //ENGINE_LUACLOSURE_H
