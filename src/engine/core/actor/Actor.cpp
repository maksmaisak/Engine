//
// Created by Maksym Maisak on 21/10/18.
//

#include "Actor.h"
#include "Engine.h"
#include "Transform.h"
#include "Name.h"
#include "LuaState.h"
#include "Destroy.h"
#include "Tween.h"

using namespace en;

namespace {

    int pushByTypeName(lua_State* L) {

        auto actor = lua::check<Actor>(L, 1);
        if (!actor)
            return 0;

        auto name = lua::check<std::string>(L, 2);
        ComponentsToLua::pushComponentReferenceByTypeName(L, actor, name);

        return 1;
    }

    int pushByTypeNameWithChildren(lua_State* L) {

        auto actor = lua::check<Actor>(L, 1);
        if (!actor)
            return 0;

        auto name = lua::check<std::string>(L, 2);
        ComponentsToLua::pushComponentReferenceByTypeNameWithChildren(L, actor, name);

        return 1;
    }

    int addByTypeName(lua_State* L) {

        auto& actor = lua::check<Actor>(L, 1);
        if (!actor)
            return 0;

        auto name = lua::check<std::string>(L, 2);
        if (lua_isnoneornil(L, 3))
            ComponentsToLua::addComponentByTypeName(L, actor, name);
        else
            ComponentsToLua::addComponentByTypeName(L, actor, name, 3);

        return 1;
    }

    int removeByTypeName(lua_State* L) {

        auto& actor = lua::check<Actor>(L, 1);
        if (!actor)
            return 0;

        auto name = lua::check<std::string>(L, 2);
        ComponentsToLua::removeComponentByTypeName(L, actor, name);

        return 0;
    }

    template<typename TComponent>
    TComponent* tryGetInChildren(const EntityRegistry& registry, Entity e) {

        auto* component = registry.tryGet<TComponent>(e);
        if (component)
            return component;

        if (auto* tf = registry.tryGet<Transform>(e))
            for (Entity child : tf->getChildren())
                if (auto* childComponent = tryGetInChildren<TComponent>(registry, child))
                    return childComponent;
    }
}

Actor::Actor(Engine& engine, Entity entity) :
    m_engine(&engine),
    m_registry(&engine.getRegistry()),
    m_entity(entity)
{}

std::string Actor::getName() const {

    const Name* ptr = tryGet<Name>();
    return ptr ? ptr->value : "unnamed";
}

void Actor::destroy() {

    if (*this && !tryGet<Destroy>()) {
        add<Destroy>();
    }
}

void Actor::destroyImmediate() {

    m_registry->destroy(m_entity);
}

void Actor::initializeMetatable(LuaState& lua) {

    lua.setField("get", &pushByTypeName);
    lua.setField("getInChildren", &pushByTypeNameWithChildren);
    lua.setField("add", &addByTypeName);
    lua.setField("remove", &removeByTypeName);
    lua.setField("destroy", &Actor::destroy);
    lua.setField("destroyImmediate", &Actor::destroyImmediate);

    lua::addProperty(lua, "name", lua::property(
        [](Actor& actor) {
            Name* ptr = actor.tryGet<Name>();
            return ptr ? std::make_optional(ptr->value) : std::nullopt;
        },
        [](Actor& actor, const std::string& newName) {
            Name* nameComponent = actor.tryGet<Name>();
            if (nameComponent) {
                nameComponent->value = newName;
            } else {
                actor.add<Name>(newName);
            }
        }
    ));

    lua::addProperty(lua, "isValid", lua::readonlyProperty([](Actor& actor) -> bool {
        return actor;
    }));

    lua::addProperty(lua, "isDestroyed", lua::readonlyProperty([](Actor& actor) -> bool {
        return !actor || actor.tryGet<Destroy>();
    }));

    static const auto makeAllTweenModifier = [](const std::function<void(Tween&)>& action) {

        return [action](const Actor& actor) {

            int count = 0;
            auto& registry = *actor.m_registry;
            const Entity entity = actor.m_entity;

            for (Entity e : registry.with<Tween>()) {

                auto& tween = registry.get<Tween>(e);
                if (tween.target == entity && !registry.tryGet<Destroy>(e)) {
                    action(tween);
                    count += 1;
                }
            }

            return count;
        };
    };

    lua.setField("tweenPause"    , makeAllTweenModifier([](Tween& tween) {tween.isPaused      = true; }));
    lua.setField("tweenPlay"     , makeAllTweenModifier([](Tween& tween) {tween.isPaused      = false;}));
    lua.setField("tweenKill"     , makeAllTweenModifier([](Tween& tween) {tween.isKillPending = true; }));
    lua.setField("tweenComplete" , makeAllTweenModifier([](Tween& tween) {tween.complete();}));
}