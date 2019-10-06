//
// Created by Maksym Maisak on 4/11/18.
//

#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#include "HashedString.h"

namespace en {

    /// Pretty much global state. Intended for configuration only.
    class State {

    public:
        template<typename T, HashedString::hash_type Id>
        inline static T& value() { return m_value<T, Id>; }

    private:
        template<typename T, HashedString::hash_type Id>
        inline static T m_value = {};
    };
}


#endif //ENGINE_STATE_H
