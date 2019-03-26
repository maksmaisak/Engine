//
// Created by Maksym Maisak on 27/12/18.
//

#ifndef ENGINE_SCENEMANAGER_H
#define ENGINE_SCENEMANAGER_H

#include <vector>
#include <cassert>
#include <memory>
#include "Scene.h"

namespace en {

    class Engine;

    class SceneManager {

    public:

        struct OnSceneClosed {SceneManager* sceneManager;};
        struct OnSceneOpened {SceneManager* sceneManager;};

        explicit SceneManager(Engine* engine) : m_engine(engine) {assert(m_engine);}
        void update(float dt);

        inline Scene* getCurrentScene() { return m_currentScene.get(); }
        void setCurrentScene(std::unique_ptr<Scene> scene);
        void setCurrentSceneNextUpdate(std::unique_ptr<Scene> scene);
        void closeCurrentScene();
        void setCurrentSceneNextUpdate();

        template<typename TScene, typename... Args>
        inline void setCurrentScene(Args&&... args) {
            setCurrentScene(std::make_unique<TScene>(std::forward<Args>(args)...));
        }

        template<typename TScene, typename... Args>
        inline void setCurrentSceneNextUpdate(Args&&... args) {
            setCurrentSceneNextUpdate(std::make_unique<TScene>(std::forward<Args>(args)...));
        }

    private:

        Engine* m_engine = nullptr;
        std::unique_ptr<Scene> m_currentScene;
        std::unique_ptr<Scene> m_openNextUpdateScene;
        bool m_shouldCloseSceneNextFrame = false;
    };
}

#endif //ENGINE_SCENEMANAGER_H
