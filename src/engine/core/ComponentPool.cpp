//
// Created by Maksym Maisak on 26/10/18.
//

#include "ComponentPool.h"
#include <limits>

namespace en {

    bool ComponentPoolBase::contains(en::Entity entity) const {

        const auto id = getId(entity);
        if (id >= m_entityIdToIndex.size()) return false;
        const index_type index = m_entityIdToIndex[id];
        if (index >= m_indexToEntity.size()) return false;
        return m_indexToEntity[index] == entity;
    }

    ComponentPoolBase::index_type ComponentPoolBase::insert(en::Entity entity) {

        assert(!contains(entity) && "Entity already has a component of this type!");

        const auto id = getId(entity);
        if (id >= m_entityIdToIndex.size()) {
            m_entityIdToIndex.resize(id + 1);
        }

        const index_type index = m_indexToEntity.size();
        m_entityIdToIndex[id] = index;
        m_indexToEntity.push_back(entity);
        return index;
    }

    bool ComponentPoolBase::remove(en::Entity entity) {

        return removeInternal(entity) != nullIndex;
    }

    void ComponentPoolBase::clear() {

        m_indexToEntity.clear();
        m_entityIdToIndex.clear();
    }

    ComponentPoolBase::index_type ComponentPoolBase::removeInternal(en::Entity entity) {

        if (!contains(entity)) return nullIndex;

        const auto id = getId(entity);
        assert(id < m_entityIdToIndex.size());

        const index_type index = m_entityIdToIndex[id];
        const en::Entity lastEntity = m_indexToEntity.back();

        // Swap and pop.
        // Overwrite the entity with the last entity, adjust the indices,
        // then remove the last entity, which is now duplicate.
        m_indexToEntity[index] = lastEntity;
        m_entityIdToIndex[getId(lastEntity)] = index;
        m_entityIdToIndex[id] = nullIndex;
        m_indexToEntity.pop_back();

        return index;
    }
}
