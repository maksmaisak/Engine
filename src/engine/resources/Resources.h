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

    namespace detail {

        template<typename T, typename... Args>
        struct has_valid_load_function {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
            template<typename T_>
            static auto test(int) -> std::void_t<decltype(T_::load(std::declval<Args>()...))>;

            template<typename>
            static auto test(...) -> struct dummy;
#pragma clang diagnostic pop

            inline static constexpr bool value = std::is_void_v<decltype(test<T>(0))>;
        };

        template<typename T, typename... Args>
        inline static constexpr bool has_valid_load_function_v = has_valid_load_function<T, Args...>::value;

        template<class T>
        struct dependent_false : std::false_type {};
    }

    template<typename TResource>
    class Resources {

        using map_t = std::unordered_map<Name, std::shared_ptr<TResource>>;
    public:
        using iterator = typename map_t::const_iterator;

        /// Gets a resource by given key.
        /// If not already present, tries create one using a load function.
        /// The load function is one of these, in descending priority:
        /// - load in ResourceLoader<TResource> template specialization.
        /// - static TResource::load, returning a shared_ptr or a raw pointer to TResource.
        /// - the constructor of TResource, if exists.
        /// If no valid load function could be determined, you get a compile error.
        /// If the load function can't be called with the given arguments, it will be called with the key AND the arguments.
        template<typename... Args>
        inline static std::shared_ptr<TResource> get(const Name& key, Args&&... args) {

            map_t& resources = getResourcesMap();

            const auto foundIterator = resources.find(key);
            if (foundIterator != resources.end()) {
                return foundIterator->second;
            }

            const auto [it, didAdd] = resources.emplace(key, makeResource(key, std::forward<Args>(args)...));
            assert(didAdd);
            return it->second;
        }

        /// Removes all resources not referenced elsewhere.
        inline static std::size_t removeUnused() {

            map_t& resources = getResourcesMap();

            std::size_t numRemoved = 0;
            for (auto it = resources.begin(); it != resources.end();) {
                if (it->second.unique()) {
                    it = resources.erase(it);
                    ++numRemoved;
                } else {
                    ++it;
                }
            }

            return numRemoved;
        }

        inline static void clear() {
            getResourcesMap().clear();
        }

        inline static iterator begin() {return getResourcesMap().cbegin();}
        inline static iterator end()   {return getResourcesMap().cend();  }

    private:

        template<typename T>
        inline static constexpr bool false_v = false;

        template<typename... Args>
        inline static std::shared_ptr<TResource> makeResource(const Name& key, Args&&... args) {

            using Loader = ResourceLoader<TResource>;

            if constexpr (detail::has_valid_load_function_v<Loader, decltype(std::forward<Args>(args))...>) {
                return Loader::load(std::forward<Args>(args)...);

            } else if constexpr (detail::has_valid_load_function_v<Loader, decltype(key), decltype(std::forward<Args>(args))...>) {
                return Loader::load(key, std::forward<Args>(args)...);

            } else if constexpr (detail::has_valid_load_function_v<TResource, decltype(std::forward<Args>(args))...>) {
                return TResource::load(std::forward<Args>(args)...);

            } else if constexpr (detail::has_valid_load_function_v<TResource, decltype(key), decltype(std::forward<Args>(args))...>) {
                return TResource::load(key, std::forward<Args>(args)...);

            } else if constexpr (std::is_constructible_v<TResource, decltype(std::forward<Args>(args))...>) {
                return std::make_shared<TResource>(std::forward<Args>(args)...);

            } else if constexpr (std::is_constructible_v<TResource, decltype(key), decltype(std::forward<Args>(args))...>) {
                return std::make_shared<TResource>(key, std::forward<Args>(args)...);

            } else {

                static_assert(
                    false_v<TResource>,
                    "Can't create a resource of this type with the given arguments. "
                    "For this type there must exist at least one of: "
                    "ResourceLoader specialization, "
                    "static 'load' function, "
                    "constructor, "
                    "that takes either the arguments or the key and the arguments."
                );
                return nullptr;
            }
        }

        inline static map_t& getResourcesMap() {
            static map_t resources;
            return resources;
        }
    };
}

#endif //ENGINE_RESOURCES_H
