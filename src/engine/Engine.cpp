//
// Created by Maksym Maisak on 27/9/18.
//
#include <iostream>
#include <cmath>
#include <algorithm>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

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

using namespace en;

namespace {

    const sf::Time TimestepFixed = sf::seconds(0.01f);
    const unsigned int MAX_FIXED_UPDATES_PER_FRAME = 3;
}

Engine::Engine() :
    m_lua(std::make_unique<LuaState>()),
    m_systems(*this),
    m_sceneManager(*this)
{}

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

void Engine::quit() {
    m_shouldExit = true;
}

void Engine::update(float dt) {

    m_sceneManager.update(dt);
    m_systems.update(dt);
    m_scheduler.update(dt);

    utils::KeyboardHelper::update();
    utils::MouseHelper::update();
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
    if (glewStatus == GLEW_OK) {
        std::cout << "Initialized GLEW: OK" << std::endl;
    } else {
        std::cerr << "Initialized GLEW: FAILED" << std::endl;
    }
}

void Engine::initializeLua() {

    ExposeToLua::expose(*this);

    auto& lua = getLuaState();
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
