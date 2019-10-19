//
// Created by Maksym Maisak on 21/10/18.
//

#include "EntityRegistry.h"
#include <algorithm>
#include "Messaging.h"
#include "EntityEvents.h"
#include "Name.h"

namespace en {

    Entity EntityRegistry::makeEntity() {

        const en::Entity entity = m_entities.add();
        Receiver<EntityCreated>::broadcast({entity});
        return entity;
    }

    Entity EntityRegistry::makeEntity(const Name& name) {

        const en::Entity entity = m_entities.add();
        add<Name>(entity, name);
        Receiver<EntityCreated>::broadcast({entity});
        return entity;
    }

    bool EntityRegistry::isAlive(Entity e) const {

        return m_entities.contains(e);
    }

    Entity EntityRegistry::findByName(const Name& name) const {

        const auto range = with<Name>();
        const auto end = range.end();
        const auto it = std::find_if(range.begin(), end, [&](Entity e) {return get<Name>(e) == name;});
        return it != end ? *it : nullEntity;
    }

    void EntityRegistry::destroy(Entity entity) {

        if (!m_entities.contains(entity)) {
            return;
        }

        Receiver<EntityWillBeDestroyed>::broadcast({entity});
        m_entities.remove(entity);
        for (const std::unique_ptr<ComponentPoolBase>& poolPtr : m_componentPools) {
            if (poolPtr) {
                poolPtr->remove(entity);
                assert(!poolPtr->contains(entity));
            }
        }
    }

    void EntityRegistry::destroyAll() {

        for (Entity e : m_entities) {
            Receiver<EntityWillBeDestroyed>::broadcast({e});
        }

        m_entities.removeAll();
        for (const std::unique_ptr<ComponentPoolBase>& poolPtr : m_componentPools) {
            if (poolPtr) {
                poolPtr->clear();
            }
        }
    }
}