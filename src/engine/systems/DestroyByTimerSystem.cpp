//
// Created by Maksym Maisak on 25/10/18.
//

#include "DestroyByTimerSystem.h"
#include "EntityRegistry.h"

namespace en {

    void DestroyByTimerSystem::update(float dt) {

        const TimePoint now = Clock::now();

        for (en::Entity e : m_registry->with<DestroyTimer>()) {
            if (m_registry->get<DestroyTimer>(e).timeToDestroy <= now) {
                m_entitiesToDestroy.push_back(e);
            }
        }

        for (en::Entity e : m_entitiesToDestroy) {
            m_registry->destroy(e);
        }
        m_entitiesToDestroy.clear();
    }
}
