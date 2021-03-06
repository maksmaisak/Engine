//
// Created by Maksym Maisak on 21/10/18.
//

#ifndef ENGINE_ENTITYREGISTRY_H
#define ENGINE_ENTITYREGISTRY_H

#include <memory>
#include <utility>
#include <set>
#include <iostream>
#include <string>
#include "Entity.h"
#include "ComponentPool.h"
#include "EntitiesView.h"
#include "Messaging.h"
#include "EntityEvents.h"
#include "Entities.h"
#include "CustomTypeIndex.h"

namespace en {

    /// Manages entities and components
    /// Create, destroy entities.
    /// Add, get and remove components.
    /// Example:
    /// Iterate through all entities which have a given set of components:
    /// for (Entity e : registry.with<Transform, RenderInfo>()) {}
    class EntityRegistry {

    public:
        en::Entity makeEntity();
        en::Entity makeEntity(const std::string& name);
        /// Unsafe while iterating over entities. Add a marker component like en::Destroy instead.
        void destroy(Entity entity);
        void destroyAll();

        template<class TComponent, typename... Args>
        inline TComponent& add(Entity entity, Args&&... args);

        template<class TComponent, typename... Args>
        inline TComponent& getOrAdd(Entity entity, Args&&... args);

        template<class TComponent>
        inline TComponent& get(Entity entity) const;

        template<class TComponent>
        inline TComponent* tryGet(Entity entity) const;

        template<class TComponent>
        inline bool remove(Entity entity);

        /// Returns an iterable collection of entities that all have components of the specified types.
        template<typename... TComponent>
        inline EntitiesView<TComponent...> with() const;

        bool isAlive(Entity e) const;
        Entity findByName(const std::string& name) const;

    private:

        template<typename TComponent>
        inline ComponentPool<TComponent>& getPool(bool mustBePresentAlready = false) const;

        Entities m_entities;

        using ComponentTypeIndices = utils::CustomTypeIndex<struct ComponentIndicesDummy>;
        mutable std::vector<std::unique_ptr<ComponentPoolBase>> m_componentPools;
    };

    template<class TComponent, typename... Args>
    inline TComponent& EntityRegistry::add(Entity entity, Args&& ... args) {

        ComponentPool<TComponent>& pool = getPool<TComponent>();

        auto [index, componentReference] = pool.insert(entity, std::forward<Args>(args)...);
        Receiver<ComponentAdded<TComponent>>::broadcast({entity, componentReference});

        return componentReference;
    }

    template<class TComponent, typename... Args>
    TComponent& EntityRegistry::getOrAdd(Entity entity, Args&& ... args) {

        ComponentPool<TComponent>& pool = getPool<TComponent>();
        if (TComponent* ptr = pool.tryGet(entity)) {
            return *ptr;
        }

        auto [index, componentReference] = pool.insert(entity, std::forward<Args>(args)...);
        Receiver<ComponentAdded<TComponent>>::broadcast({entity, componentReference});

        return componentReference;
    }

    template<class TComponent>
    inline TComponent& EntityRegistry::get(const en::Entity entity) const {

        ComponentPool<TComponent>& pool = getPool<TComponent>(true);
        return pool.get(entity);
    }

    template<class TComponent>
    inline TComponent* EntityRegistry::tryGet(Entity entity) const {

        ComponentPool<TComponent>& pool = getPool<TComponent>();
        return pool.tryGet(entity);
    }

    template<typename... TComponent>
    inline EntitiesView<TComponent...> EntityRegistry::with() const {

        return EntitiesView<TComponent...>(getPool<TComponent>()...);
    }

    template<class TComponent>
    inline bool EntityRegistry::remove(Entity entity) {

        return getPool<TComponent>(true).remove(entity);
    }

    template<typename TComponent>
    inline ComponentPool<TComponent>& EntityRegistry::getPool(bool mustBePresentAlready) const {

        const std::size_t index = ComponentTypeIndices::index<TComponent>;

        if (index >= m_componentPools.size()) {
            m_componentPools.resize(index + 1);
        }

        if (!m_componentPools[index]) {

            assert(!mustBePresentAlready);
            m_componentPools[index] = std::make_unique<ComponentPool<TComponent>>();
            //std::clog << "Created a component pool (index " << index << ") for " << typeid(TComponent).name() << std::endl;
        }

        return *static_cast<ComponentPool<TComponent>*>(m_componentPools[index].get());
    }
}

#endif //ENGINE_ENTITYREGISTRY_H
