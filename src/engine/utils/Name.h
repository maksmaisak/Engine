//
// Created by Maksym Maisak on 13/10/19.
//

#ifndef ENGINE_NAME_H
#define ENGINE_NAME_H

#include <string>
#include <utility>

namespace en {

    /// An efficient representation of a name.
    /// Uses a 64bit id internally,
    /// but can be freely converted to and from strings using a global table in a thread-safe manner.
    class Name {

        friend struct std::hash<en::Name>;

    public:
        using id_t = std::uint64_t;

        Name();
        Name(const std::string& string);
        Name(const char* string);

        bool isValid() const;
        const std::string& getString() const;

        operator const std::string&() const;
        operator const char*() const;
        operator bool() const;

        friend bool operator==(const Name& lhs, const Name& rhs);
        friend std::ostream& operator <<(std::ostream& stream, const Name& name);

    private:
        id_t m_id;
    };
}

namespace std {

    template<>
    struct hash<en::Name> {

        using argument_type = en::Name;
        using result_type = std::size_t;
        inline result_type operator()(const argument_type& name) const noexcept {
            return std::hash<en::Name::id_t>{}(name.m_id);
        }
    };
}

#endif //ENGINE_NAME_H
