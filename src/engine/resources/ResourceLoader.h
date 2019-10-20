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

    /// Base template
    template<typename TResource, typename = void>
    struct ResourceLoader : NoLoader {};

    // If TResource::load(const Name&) exists and returns either a std::shared_ptr<TResource> or a TResource*
    template<typename TResource>
    struct ResourceLoader<TResource, std::enable_if_t<std::is_invocable_v<decltype(&TResource::load), const Name&>>> {

        inline static std::shared_ptr<TResource> load(const Name& filename) {

            using loadResult_t = std::invoke_result_t<decltype(&TResource::load), const Name&>;

            constexpr bool loadReturnsSharedPtr = std::is_convertible_v<loadResult_t, std::shared_ptr<TResource>>;
            if constexpr (loadReturnsSharedPtr) {
                return TResource::load(filename);
            }

            constexpr bool loadReturnsRawPtr = std::is_convertible_v<loadResult_t, TResource*>;
            if constexpr (loadReturnsRawPtr) {
                return std::shared_ptr<TResource>(TResource::load(filename));
            }

            static_assert(
                loadReturnsSharedPtr || loadReturnsRawPtr,
                "TResource::load(filename) must return either a shared_ptr<TResource> or a raw pointer to TResource!"
            );
            return nullptr;
        }
    };

    /// An example specialization for the sf::Font type
    template<>
    struct ResourceLoader<sf::Font> {

        inline static std::shared_ptr<sf::Font> load(const Name& filename) {

            const auto fontPtr = std::make_shared<sf::Font>();
            const bool didLoadFont = fontPtr->loadFromFile(filename);
            return didLoadFont ? fontPtr : nullptr;
        }
    };
}

#endif //ENGINE_RESOURCELOADER_H
