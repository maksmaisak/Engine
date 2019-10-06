//
// Created by Maksym Maisak on 21/10/18.
//

#ifndef ENGINE_ACTOR_H
#define ENGINE_ACTOR_H

// The declaration is in ActorDecl.h to prevent an include loop involving Engine.h
#include "ActorDecl.h"
#include "Engine.h"

namespace en {

    template<typename T>
    constexpr inline bool isBehavior = std::is_base_of<class Behavior, T>::value;

    template<typename TComponent, typename... Args>
    TComponent& Actor::add(Args&&... args) {

        if constexpr (isBehavior<TComponent>) {

            m_engine->getSystems().ensureBehaviorSystem<TComponent>();
            return m_registry->add<TComponent>(m_entity, *this, std::forward<Args>(args)...);

        } else {

            return m_registry->add<TComponent>(m_entity, std::forward<Args>(args)...);
        }
    }

    template<typename TComponent, typename... Args>
    TComponent& Actor::getOrAdd(Args&&... args) {

        if constexpr (isBehavior<TComponent>) {

            m_engine->getSystems().ensureBehaviorSystem<TComponent>();
            return m_registry->getOrAdd<TComponent>(m_entity, *this, std::forward<Args>(args)...);

        } else {

            return m_registry->getOrAdd<TComponent>(m_entity, std::forward<Args>(args)...);
        }
    }
}


#endif //ENGINE_ACTOR_H
