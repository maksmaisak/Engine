//
// Created by Maksym Maisak on 27/12/18.
//

#include "SceneManager.h"
#include "Engine.h"
#include "Receiver.h"

using namespace en;

SceneManager::SceneManager(Engine& engine) :
    m_engine(&engine)
{}

void SceneManager::update(float dt) {

    if (m_shouldCloseSceneNextUpdate)
        closeCurrentScene();

    if (m_shouldRestartSceneNextUpdate)
        restartCurrentScene();

    if (m_openNextUpdateScene)
        setCurrentScene(std::move(m_openNextUpdateScene));

    if (m_currentScene)
        m_currentScene->update(dt);
}

void SceneManager::setCurrentScene(std::unique_ptr<en::Scene> scene) {

    closeCurrentScene();

    m_currentScene = std::move(scene);
    if (m_currentScene) {
        m_currentScene->setEngine(*m_engine);
        m_currentScene->open();
        Receiver<OnSceneOpened>::broadcast({this});
    }
}

void SceneManager::closeCurrentScene() {

    if (!m_currentScene)
        return;

    m_shouldCloseSceneNextUpdate = false;
    m_shouldRestartSceneNextUpdate = false;

    close(*m_currentScene);
    m_currentScene = nullptr;
}

void SceneManager::restartCurrentScene() {

    if (!m_currentScene)
        return;

    m_shouldCloseSceneNextUpdate = false;
    m_shouldRestartSceneNextUpdate = false;

    close(*m_currentScene);
    m_currentScene->open();
}

void SceneManager::setCurrentSceneNextUpdate(std::unique_ptr<Scene> scene) {m_openNextUpdateScene = std::move(scene);}
void SceneManager::closeCurrentSceneNextUpdate  () {m_shouldCloseSceneNextUpdate   = true;}
void SceneManager::restartCurrentSceneNextUpdate() {m_shouldRestartSceneNextUpdate = true;}

void SceneManager::close(Scene& scene) {

    scene.close();
    m_engine->getRegistry().destroyAll();
    Receiver<OnSceneClosed>::broadcast({this});
}
