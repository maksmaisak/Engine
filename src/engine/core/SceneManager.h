//
// Created by Maksym Maisak on 27/12/18.
//

#ifndef SAXION_Y2Q2_RENDERING_SCENEMANAGER_H
#define SAXION_Y2Q2_RENDERING_SCENEMANAGER_H

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

        template<typename TScene, typename... Args>
        inline void setCurrentScene(Args&&... args) {
            setCurrentScene(std::make_unique<TScene>(std::forward<Args>(args)...));
        }

        template<typename TScene, typename... Args>
        inline void setCurrentSceneNextUpdate(Args&&... args) {
            setCurrentSceneNextUpdate(std::make_unique<TScene>(std::forward<Args>(args)...));
        }

    private:
        void closeCurrentScene();

        Engine* m_engine;
        std::unique_ptr<Scene> m_currentScene;
        std::unique_ptr<Scene> m_openNextUpdateScene;
    };
}

#endif //SAXION_Y2Q2_RENDERING_SCENEMANAGER_H
