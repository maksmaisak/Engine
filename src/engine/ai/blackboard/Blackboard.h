//
// Created by Maksym Maisak on 12/10/19.
//

#ifndef ENGINE_BLACKBOARD_H
#define ENGINE_BLACKBOARD_H

#include <unordered_map>
#include <tuple>
#include <optional>
#include "ActorDecl.h"
#include "glm.h"
#include "TupleUtils.h"
#include "Grid.h"
#include "Name.h"

namespace ai {

    class Blackboard final {

    public:

        template<typename T>
        inline std::optional<T> get(const en::Name& key) const {

            const auto& keyToValueMap = getKeyToValueMap<T>();
            const auto it = keyToValueMap.find(key);
            if (it != keyToValueMap.end()) {
                return it->second;
            }

            return std::nullopt;
        }

        template<typename T>
        inline void set(const en::Name& key, const T& value) {

            auto& keyToValueMap = getKeyToValueMap<T>();
            keyToValueMap[key] = value;
        }

        template<typename T>
        inline bool unset(const en::Name& key) {

            auto& keyToValueMap = getKeyToValueMap<T>();
            const auto it = keyToValueMap.find(key);
            if (it != keyToValueMap.end()) {
                keyToValueMap.erase(it);
                return true;
            }

            return false;
        }

    private:

        template<typename T>
        using KeyToValueMap = std::unordered_map<en::Name, T>;

        template<typename... T>
        using KeyToValueMaps = std::tuple<KeyToValueMap<T>...>;

        template<typename T>
        inline KeyToValueMap<T>& getKeyToValueMap() {

            static_assert(
                has_type_v<KeyToValueMap<T>, decltype(m_keyToValueMaps)>,
                "Blackboard can't have values of this type."
            );

            return std::get<KeyToValueMap<T>>(m_keyToValueMaps);
        }

        template<typename T>
        inline const KeyToValueMap<T>& getKeyToValueMap() const {

            static_assert(
                has_type_v<KeyToValueMap<T>, decltype(m_keyToValueMaps)>,
                "Blackboard can't have values of this type."
            );

            return std::get<KeyToValueMap<T>>(m_keyToValueMaps);
        }

        KeyToValueMaps<
            bool,
            int,
            glm::vec3,
            en::GridPosition,
            en::Actor
        > m_keyToValueMaps;
    };
}

#endif //ENGINE_BLACKBOARD_H
