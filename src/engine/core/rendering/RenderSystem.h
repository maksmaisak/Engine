//
// Created by Maksym Maisak on 22/10/18.
//

#ifndef ENGINE_RENDERSYSTEM_H
#define ENGINE_RENDERSYSTEM_H

#include "CompoundSystem.h"
#include "DebugHud.h"
#include "Receiver.h"
#include "RenderingSharedState.h"
#include "SceneManager.h"

namespace en {

    /// Renders (in this order):
    /// * 3D Entities (Entities with both a Transform and a RenderInfo)
    /// * Skybox (see RenderSkyboxSystem)
    /// * 2D Entities (see Render2DSystem)
    /// * UI (see RenderUISystem)
    class RenderSystem : public CompoundSystem, Receiver<SceneManager::OnSceneClosed, sf::Event> {

    public:
        void start() override;
        void draw() override;

    private:
        void receive(const SceneManager::OnSceneClosed&) override;
        void receive(const sf::Event&) override;

        static void setOpenGLSettings();
        void getConfigFromLua();
        void renderDebug();

        RenderingSharedState m_renderingSharedState;
        std::unique_ptr<DebugHud> m_debugHud;
    };
}

#endif //ENGINE_RENDERSYSTEM_H
