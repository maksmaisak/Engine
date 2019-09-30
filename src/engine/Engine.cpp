//
// Created by Maksym Maisak on 27/9/18.
//
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Gl/glew.h>
#include <SFML/Graphics.hpp>
#include <type_traits>

#include "Engine.h"
#include "Actor.h"
#include "ComponentsToLua.h"
#include "GLHelpers.h"
#include "GameTime.h"
#include "Transform.h"
#include "Name.h"
#include "LuaScene.h"
#include "LuaState.h"
#include "MetatableHelper.h"
#include "ClosureHelper.h"
#include "Resources.h"
#include "Material.h"

#include "KeyboardHelper.h"
#include "MouseHelper.h"
#include "Sound.h"
#include "MusicIntegration.h"
#include "Ease.h"
#include "Camera.h"

using namespace en;

const sf::Time TimestepFixed = sf::seconds(0.01f);
const unsigned int MAX_FIXED_UPDATES_PER_FRAME = 3;

Engine::Engine() :
    m_sceneManager(this),
    m_lua(std::make_unique<LuaState>()) {}

Engine::~Engine() {

    // If sf::SoundBuffer is being destroyed when statics are destroyed when the app quits,
    // that causes OpenAL to throw an error. This prevents that.
    Resources<Sound>::clear();
    Resources<sf::SoundBuffer>::clear();
    Resources<sf::Music>::clear();
}

void Engine::initialize() {

    initializeLua();
    initializeWindow(m_window);
    printGLContextVersionInfo();
    initializeGlew();
    m_systems.init(*this);
}

void Engine::run() {

    m_systems.start();

    const float timestepFixedSeconds = TimestepFixed.asSeconds();
    sf::Clock fixedUpdateClock;
    sf::Time fixedUpdateLag = sf::Time::Zero;

    const sf::Time timestepDraw = sf::microseconds((sf::Int64)(1000000.0 / m_framerateCap));
    sf::Clock drawClock;

    while (m_window.isOpen()) {

        fixedUpdateLag += fixedUpdateClock.restart();
        fixedUpdateLag = std::min(fixedUpdateLag, TimestepFixed * static_cast<float>(MAX_FIXED_UPDATES_PER_FRAME));
        while (fixedUpdateLag >= TimestepFixed) {
            m_deltaTime = timestepFixedSeconds;
            update(m_deltaTime);
            fixedUpdateLag -= TimestepFixed;
        }

        if (drawClock.getElapsedTime() >= timestepDraw) {

            m_fps = (float)(1000000.0 / drawClock.restart().asMicroseconds());
            auto frameClock = sf::Clock();
            draw();
            m_frameTimeMicroseconds = frameClock.getElapsedTime().asMicroseconds();

        } else {

            do sf::sleep(sf::microseconds(1));
            while (drawClock.getElapsedTime() < timestepDraw && fixedUpdateLag + fixedUpdateClock.getElapsedTime() < TimestepFixed);
        }

        processWindowEvents();
    }

    m_sceneManager.closeCurrentScene();
    m_registry.destroyAll();
}

void Engine::update(float dt) {

    m_sceneManager.update(dt);
    m_systems     .update(dt);
    m_scheduler   .update(dt);

    utils::KeyboardHelper::update();
    utils::MouseHelper   ::update();
}

void Engine::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_systems.draw();
    m_window.display();
}

