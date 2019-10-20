//
// Created by Maksym Maisak on 19/12/18.
//

#ifndef ENGINE_RESOURCES_H
#define ENGINE_RESOURCES_H

#include <memory>
#include <unordered_map>
#include <type_traits>
#include <algorithm>
#include "ResourceLoader.h"

namespace en {

    template<typename TLoader, typename = void>
    struct canLoadWithNoArgs : std::false_type {};

    template<typename TLoader>
    struct canLoadWithNoArgs<TLoader, decltype(TLoader::load())> : std::true_type {};

    template<typename TLoader>
    inline constexpr bool canLoadWithNoArgs_v = canLoadWithNoArgs<TLoader>::value;

    template<typename TResource>
    class Resources {

    public:

        using iterator = typename std::unordered_map<Name, std::shared_ptr<TResource>>::const_iterator;

        /// Gets a resource by given key.
        /// If not already present, tries create one using a load function.
        /// The load function is one of these, in descending priority:
        /// - load in ResourceLoader<TResource> template specialization.
        /// - static TResource::load, returning a shared_ptr or a raw pointer to TResource.
        /// - the constructor of TResource, if exists.
        /// If no valid load function could be determined, you get a compile error.
        /// If the load function can't be called with the given arguments, it will be called with the key AND the arguments.
        template<typename TLoader = ResourceLoader<TResource>, typename... Args>
        inline static std::shared_ptr<TResource> get(const Name& key, Args&&... args) {

            const auto foundIterator = m_resources.find(key);
            if (foundIterator != m_resources.end()) {
                return foundIterator->second;
            }

            std::shared_ptr<TResource> resource;

            constexpr bool isLoaderAvailable = !std::is_base_of_v<NoLoader, TLoader>;
            if constexpr (isLoaderAvailable) {

                constexpr bool canLoadWithGivenArgs = sizeof...(Args) > 0 || canLoadWithNoArgs_v<TLoader>;
                if constexpr (canLoadWithGivenArgs) {
                    resource = TLoader::load(std::forward<Args>(args)...);
                } else {
                    resource = TLoader::load(key);
                }

            } else {

                // Fall back to constructor if there is no valid loader.
                if constexpr (std::is_constructible_v<TResource, Args...>) {
                    resource = std::make_shared<TResource>(std::forward<Args>(args)...);
                } else {
                    resource = std::make_shared<TResource>(key, std::forward<Args>(args)...);
                }
            }

            const auto [it, didAdd] = m_resources.emplace(key, std::move(resource));
            assert(didAdd);
            return it->second;
        }

        /// Removes all resources not referenced elsewhere.
        inline static std::size_t removeUnused() {

            std::size_t numRemoved = 0;
            for (auto it = m_resources.begin(); it != m_resources.end();) {
                if (it->second.unique()) {
                    it = m_resources.erase(it);
                    ++numRemoved;
                } else {
                    ++it;
                }
            }

            return numRemoved;
        }

        inline static void clear() {
            m_resources.clear();
        }

        inline static iterator begin() {return m_resources.cbegin();}
        inline static iterator end()   {return m_resources.cend();  }

    private:
        static std::unordered_map<Name, std::shared_ptr<TResource>> m_resources;
    };

    // Using inline static to avoid defining this out of the class body causes clang to segfault :(
    template<typename TResource>
    std::unordered_map<Name, std::shared_ptr<TResource>> Resources<TResource>::m_resources;
}

#endif //ENGINE_RESOURCES_H
