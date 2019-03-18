//
// Created by Maksym Maisak on 4/11/18.
//

#ifndef SAXION_Y2Q1_CPP_STATE_H
#define SAXION_Y2Q1_CPP_STATE_H

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


#endif //SAXION_Y2Q1_CPP_STATE_H
