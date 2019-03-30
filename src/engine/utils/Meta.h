//
// Created by Maksym Maisak on 2019-01-17.
//

#ifndef ENGINE_META_H
#define ENGINE_META_H

#include <functional>

namespace utils {

    /// An aide for automatic template parameter deduction.
    template<typename... T>
    struct types {};

    // T without const, volatile, & or &&
    // remove_cvref_t<const std::string&> is std::string
    // Equivalent to std::remove_cvref_t from C++20
    template<typename T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    template<typename T>
    using is_string = std::disjunction<
        std::is_same<std::decay_t<remove_cvref_t<T>>, const char*>,
        std::is_same<std::decay_t<remove_cvref_t<T>>, char*>,
        std::is_same<remove_cvref_t<T>, std::string>
    >;

    template<typename T>
    inline constexpr bool is_string_v = is_string<T>::value;

    template<typename A, typename B = A, typename = void>
    struct is_equatable : std::false_type {};

    template<typename A, typename B>
    struct is_equatable<A, B, std::void_t<decltype(std::declval<A>() == std::declval<B>())>> : std::true_type {};

    template<typename A, typename B = A>
    inline constexpr bool is_equatable_v = is_equatable<A, B>::value;

    template<typename A, typename B = A, typename = void>
    struct equalityComparer {

        inline bool operator()(const A& a, const B& b) {
            return false;
        }
    };

    template<typename A, typename B>
    struct equalityComparer<A, B, std::enable_if_t<is_equatable_v<const A, const B>>> {

        inline bool operator()(const A& a, const B& b) {
            return a == b;
        }
    };

    template<class F>
    struct checkDeducibleSignature {

        template<class G>
        static auto test(int) -> decltype(&G::operator(), void());

        struct dummy {};
        template<class>
        static auto test(...) -> dummy;

        using type = std::is_void<decltype(test<utils::remove_cvref_t<F>>(0))>;
    };

    template<typename F>
    struct hasDeducibleSignature : checkDeducibleSignature<F>::type {};

    template<typename F>
    inline constexpr bool hasDeducibleSignature_v = hasDeducibleSignature<F>::value;

    template<typename TResult, typename TOwner, typename... TArgs>
    struct functionTraitsBase {

        inline static constexpr bool isFunction = true;

        using Result = TResult;
        using Owner  = TOwner;
        using Signature = TResult(TArgs...);
        using FunctionType = std::function<Signature>;
    };

    template<typename T, bool = hasDeducibleSignature_v<T>>
    struct functionTraits {
        inline static constexpr bool isFunction = false;
    };

    template<typename TResult, typename... Args>
    struct functionTraits<TResult(*)(Args...), false> : functionTraitsBase<TResult, void, Args...> {};

    template<typename TResult, typename TOwner, typename... Args>
    struct functionTraits<TResult(TOwner::*)(Args...), false> : functionTraitsBase<TResult, TOwner, Args...> {};

    template<typename TResult, typename... Args>
    struct functionTraits<TResult(Args...), false> : functionTraitsBase<TResult, void, Args...> {};

    template<typename TResult, typename TOwner, typename... Args>
    struct functionTraits<TResult(TOwner::*)(Args...) const, false> : functionTraitsBase<TResult, TOwner, Args...> {};

    template<typename TResult, typename... Args>
    struct functionTraits<TResult(Args...) const, false> : functionTraitsBase<TResult, void, Args...> {};

    template<typename F>
    struct functionTraits<F, true> : functionTraits<typename functionTraits<decltype(&F::operator())>::Signature> {};
}

#endif //ENGINE_META_H
