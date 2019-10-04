//
// Created by Maksym Maisak on 22/10/18.
//

#ifndef ENGINE_RENDERSYSTEMS_H
#define ENGINE_RENDERSYSTEMS_H

#include "CompoundSystem.h"
#include "DebugHud.h"
#include "Receiver.h"
#include "SceneManager.h"

namespace en {

    /// Contains all rendering systems and creates the shared rendering state.
    class RenderSystems : public CompoundSystem, Receiver<SceneManager::OnSceneClosed, sf::Event> {

    public:
        RenderSystems();
        void start() override;
        void draw() override;

    private:
        void receive(const SceneManager::OnSceneClosed&) override;
        void receive(const sf::Event&) override;

        void renderDebug();

        std::shared_ptr<class RenderingSharedState> m_renderingSharedState;
        std::unique_ptr<DebugHud> m_debugHud;
    };
}

#endif //ENGINE_RENDERSYSTEMS_H
