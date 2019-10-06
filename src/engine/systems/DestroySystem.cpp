//
// Created by Maksym Maisak on 4/11/18.
//

#include "DestroySystem.h"
#include "Destroy.h"

void en::DestroySystem::update(float dt) {

    const auto entities = m_registry->with<Destroy>();
    const auto entitiesToDestroy = std::vector<Entity>(entities.begin(), entities.end());
    for (Entity e : entitiesToDestroy) {
        m_registry->destroy(e);
    }
}
