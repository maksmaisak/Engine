//
// Created by Maksym Maisak on 30/9/19.
//

#include "ExposeToLua.h"
#include "Engine.h"
#include "ComponentsToLua.h"
#include "MetatableHelper.h"
#include "ClosureHelper.h"
#include "Material.h"

#include "KeyboardHelper.h"
#include "MouseHelper.h"
#include "Sound.h"
#include "MusicIntegration.h"
#include "Ease.h"
#include "GameTime.h"
#include "LuaScene.h"
#include "Actor.h"

#include "Transform.h"
#include "LuaBehavior.h"
#include "UIRect.h"
#include "UISprite.h"

using namespace en;

namespace {

    int makeActors(lua_State* L) {

        luaL_checktype(L, 1, LUA_TTABLE);
        Engine& engine = *lua::check<Engine*>(L, lua_upvalueindex(1));
        ComponentsToLua::makeEntities(L, engine, 1);

        return 0;
    }

    int makeActorFromLua(lua_State* L) {

        Engine& engine = *lua::check<Engine*>(L, lua_upvalueindex(1));

        // makeActor(table)
        if (lua_istable(L, 1)) {

            Actor actor = ComponentsToLua::makeEntity(L, engine, 1);
            ComponentsToLua::addComponents(L, actor, 1);
            lua::push(L, actor);

            return 1;
        }

        // makeActor(name, [table])
        Actor actor = engine.makeActor(luaL_checkstring(L, 1));
        if (lua_istable(L, 2))
            ComponentsToLua::addComponents(L, actor, 2);
        lua::push(L, actor);
        return 1;
    }

    int makeMaterial(lua_State* L) {

        auto lua = LuaState(L);

        std::shared_ptr<Material> material;
        if (lua.is<std::string>(1)) {

            if (lua_isnoneornil(L, 2)) {
                material = Resources<Material>::get(lua.to<std::string>(1));
            } else {
                lua_pushvalue(L, 2);
                material = Resources<Material>::get(lua.to<std::string>(1), lua);
            }
        } else {

            if (lua_isnoneornil(L, 1)) {
                material = std::make_shared<Material>("lit");
            } else {
                material = std::make_shared<Material>(lua);
            }
        }

        lua.push(std::move(material));
        return 1;
    }

    int loadScene(lua_State* L) {

        Engine* engine = lua::check<Engine*>(L, lua_upvalueindex(1));

        if (lua::is<std::string>(L, 1)) {

            std::string path = lua::to<std::string>(L, 1);
            engine->getSceneManager().setCurrentSceneNextUpdate<LuaScene>(engine->getLuaState(), path);

        } else if (lua_istable(L, 1)) {

            lua_pushvalue(L, 1);
            engine->getSceneManager().setCurrentSceneNextUpdate<LuaScene>(LuaReference(L));
        }

        return 0;
    }

    void exposeGameWindow(Engine& engine, LuaState& lua) {

        lua_pushvalue(lua, -1);
        lua_newtable(lua);
        {
            lua.setField("getSize", [&window = engine.getWindow()]() -> glm::vec2 {
                auto size = window.getSize();
                return {size.x, size.y};
            });
        }
        lua_setfield(lua, -2, "window");
    }

    void exposeGameAudio(Engine& engine, LuaState& lua) {

        lua_pushvalue(lua, -1);
        lua_newtable(lua);
        {
            lua.setField("makeSound", [](const std::string& filepath) {
                auto sound = std::make_shared<Sound>(config::ASSETS_PATH + filepath);
                return sound->isValid() ? std::make_optional(sound) : std::nullopt;
            });

            lua.setField("getSound", [](const std::string& filepath) {
                auto sound = Resources<Sound>::get(config::ASSETS_PATH + filepath);
                return sound->isValid() ? std::make_optional(sound) : std::nullopt;
            });

            lua.setField("makeMusic", [](const std::string& filepath) {
                auto ptr = ResourceLoader<sf::Music>::load(config::ASSETS_PATH + filepath);
                return ptr ? std::make_optional(ptr) : std::nullopt;
            });

            lua.setField("getMusic", [](const std::string& filepath) {
                auto ptr = Resources<sf::Music>::get(config::ASSETS_PATH + filepath);
                return ptr ? std::make_optional(ptr) : std::nullopt;
            });

            lua.setField("stopAll", []() {

                std::size_t count = 0;

                std::for_each(Resources<Sound>::begin(), Resources<Sound>::end(), [&count](const auto& pair) {
                    pair.second->getUnderlyingSound().stop();
                    count += 1;
                });

                std::for_each(Resources<sf::Music>::begin(), Resources<sf::Music>::end(), [&count](const auto& pair) {
                    pair.second->stop();
                    count += 1;
                });

                return count;
            });
        }
        lua_setfield(lua, -2, "audio");
    }

