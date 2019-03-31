//
// Created by Maksym Maisak on 19/12/18.
//

#ifndef ENGINE_RESOURCES_H
#define ENGINE_RESOURCES_H

#include "ResourceLoader.h"
#include <memory>
#include <map>
#include <type_traits>
#include <algorithm>
#include "Texture.hpp"
#include "Model.h"
#include "config.hpp"

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

        using iterator = typename std::map<std::string, std::shared_ptr<TResource>>::const_iterator;

        /// Gets a resource by given key.
        /// If not already present, tries create one using a load function.
        /// The load function is one of these, in order of priority:
        /// - load in ResourceLoader<TResource> template specialization.
        /// - static TResource::load, returning a shared_ptr or a raw pointer to TResource.
        /// - the constructor of TResource, if exists.
        /// If no valid load function could be determined, you get a compile error.
        /// If the load function can't be called with the given arguments, it will be called with the key AND the arguments.
        template<typename TLoader = ResourceLoader<TResource>, typename... Args>
        inline static std::shared_ptr<TResource> get(const std::string& key, Args&&... args) {

            auto it = m_resources.find(key);
            if (it != m_resources.end())
                return it->second;

            bool didAdd = false;

            std::shared_ptr<TResource> resource;

            if constexpr (!std::is_base_of_v<NoLoader, TLoader>) {

                if constexpr (sizeof...(Args) > 0 || canLoadWithNoArgs_v<TLoader>)
                    resource = TLoader::load(std::forward<Args>(args)...);
                else
                    resource = TLoader::load(key);

            } else {

                // Fall back to constructor if there is no valid loader.
                if constexpr (std::is_constructible_v<TResource, Args...>)
                    resource = std::make_shared<TResource>(std::forward<Args>(args)...);
                else
                    resource = std::make_shared<TResource>(key, std::forward<Args>(args)...);
            }

            std::tie(it, didAdd) = m_resources.emplace(key, std::move(resource));
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
        static std::map<std::string, std::shared_ptr<TResource>> m_resources;
    };

    // Using inline static to avoid defining this out of the class body causes clang to segfault :(
    template<typename TResource>
    std::map<std::string, std::shared_ptr<TResource>> Resources<TResource>::m_resources;

    using Models = Resources<Model>;

    struct Textures : Resources<Texture> {

        inline static std::shared_ptr<Texture> white() {return get(config::TEXTURE_PATH + "white.png");}
        inline static std::shared_ptr<Texture> black() {return get(config::TEXTURE_PATH + "black.png");}
        inline static std::shared_ptr<Texture> transparent() {return get(config::TEXTURE_PATH + "transparent.png");}
        inline static std::shared_ptr<Texture> defaultNormalMap() {return get(config::TEXTURE_PATH + "defaultNormalMap.png", GL_RGBA);}
    };
}

#endif //ENGINE_RESOURCES_H
