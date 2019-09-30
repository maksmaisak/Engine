//
// Created by Maksym Maisak on 4/11/18.
//

#include "TransformHierarchySystem.h"

namespace en {

    void TransformHierarchySystem::receive(const ComponentAdded<Transform>& info) {
        info.component.m_engine = m_engine;
        info.component.m_registry = m_registry;
        info.component.m_entity = info.entity;
    }

    void TransformHierarchySystem::receive(const ComponentWillBeRemoved<Transform>& info) {
        for (Entity child : info.component.m_children) {
            m_registry->destroy(child);
        }
    }
}