    void exposeGameKeyboard(Engine& engine, LuaState& lua) {

        // Game.keyboard
        lua_pushvalue(lua, -1);
        lua_newtable(lua);
        {
            lua.setField("isHeld", static_cast<bool(*)(const std::string&)>(utils::KeyboardHelper::isHeld));
            lua.setField("isDown", static_cast<bool(*)(const std::string&)>(utils::KeyboardHelper::isDown));
            lua.setField("isUp"  , static_cast<bool(*)(const std::string&)>(utils::KeyboardHelper::isUp));
        }
        lua_setfield(lua, -2, "keyboard");
    }

    void exposeGameMouse(Engine& engine, LuaState& lua) {

        // Game.mouse
        lua_pushvalue(lua, -1);
        lua_newtable(lua);
        {
            lua.setField("isHeld", [](int buttonNum){return utils::MouseHelper::isHeld((sf::Mouse::Button)(buttonNum - 1));});
            lua.setField("isDown", [](int buttonNum){return utils::MouseHelper::isDown((sf::Mouse::Button)(buttonNum - 1));});
            lua.setField("isUp"  , [](int buttonNum){return utils::MouseHelper::isUp  ((sf::Mouse::Button)(buttonNum - 1));});
        }
        lua_setfield(lua, -2, "mouse");
    }

    void exposeGame(Engine& engine, LuaState& lua) {

        lua_newtable(lua);
        {
            lua.setField("find", [&engine](const std::string& name) -> std::optional<Actor> {

                if (Actor actor = engine.findByName(name)) {
                    return std::make_optional(actor);
                }

                return std::nullopt;
            });

            lua.push(&engine);
            lua_pushcclosure(lua, &makeActors, 1);
            lua_setfield(lua, -2, "makeActors");

            lua.push(&engine);
            lua_pushcclosure(lua, &makeActorFromLua, 1);
            lua_setfield(lua, -2, "makeActor");

            lua.setField("getTime", [](){return GameTime::nowAsSeconds();});

            lua.push(&engine);
            lua_pushcclosure(lua, &loadScene, 1);
            lua_setfield(lua, -2, "loadScene");

            lua.setField("quit", [&engine](){engine.quit();});

            // TODO make addProperty work on both tables and their metatables
            //lua::addProperty(lua, "time", lua::Property<float>([](){return GameTime::now().asSeconds();}));

            lua.setField("makeMaterial", &makeMaterial);

            exposeGameWindow(engine, lua);
            exposeGameAudio(engine, lua);
            exposeGameKeyboard(engine, lua);
            exposeGameMouse(engine, lua);
        }
        lua_setglobal(lua, "Game");
    }

    void exposeEase(Engine& engine, LuaState& lua) {

        lua_pushvalue(lua, -1);
        lua_newtable(lua);
        {
            static const auto setFieldAsUserdata = [&lua](const std::string& name, const ease::Ease& ease){
                lua.push(ease);
                lua_setfield(lua, -2, name.c_str());
            };

            setFieldAsUserdata("linear", ease::linear);

            setFieldAsUserdata("inQuad" , ease::inQuad );
            setFieldAsUserdata("inCubic", ease::inCubic);
            setFieldAsUserdata("inQuart", ease::inQuart);
            setFieldAsUserdata("inQuint", ease::inQuint);
            setFieldAsUserdata("inExpo" , ease::inExpo );
            setFieldAsUserdata("inCirc" , ease::inCirc );
            setFieldAsUserdata("inSine" , ease::inSine );

            setFieldAsUserdata("outQuad" , ease::outQuad );
            setFieldAsUserdata("outCubic", ease::outCubic);
            setFieldAsUserdata("outQuart", ease::outQuart);
            setFieldAsUserdata("outQuint", ease::outQuint);
            setFieldAsUserdata("outExpo" , ease::outExpo );
            setFieldAsUserdata("outCirc" , ease::outCirc );
            setFieldAsUserdata("outSine" , ease::outSine );

            setFieldAsUserdata("inOutQuad ", ease::inOutQuad );
            setFieldAsUserdata("inOutCubic", ease::inOutCubic);
            setFieldAsUserdata("inOutQuart", ease::inOutQuart);
            setFieldAsUserdata("inOutQuint", ease::inOutQuint);
            setFieldAsUserdata("inOutExpo ", ease::inOutExpo );
            setFieldAsUserdata("inOutCirc ", ease::inOutCirc );
            setFieldAsUserdata("inOutSine" , ease::inOutSine );

            setFieldAsUserdata("punch"    , ease::punch);
            setFieldAsUserdata("fluctuate", ease::fluctuate);
        }
        lua_setglobal(lua, "Ease");
    }
}

void ExposeToLua::expose(Engine& engine) {

    auto& lua = engine.getLuaState();
    exposeGame(engine, lua);
    exposeEase(engine, lua);
}
