//
// Created by Maksym Maisak on 27/12/18.
//

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <cassert>
#include "RenderSettings.h"

namespace en {

    class Engine;

    class Scene {

    public:
        virtual ~Scene() = default;

        virtual void open() = 0;
        virtual void close() {};

        virtual void update(float dt) {}

        Engine& getEngine();
        void setEngine(Engine& engine);

        RenderSettings& getRenderSettings();

    protected:
        RenderSettings m_renderSettings;

    private:
        en::Engine* m_engine = nullptr;
    };
}

#endif //ENGINE_SCENE_H
