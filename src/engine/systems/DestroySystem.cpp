//
// Created by Maksym Maisak on 4/11/18.
//

#include "DestroySystem.h"
#include <algorithm>
#include "Destroy.h"

void en::DestroySystem::update(float dt) {

    auto view = m_registry->with<Destroy>();
    auto entitiesToDestroy = std::vector<Entity>(view.begin(), view.end());
    for (Entity e : entitiesToDestroy) {
        m_registry->destroy(e);
    }
}
