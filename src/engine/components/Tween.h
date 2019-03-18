//
// Created by Maksym Maisak on 2019-03-04.
//

#ifndef SAXION_Y2Q2_RENDERING_TWEEN_H
#define SAXION_Y2Q2_RENDERING_TWEEN_H

#include <functional>
#include <optional>

#include "Ease.h"
#include "EntityRegistry.h"
#include "ComponentReference.h"

namespace en {

    struct Tween {

        enum class LoopingBehavior {

            Single = 0,
            Repeat,
            Bounce,

            Count
        };

        static void initializeMetatable(LuaState& lua);
        static ComponentReference<Tween> make(
            EntityRegistry& registry,
            Entity target,
            const std::optional<float>& duration,
            const std::optional<ease::Ease>& ease,
            const std::function<void(float)>& set
        );

        void complete();

        Entity target = nullEntity;
        float duration = 1.f;
        std::function<float(float)> ease = ease::inOutQuad;
        std::function<void(float)>  set;

        float progress = 0.f;
        bool isKillPending     = false;
        bool isPaused          = false;
        bool playsBackward     = false;
        LoopingBehavior loopingBehavior = LoopingBehavior::Single;
    };
}

#endif //SAXION_Y2Q2_RENDERING_TWEEN_H
