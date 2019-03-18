//
// Created by Maksym Maisak on 2019-03-04.
//

#include "Tween.h"
#include "Destroy.h"

using namespace en;

void Tween::initializeMetatable(LuaState& lua) {

    lua.setField("pause", [](const ComponentReference<Tween>& tween) {tween->isPaused = true;});
    lua.setField("play" , [](const ComponentReference<Tween>& tween) {tween->isPaused = false;});
    lua.setField("kill" , [](const ComponentReference<Tween>& tween) {tween->isKillPending = true;});
    lua.setField("complete", [](const ComponentReference<Tween>& tween) {tween->complete();});

    lua::addProperty(lua, "progress", lua::property(&Tween::progress));
    lua::addProperty(lua, "duration", lua::property(&Tween::duration));
    lua::addProperty(lua, "ease"    , lua::property(&Tween::ease));
    lua::addProperty(lua, "playsBackward", lua::property(&Tween::playsBackward));

    lua.setField("setLoopNone"  , [](const ComponentReference<Tween>& tween){tween->loopingBehavior = Tween::LoopingBehavior::Single;});
    lua.setField("setLoopRepeat", [](const ComponentReference<Tween>& tween){tween->loopingBehavior = Tween::LoopingBehavior::Repeat;});
    lua.setField("setLoopBounce", [](const ComponentReference<Tween>& tween){tween->loopingBehavior = Tween::LoopingBehavior::Bounce;});
}

ComponentReference<Tween> Tween::make(EntityRegistry& registry,
    Entity target,
    const std::optional<float>& duration,
    const std::optional<ease::Ease>& ease,
    const std::function<void(float)>& set
) {

    Entity e = registry.makeEntity();

    registry.add<Tween>(e,
        target,
        duration.value_or(1.f),
        ease.value_or(ease::inOutQuad),
        set
    );

    return ComponentReference<Tween>(registry, e);
}

void Tween::complete() {

    isPaused = false;
    progress = playsBackward ? 0.f : 1.f;
    set(ease(progress));
    isKillPending = true;
}
