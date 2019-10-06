//
// Created by Maksym Maisak on 27/10/18.
//

#include "Entities.h"
#include <iostream>
#include <cassert>

namespace en {

    Entity Entities::add() {

        if (m_nextFree != nullEntity) {

            const Entity entity = m_nextFree;
            const auto id = getId(entity);
            m_nextFree = m_entities[id];

            //std::clog << "Reused entity id " << id << " (new version " << getVersion(entity) << ") " << std::endl;
            return m_entities[id] = entity;
        }

        const Entity entity = m_entities.size();
        m_entities.push_back(entity);

        //std::clog << "Added new entity id " << getId(entity) << " (version " << getVersion(entity) << ") " << std::endl;
        return entity;
    }

    void Entities::remove(Entity entity) {

        if (!contains(entity)) {
            return;
        }

        const auto id = getId(entity);
        m_entities[id] = m_nextFree;
        m_nextFree = setVersion(entity, getVersion(entity) + 1);

        //std::clog << "Removed entity id " << id << " (version " << getVersion(entity) << ") " << std::endl;
    }

    void Entities::removeAll() {

        //std::clog << "Removed all entities" << std::endl;

        for (std::size_t id = 1; id < m_entities.size(); ++id)
            if (getId(m_entities[id]) == id)
                remove(m_entities[id]);
    }

    void Entities::reset() {

        //std::clog << "Reset the Entities collection" << std::endl;

        m_entities.clear();
        m_entities.push_back(nullEntity);
        m_nextFree = nullEntity;
    }

    bool Entities::contains(Entity entity) const {

        if (entity == nullEntity) {
            return false;
        }

        const auto id = getId(entity);
        if (id >= m_entities.size()) {
            return false;
        }

        return m_entities[id] == entity;
    }
}
