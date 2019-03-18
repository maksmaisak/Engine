//
// Created by Maksym Maisak on 2019-03-04.
//

#include "TweenSystem.h"
#include <vector>
#include "EntityRegistry.h"
#include "Tween.h"
#include "Destroy.h"

using namespace en;

namespace {

    inline void markForDestruction(EntityRegistry& registry, Entity e) {

        if (!registry.tryGet<Destroy>(e))
            registry.add<Destroy>(e);
    }
}

void TweenSystem::update(float dt) {

    for (Entity e : m_registry->with<Tween>()) {

        if (m_registry->tryGet<Destroy>(e))
            continue;

        auto& tween = m_registry->get<Tween>(e);
        if (tween.isKillPending || !m_registry->isAlive(tween.target)) {
            markForDestruction(*m_registry, e);
            return;
        }

        if (!tween.isPaused) {

            bool isFinished;
            if (!tween.playsBackward) {
                tween.progress += dt / tween.duration;
                isFinished = tween.progress >= 1.f;
            } else {
                tween.progress -= dt / tween.duration;
                isFinished = tween.progress <= 0.f;
            }

            if (isFinished) {

                switch (tween.loopingBehavior) {
                    case (Tween::LoopingBehavior::Single):
                        markForDestruction(*m_registry, e);
                        break;
                    case (Tween::LoopingBehavior::Repeat):
                        tween.progress = tween.playsBackward ? 1.f : 0.f;
                        break;
                    case (Tween::LoopingBehavior::Bounce):
                        tween.playsBackward = !tween.playsBackward;
                        break;
                    default:
                        break;
                }
            }
        }

        const float t = tween.ease(tween.progress);
        tween.set(t);
    }
}
