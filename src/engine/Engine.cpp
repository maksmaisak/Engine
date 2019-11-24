//
// Created by Maksym Maisak on 27/9/18.
//
#include <iostream>
#include <algorithm>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"
#include "ExposeToLua.h"
#include "GameTime.h"
#include "Transform.h"
#include "LuaState.h"
#include "ClosureHelper.h"
#include "Resources.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sound.h"
#include "Camera.h"
#include "Name.h"
#include "Font.h"
#include "Keyboard.h"
#include "Mouse.h"

using namespace en;
using namespace std::chrono_literals;

namespace {

    constexpr DurationFloat FixedTimestep = 0.01s;
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
    m_sceneManager(*this),
    m_framerateCap(240),
    m_fps(0.0),
    m_frameTimeMicroseconds(0),
    m_deltaTime(0.f),
    m_deltaTimeRealtime(0.f),
    m_timeScale(1.f),
    m_shouldExit(false)
{
    assert(!g_engine && "Can't have more than one Engine!");
    g_engine = this;
}

Engine::~Engine() {

    Resources<Font>::clear();

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

    DurationFloat fixedUpdateLag = DurationFloat::zero();
    const DurationFloat drawTimestep = 1s / m_framerateCap;

    TimePoint timeOfLastFixedUpdate = Clock::now();
    TimePoint timeOfLastDraw = Clock::now();

    while (!m_shouldExit && !m_window.shouldClose()) {

        // Perform fixed updates if needed
        fixedUpdateLag += (Clock::now() - timeOfLastFixedUpdate) * m_timeScale;
        timeOfLastFixedUpdate = Clock::now();

        fixedUpdateLag = std::min(fixedUpdateLag, FixedTimestep * static_cast<float>(MaxNumFixedUpdatesPerFrame) * std::max(1.f, m_timeScale));
        while (fixedUpdateLag >= FixedTimestep) {

            m_deltaTime = FixedTimestep.count();
            m_deltaTimeRealtime = m_deltaTime / m_timeScale;
            fixedUpdate();

            fixedUpdateLag -= FixedTimestep;
        }

        // Perform draw if needed
        const DurationFloat timeSinceLastDraw = Clock::now() - timeOfLastDraw;
        if (timeSinceLastDraw >= drawTimestep) {

            m_fps = 1.f / timeSinceLastDraw.count();
            timeOfLastDraw = Clock::now();

            const TimePoint timeOfFrameStart = Clock::now();
            draw();
            m_frameTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - timeOfFrameStart).count();

            glfwPollEvents();

        } else {

            while (true) {

                std::this_thread::sleep_for(1us);

                const bool shouldDraw = (Clock::now() - timeOfLastDraw) >= drawTimestep;
                if (shouldDraw) {
                    break;
                }

                const bool shouldUpdate = fixedUpdateLag + (Clock::now() - timeOfLastFixedUpdate) * m_timeScale >= FixedTimestep;
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

    Keyboard::update();
    Mouse::update();
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
