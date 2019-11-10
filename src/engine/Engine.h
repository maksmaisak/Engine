//
// Created by Maksym Maisak on 27/9/18.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include "EntityRegistry.h"
#include "EngineSystems.h"
#include "Entity.h"
#include "Scheduler.h"
#include "SceneManager.h"
#include "KeyboardHelper.h"
#include "MouseHelper.h"
#include "Window.h"

namespace en {

    class Actor;
    class LuaState;

    /// The root object of the entire engine. Runs the mainloop and owns the various submodules of the engine:
    /// EntityRegistry: manages entities and their components.
    /// EngineSystems: manages the collection of systems used in the game.
    /// Window: a wrapper around the glfw window to which the game renders.
    /// SceneManager: keeps track of the current scene and loads/unloads new ones.
    /// LuaState: a wrapper around the lua_State through which interoperation with Lua scripts happens.
    /// Scheduler: lets you schedule arbitrary functions to be executed at some point in the future.
    class Engine {

    public:
        static Engine& get();

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
        inline Window& getWindow() { return m_window; }
        inline SceneManager& getSceneManager() { return m_sceneManager; }
        inline LuaState& getLuaState() { return *m_lua; }

        inline double getFps() const { return m_fps; }
        inline std::int64_t getFrameTimeMicroseconds() const { return m_frameTimeMicroseconds; }
        inline float getDeltaTime() const { return m_deltaTime; }
        inline float getDeltaTimeRealtime() const { return m_deltaTimeRealtime; }
        inline float getTimeScale() const { return m_timeScale; }
        inline void setTimeScale(float timeScale) { m_timeScale = timeScale; }

        Actor actor(Entity entity) const;
        Actor makeActor();
        Actor makeActor(const class Name& name);
        Actor findByName(const class Name& name) const;
        Actor getMainCamera() const;

    private:
        void initializeLua();
        void initializeWindow();

        void fixedUpdate();
        void draw();

        std::unique_ptr<LuaState> m_lua;
        EntityRegistry m_registry;
        EngineSystems m_systems;
        Scheduler m_scheduler;
        SceneManager m_sceneManager;

        Window m_window;

        utils::KeyboardHelper m_keyboardHelper;
        utils::MouseHelper m_mouseHelper;

        std::uint32_t m_framerateCap = 240;
        double m_fps = 0.f;
        std::int64_t m_frameTimeMicroseconds = 0;
        float m_deltaTime = 0.f;
        float m_deltaTimeRealtime = 0.f;
        float m_timeScale = 1.f;

        bool m_shouldExit = false;
    };
}

#endif //ENGINE_H
