//
// Created by Maksym Maisak on 2019-01-21.
//

#include "Scene.h"

using namespace en;

Engine& Scene::getEngine() {

    assert(m_engine);
    return *m_engine;
}

void Scene::setEngine(Engine& engine) {

    assert(!m_engine && "Engine already set!");
    m_engine = &engine;
}

RenderSettings& Scene::getRenderSettings() {

    return m_renderSettings;
}
