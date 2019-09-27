//
// Created by Maksym Maisak on 22/10/18.
//

#ifndef SAXION_Y2Q1_CPP_RENDERSYSTEM_H
#define SAXION_Y2Q1_CPP_RENDERSYSTEM_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <SFML/Window.hpp>
#include "glm.h"
#include "Entity.h"
#include "System.h"
#include "Engine.h"
#include "DepthMaps.h"
#include "VertexRenderer.h"
#include "ComponentPool.h"
#include "Receiver.h"
#include "SceneManager.h"
#include "DebugHud.hpp"
#include "Bounds.h"
#include "RenderingSharedState.h"

namespace en {

	/// Renders (in this order):
	/// * Entities with both a Transform and a RenderInfo.
	/// * Skybox
	/// * Entities with both a Transform and a UIRect.
    class RenderSystem : public CompoundSystem, Receiver<SceneManager::OnSceneClosed, sf::Event> {

    public:
        RenderSystem();
        void start() override;
        void draw() override;

    private:
        void receive(const SceneManager::OnSceneClosed&) override;
        void receive(const sf::Event&) override;

        void updateBatches();
        void updateDepthMaps();
        void renderEntities();
        void renderDebug();

        void setOpenGLSettings();
        void getConfigFromLua();

        Actor getMainCamera();
        void updateDepthMapsDirectionalLights(const std::vector<Entity>& directionalLights);
        void updateDepthMapsPositionalLights (const std::vector<Entity>& pointLights);
        void updateShadowCastersBounds();
        utils::Bounds getCameraFrustumBounds();

        RenderingSharedState m_renderingSharedState;

        // Shadowmapping
        std::shared_ptr<ShaderProgram> m_directionalDepthShader;
        std::shared_ptr<ShaderProgram> m_positionalDepthShader;
        utils::Bounds m_shadowReceiversBounds;

        // Static batching
        bool m_enableStaticBatching;
        std::unordered_map<std::shared_ptr<Material>, Mesh> m_batches;

        // Debug
        bool m_enableDebugOutput;
        std::unique_ptr<DebugHud> m_debugHud;
    };
}

#endif //SAXION_Y2Q1_CPP_RENDERSYSTEM_H
