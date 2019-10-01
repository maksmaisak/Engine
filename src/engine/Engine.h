//
// Created by Maksym Maisak on 27/9/18.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <optional>
#include <typeindex>
#include <type_traits>
#include <iostream>
#include "EntityRegistry.h"
#include "EngineSystems.h"
#include "Entity.h"
#include "System.h"
#include "BehaviorSystem.h"
#include "Scheduler.h"
#include "SceneManager.h"
#include "CompoundSystem.h"
#include "KeyboardHelper.h"
#include "MouseHelper.h"

namespace en {

    class Actor;
    class LuaState;

    /// The root object of the entire engine. Runs the mainloop and owns the various submodules of the engine:
    /// EntityRegistry: manages entities and their components.
    /// EngineSystems: manages the collection of systems used in the game.
    /// sf::RenderWindow: the SFML window to which the game renders.
    /// SceneManager: keeps track of the current scene and loads/unloads new ones.
    /// LuaState: a wrapper around the lua_State through which interoperation with Lua scripts happens.
    /// Scheduler: lets you schedule arbitrary functions to be executed at some point in the future.
    class Engine {

    public:
        Engine();
        virtual ~Engine();
        Engine(const Engine& other) = delete;
        Engine& operator=(const Engine& other) = delete;
        Engine(Engine&& other) = delete;
        Engine& operator=(Engine&& other) = delete;

        void initialize();
        void run();
        void quit();

        inline EntityRegistry& getRegistry() { return m_registry; }
        inline EngineSystems& getSystems() {return m_systems; }
        inline Scheduler& getScheduler() { return m_scheduler; }
        inline sf::RenderWindow& getWindow() { return m_window; }
        inline SceneManager& getSceneManager() { return m_sceneManager; }
        inline LuaState& getLuaState() { return *m_lua; }

        inline double getFps() const { return m_fps; }
        inline std::int64_t getFrameTimeMicroseconds() const { return m_frameTimeMicroseconds; }
        inline float getDeltaTime() const { return m_deltaTime; }

        Actor actor(Entity entity) const;
        Actor makeActor();
        Actor makeActor(const std::string& name);
        Actor findByName(const std::string& name) const;
        Actor getMainCamera() const;

    protected:
        virtual void initializeWindow(sf::RenderWindow& window);

    private:
        void initializeLua();

        void update(float dt);
        void draw();
        void processWindowEvents();

        std::unique_ptr<LuaState> m_lua;
        EntityRegistry m_registry;
        EngineSystems m_systems;
        Scheduler m_scheduler;
        sf::RenderWindow m_window;
        SceneManager m_sceneManager;

        utils::KeyboardHelper m_keyboardHelper;
        utils::MouseHelper m_mouseHelper;

        std::uint32_t m_framerateCap = 240;
        double m_fps = 0.f;
        std::int64_t m_frameTimeMicroseconds = 0;
        float m_deltaTime = 0.f;

        bool m_shouldExit = false;
    };
}

#endif //SAXION_Y2Q1_CPP_ENGINE_H
