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
        using id_t = std::hash<std::string>::result_type;

        Name();
        Name(const std::string& string);
        Name(const char* string);

        bool isValid() const;
        const std::string& getString() const;
        Name getUppercase() const;

        operator const std::string&() const;
        operator const char*() const;
        operator bool() const;

        friend bool operator==(const Name& lhs, const Name& rhs);
        friend std::ostream& operator<<(std::ostream& stream, const Name& name);

    private:
        Name(id_t id);

        id_t m_id;
        bool m_isValid;
    };
}

namespace std {

    template<>
    struct hash<en::Name> {

        using argument_type = en::Name;
        using result_type = std::size_t;
        inline result_type operator()(const argument_type& name) const noexcept {
            return name.m_isValid ? name.m_id : 0;
        }
    };
}

#endif //ENGINE_NAME_H