void Engine::initializeWindow(sf::RenderWindow& window) {

    std::cout << "Initializing window..." << std::endl;

    auto& lua = getLuaState();
    lua_getglobal(lua, "Config");
    auto popConfig = lua::PopperOnDestruct(lua);
    unsigned int width      = lua.tryGetField<unsigned int>("width").value_or(800);
    unsigned int height     = lua.tryGetField<unsigned int>("height").value_or(600);
    bool vsync              = lua.tryGetField<bool>("vsync").value_or(true);
    bool fullscreen         = lua.tryGetField<bool>("fullscreen").value_or(false);
    std::string windowTitle = lua.tryGetField<std::string>("windowTitle").value_or("Game");

    auto contextSettings = sf::ContextSettings(24, 8, 8, 4, 5, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug);
    window.create(sf::VideoMode(width, height), windowTitle, fullscreen ? sf::Style::Fullscreen : sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(vsync);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(0);
    window.setActive(true);

    std::cout << "Window initialized." << std::endl << std::endl;
}

void Engine::printGLContextVersionInfo() {

    std::cout << "Context info:" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    const GLubyte* vendor      = glGetString(GL_VENDOR);
    const GLubyte* renderer    = glGetString(GL_RENDERER);
    const GLubyte* version     = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    std::cout << "----------------------------------" << std::endl << std::endl;
}

void Engine::initializeGlew() {

    std::cout << "Initializing GLEW..." << std::endl;

    const GLint glewStatus = glewInit();
    if (glewStatus == GLEW_OK)
        std::cout << "Initialized GLEW: OK" << std::endl;
    else
        std::cerr << "Initialized GLEW: FAILED" << std::endl;
}

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
}

void Engine::initializeLua() {

    auto& lua = getLuaState();

    lua_newtable(lua);
    {
        lua.setField("find", [this](const std::string& name) -> std::optional<Actor> {
            Actor actor = findByName(name);
            if (actor)
                return std::make_optional(actor);
            return std::nullopt;
        });

        lua.push(this);
        lua_pushcclosure(lua, &makeActors, 1);
        lua_setfield(lua, -2, "makeActors");

        lua.push(this);
        lua_pushcclosure(lua, &makeActorFromLua, 1);
        lua_setfield(lua, -2, "makeActor");

        lua.setField("getTime", [](){return GameTime::nowAsSeconds();});

        lua.push(this);
        lua_pushcclosure(lua, &loadScene, 1);
        lua_setfield(lua, -2, "loadScene");

        lua.setField("quit", [this]() { m_shouldExit = true; });

        // TODO make addProperty work on both tables and their metatables
        //lua::addProperty(lua, "time", lua::Property<float>([](){return GameTime::now().asSeconds();}));

        lua.setField("makeMaterial", &makeMaterial);

        // Game.window
        lua_pushvalue(lua, -1);
        lua_newtable(lua);
        {
            lua.setField("getSize", [&window = m_window]() -> glm::vec2 {
                auto size = window.getSize();
                return {size.x, size.y};
            });
        }
        lua_setfield(lua, -2, "window");

        // Game.audio
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

        // Game.keyboard
        lua_pushvalue(lua, -1);
        lua_newtable(lua);
        {
            lua.setField("isHeld", &utils::KeyboardHelper::isHeld);
            lua.setField("isDown", &utils::KeyboardHelper::isDown);
            lua.setField("isUp"  , &utils::KeyboardHelper::isUp);
        }
        lua_setfield(lua, -2, "keyboard");

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
    lua_setglobal(lua, "Game");

    lua_pushvalue(lua, -1);
    lua_newtable(lua);
    {
        static auto setFieldAsUserdata = [&lua](const std::string& name, const ease::Ease& ease){
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

    if (lua.doFileInNewEnvironment(config::SCRIPT_PATH + "config.lua")) {
        lua_setglobal(lua, "Config");
    }

    lua_getglobal(lua, "Config");
    auto pop = PopperOnDestruct(lua);
    m_framerateCap = lua.tryGetField<unsigned int>("framerateCap").value_or(m_framerateCap);
}

void Engine::processWindowEvents() {

    sf::Event event{};
    while (m_window.pollEvent(event)) {

        switch (event.type) {
            case sf::Event::Closed:
                m_shouldExit = true;
                break;
            default:
                break;
        }

        Receiver<sf::Event>::broadcast(event);
    }

    if (m_shouldExit) {
        m_window.close();
    }
}

Actor Engine::actor(Entity entity) const {
    return Actor(*const_cast<Engine*>(this), entity);
}

Actor Engine::makeActor() {
    return actor(m_registry.makeEntity());
}

Actor Engine::makeActor(const std::string& name) {
    return actor(m_registry.makeEntity(name));
}

Actor Engine::findByName(const std::string& name) const {
    return actor(m_registry.findByName(name));
}

Actor Engine::getMainCamera() const {

    return actor(m_registry.with<Camera, Transform>().tryGetOne());
}
