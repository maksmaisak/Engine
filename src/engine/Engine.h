//
// Created by Maksym Maisak on 27/9/18.
//

#ifndef SAXION_Y2Q1_CPP_ENGINE_H
#define SAXION_Y2Q1_CPP_ENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <optional>
#include <typeindex>
#include <type_traits>
#include <iostream>
#include "EntityRegistry.h"
#include "Entity.h"
#include "System.h"
#include "BehaviorSystem.h"
#include "Scheduler.h"
#include "SceneManager.h"
#include "CompoundSystem.h"
#include "KeyboardHelper.h"

namespace en {

    class Behavior;
    class Actor;
    class LuaState;

    class BehaviorsSystem : public CompoundSystem {};

    /// The root object of the entire engine. Manages system execution and owns the various submodules of the engine:
    /// EntityRegistry: manages entities and their components.
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

        inline EntityRegistry& getRegistry()   { return m_registry; }
        inline Scheduler& getScheduler()       { return m_scheduler; }
        inline sf::RenderWindow& getWindow()   { return m_window; }
        inline SceneManager& getSceneManager() { return m_sceneManager; }
        inline LuaState& getLuaState()         { return *m_lua; }
        inline double getFps()                 { return m_fps; }
        inline std::int64_t getFrameTimeMicroseconds() { return m_frameTimeMicroseconds; }

        Actor actor(Entity entity) const;
        Actor makeActor();
        Actor makeActor(const std::string& name);
        Actor findByName(const std::string& name) const;

        template<typename TSystem, typename... Args>
        TSystem& addSystem(Args&&... args);

        template<typename TSystem, typename... Args>
        std::unique_ptr<TSystem> makeSystem(Args&&... args);

        /// Makes sure a system to handle a given behavior type is in place.
        template<typename TBehavior>
        bool ensureBehaviorSystem();

    protected:
        virtual void initializeWindow(sf::RenderWindow& window);

    private:
        std::unique_ptr<LuaState> m_lua;
        EntityRegistry m_registry;
        Scheduler m_scheduler;
        sf::RenderWindow m_window;
        SceneManager m_sceneManager;

        CompoundSystem m_systems;
        BehaviorsSystem* m_behaviors = nullptr;
        utils::CustomTypeMap<struct Dummy, bool> m_behaviorSystemPresence;

        utils::KeyboardHelper m_keyboardHelper;

        std::uint32_t m_framerateCap = 240;
        double m_fps = 0.f;
        std::int64_t m_frameTimeMicroseconds = 0;

        bool m_shouldExit = false;

        void printGLContextVersionInfo();
        void initializeGlew();
        void initializeLua();

        void update(float dt);
        void draw();
        void processWindowEvents();
    };

    template<typename TSystem, typename... Args>
    inline TSystem& Engine::addSystem(Args&&... args) {
        return m_systems.addSystem<TSystem>(std::forward<Args>(args)...);
    }

    template<>
    inline BehaviorsSystem& Engine::addSystem<BehaviorsSystem>() {
        auto& system = m_systems.addSystem<BehaviorsSystem>();
        m_behaviors = &system;
        return system;
    }

    template<typename TSystem, typename... Args>
    std::unique_ptr<TSystem> Engine::makeSystem(Args&&... args) {
        auto system = std::make_unique<TSystem>(std::forward<Args>(args)...);
        system->init(*this);
        return system;
    }

    template<typename TBehavior>
    bool Engine::ensureBehaviorSystem() {

        static_assert(std::is_base_of_v<Behavior, TBehavior>);

        if (m_behaviorSystemPresence.get<TBehavior>())
            return false;

        if (!m_behaviors)
            addSystem<BehaviorsSystem>();
        m_behaviors->addSystem<BehaviorSystem<TBehavior>>();
        m_behaviorSystemPresence.set<TBehavior>(true);
        return true;
    }
}

#endif //SAXION_Y2Q1_CPP_ENGINE_H
