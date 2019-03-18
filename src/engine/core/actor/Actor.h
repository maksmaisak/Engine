//
// Created by Maksym Maisak on 21/10/18.
//

#ifndef SAXION_Y2Q1_CPP_ACTOR_H
#define SAXION_Y2Q1_CPP_ACTOR_H

// The declaration is in ActorDecl.h to make it possible to include only the declaration,
// in case you need just the non-templated stuff.
#include "ActorDecl.h"
#include <memory>
#include <vector>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "Engine.h"

namespace en {

    template<typename TComponent, typename... Args>
    TComponent& Actor::add(Args&&... args) {

        if constexpr (!isBehavior<TComponent>) {

            return m_registry->add<TComponent>(m_entity, std::forward<Args>(args)...);

        } else {

            m_engine->ensureBehaviorSystem<TComponent>();
            return m_registry->add<TComponent>(m_entity, *this, std::forward<Args>(args)...);
        }
    }
}


#endif //SAXION_Y2Q1_CPP_ACTOR_H
