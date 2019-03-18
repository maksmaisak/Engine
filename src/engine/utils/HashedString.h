//
// Adapted from https://github.com/skypjack/entt/blob/6010f5b7a68fbe5444afa7b0c431780727f62e93/src/entt/core/hashed_string.hpp
// by Michele Caini
//

#ifndef SAXION_Y2Q1_CPP_HASHEDSTRING_H
#define SAXION_Y2Q1_CPP_HASHEDSTRING_H

#include <cstddef>
#include <cstdint>

namespace en {

    /**
     * @brief Zero overhead resource identifier.
     *
     * A hashed string is a compile-time tool that allows users to use
     * human-readable identifers in the codebase while using their numeric
     * counterparts at runtime.<br/>
     * Because of that, a hashed string can also be used in constant expressions if
     * required.
     */
    class HashedString final {

        struct const_wrapper final {
            constexpr const_wrapper(const char* str) : str{str} {}

            const char* str;
        };

        static constexpr std::uint64_t offset = 14695981039346656037ull;
        static constexpr std::uint64_t prime = 1099511628211ull;

        // Fowler–Noll–Vo hash function v. 1a - the good
        inline static constexpr std::uint64_t helper(std::uint64_t partial, const char* str) {

            return str[0] == 0 ? partial : helper((partial ^ str[0]) * prime, str + 1);
        }

    public:
        using hash_type = std::uint64_t;

        constexpr HashedString() : hash{}, str{nullptr} {}

        /**
         * @brief Constructs a hashed string from an array of const chars.
         *
         * Forcing template resolution avoids implicit conversions. An
         * human-readable identifier can be anything but a plain, old bunch of
         * characters.<br/>
         * Example of use:
         * @code{.cpp}
         * hashed_string hs{"my.png"};
         * @endcode
         *
         * @tparam N Number of characters of the identifier.
         * @param str Human-readable identifer.
         */
        template<std::size_t N>
        constexpr HashedString(const char (& str)[N]) : hash{helper(offset, str)}, str{str} {}

        explicit constexpr HashedString(const_wrapper wrapper) : hash{helper(offset, wrapper.str)}, str{wrapper.str} {}

        constexpr const char* data() const { return str; }

        constexpr hash_type value() const { return hash; }

        constexpr operator const char*() const { return str; }

        constexpr operator hash_type() const { return hash; }

        constexpr bool operator==(const HashedString& other) const { return hash == other.hash; }

    private:
        const hash_type hash;
        const char* str;
    };
}

constexpr bool operator!=(const en::HashedString& lhs, const en::HashedString& rhs) {
    return !(lhs == rhs);
}

/**
 * @brief User defined literal for hashed strings.
 * @param str The literal without its suffix.
 * @return A properly initialized hashed string.
 */
constexpr en::HashedString operator""_hs(const char* str, std::size_t) {
    return en::HashedString{str};
}


#endif //SAXION_Y2Q1_CPP_HASHEDSTRING_H
