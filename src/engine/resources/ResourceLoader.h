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

namespace en {

    struct NoLoader {};

    // Base template
    template<typename TResource, typename SFINAEDummy = void>
    struct ResourceLoader : NoLoader {};

    // If TResource::load(const std::string&) exists
    template<typename TResource>
    struct ResourceLoader<TResource, std::enable_if_t<std::is_invocable_v<decltype(&TResource::load), const std::string&>>> {

        inline static std::shared_ptr<TResource> load(const std::string& filename) {

            // If calling TResource::load(filename) returns a shared_ptr, return that.
            // If it returns a regular pointer, wrap that in a shared_ptr and return it.
            if constexpr (std::is_convertible_v<std::invoke_result_t<decltype(&TResource::load), const std::string&>, std::shared_ptr<TResource>>)
                return TResource::load(filename);
            else
                return std::shared_ptr<TResource>(TResource::load(filename));
        }
    };

    template<>
    struct ResourceLoader<sf::Font> {

        inline static std::shared_ptr<sf::Font> load(const std::string& filename) {

            auto fontPtr = std::make_shared<sf::Font>();
            bool didLoadFont = fontPtr->loadFromFile(filename);
            return didLoadFont ? fontPtr : nullptr;
        }
    };
}

#endif //ENGINE_RESOURCELOADER_H
