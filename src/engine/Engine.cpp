//
// Created by Maksym Maisak on 27/9/18.
//
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"
#include "ExposeToLua.h"
#include "GameTime.h"
#include "Transform.h"
#include "LuaState.h"
#include "ClosureHelper.h"
#include "Resources.h"
#include "KeyboardHelper.h"
#include "MouseHelper.h"
#include "Sound.h"
#include "Camera.h"
#include "Name.h"

using namespace en;

namespace {

    constexpr float FixedTimestep = 0.01f;
    constexpr unsigned int MaxNumFixedUpdatesPerFrame = 3;

    Engine* g_engine = nullptr;

    void printGLContextVersionInfo() {

        std::cout << "Context info:" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        const GLubyte* const vendor      = glGetString(GL_VENDOR);
        const GLubyte* const renderer    = glGetString(GL_RENDERER);
        const GLubyte* const version     = glGetString(GL_VERSION);
        const GLubyte* const glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

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

    void initializeGlew() {

        std::cout << "Initializing GLEW..." << std::endl;

        const GLint glewStatus = glewInit();
        if (glewStatus == GLEW_OK) {
            std::cout << "Initialized GLEW: OK" << std::endl;
        } else {
            std::cerr << "Initialized GLEW: FAILED" << std::endl;
        }
    }
}

Engine& Engine::get() {

    assert(g_engine);
    return *g_engine;
}

Engine::Engine() :
    m_lua(std::make_unique<LuaState>()),
    m_systems(*this),
    m_sceneManager(*this)
{
    assert(!g_engine && "Can't have more than one Engine!");
    g_engine = this;
}

Engine::~Engine() {

    // If sf::SoundBuffer is being destroyed when statics are destroyed when the app quits,
    // that causes OpenAL to throw an error. This prevents that.
    Resources<Sound>::clear();
    Resources<sf::SoundBuffer>::clear();
    Resources<sf::Music>::clear();
}

void Engine::initialize() {

    initializeLua();
    initializeWindow();
    initializeGlew();
}

void Engine::run() {

    m_systems.start();

    const sf::Time fixedTimestepSf = sf::seconds(FixedTimestep);
    sf::Clock fixedUpdateClock;
    sf::Time fixedUpdateLag = sf::Time::Zero;

    const sf::Time drawTimestepSf = sf::microseconds(1000000.0 / m_framerateCap);
    sf::Clock drawClock;

    while (!m_shouldExit && !m_window.shouldClose()) {

        // Perform fixed updates if needed
        fixedUpdateLag += fixedUpdateClock.restart() * m_timeScale;
        fixedUpdateLag = std::min(fixedUpdateLag, fixedTimestepSf * static_cast<float>(MaxNumFixedUpdatesPerFrame) * std::max(1.f, m_timeScale));
        while (fixedUpdateLag >= fixedTimestepSf) {

            m_deltaTime = FixedTimestep;
            m_deltaTimeRealtime = FixedTimestep / m_timeScale;
            fixedUpdate();

            fixedUpdateLag -= fixedTimestepSf;
        }

        // Perform draw if needed
        if (drawClock.getElapsedTime() >= drawTimestepSf) {

            m_fps = static_cast<float>(1000000.0 / drawClock.restart().asMicroseconds());

            sf::Clock frameClock;
            draw();
            m_frameTimeMicroseconds = frameClock.getElapsedTime().asMicroseconds();
            glfwPollEvents();

        } else {

            while (true) {

                sf::sleep(sf::microseconds(1));

                const bool shouldDraw = drawClock.getElapsedTime() >= drawTimestepSf;
                if (shouldDraw) {
                    break;
                }

                const bool shouldUpdate = fixedUpdateLag + fixedUpdateClock.getElapsedTime() * m_timeScale >= fixedTimestepSf;
                if (shouldUpdate) {
                    break;
                }
            }
        }
    }

    m_sceneManager.closeCurrentScene();
    m_registry.destroyAll();
}

void Engine::quit() {

    m_shouldExit = true;
}

void Engine::fixedUpdate() {

    m_sceneManager.update(m_deltaTime);
    m_systems.update(m_deltaTime);
    m_scheduler.update(m_deltaTime);

    utils::KeyboardHelper::update();
    utils::MouseHelper::update();
}

void Engine::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_systems.draw();
    glfwSwapBuffers(m_window.getUnderlyingWindow());
}

void Engine::initializeLua() {

    ExposeToLua::expose(*this);

    // Execute the config file.
    auto& lua = getLuaState();
    if (lua.doFileInNewEnvironment(config::SCRIPT_PATH + "config.lua")) {
        lua_setglobal(lua, "Config");
    }

    lua_getglobal(lua, "Config");
    auto pop = PopperOnDestruct(lua);
    m_framerateCap = lua.tryGetField<unsigned int>("framerateCap").value_or(m_framerateCap);
}

void Engine::initializeWindow() {

    std::cout << "Initializing window..." << std::endl;

    auto& lua = getLuaState();
    lua_getglobal(lua, "Config");
    const auto popConfig = lua::PopperOnDestruct(lua);

    const int width = lua.tryGetField<int>("width").value_or(800);
    const int height = lua.tryGetField<int>("height").value_or(600);
    const bool vsync = lua.tryGetField<bool>("vsync").value_or(true);
    const bool fullscreen = lua.tryGetField<bool>("fullscreen").value_or(false);
    const std::string windowTitle = lua.tryGetField<std::string>("windowTitle").value_or("Game");

    /*
    const auto contextSettings = sf::ContextSettings(24, 8, 8, 4, 5, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug);
    window.create(sf::VideoMode(width, height), windowTitle, fullscreen ? sf::Style::Fullscreen : sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(vsync);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(0);
    window.setActive(true);*/

    const bool didCreateWindow = m_window.create(width, height, windowTitle, fullscreen);
    if (!didCreateWindow) {
        std::cerr << "Failed to initialize window" << std::endl;
        return;
    }
    m_window.makeCurrent();
    glfwSwapInterval(vsync ? 1 : 0);

    std::cout << "Window initialized." << std::endl << std::endl;

    printGLContextVersionInfo();
}

Actor Engine::actor(Entity entity) const {
    return Actor(*const_cast<Engine*>(this), entity);
}

Actor Engine::makeActor() {
    return actor(m_registry.makeEntity());
}

Actor Engine::makeActor(const Name& name) {
    return actor(m_registry.makeEntity(name));
}

Actor Engine::findByName(const Name& name) const {
    return actor(m_registry.findByName(name));
}

Actor Engine::getMainCamera() const {
    return actor(m_registry.with<Camera, Transform>().tryGetOne());
}
