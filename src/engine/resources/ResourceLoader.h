//
// Created by Maksym Maisak on 19/12/18.
//

#ifndef ENGINE_RESOURCELOADER_H
#define ENGINE_RESOURCELOADER_H

#include "SFML/Graphics.hpp"
#include <exception>
#include <type_traits>
#include <cassert>
#include <memory>
#include "Name.h"

namespace en {

    struct NoLoader {};

    /// Base template, provide specializations.
    template<typename TResource>
    struct ResourceLoader : NoLoader {};
}

#endif //ENGINE_RESOURCELOADER_H
