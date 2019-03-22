//
// Created by Maksym Maisak on 22/10/18.
//

#ifndef SAXION_Y2Q1_CPP_RENDERSYSTEM_H
#define SAXION_Y2Q1_CPP_RENDERSYSTEM_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include "Entity.h"
#include "System.h"
#include "Engine.h"
#include "DepthMaps.h"
#include "VertexRenderer.h"
#include "ComponentPool.h"
#include "Receiver.h"
#include "SceneManager.h"
#include "SkyboxRenderer.h"
#include "DebugHud.hpp"
#include "Bounds.h"

namespace en {

    class ShaderProgram;
    class Material;
    class Mesh;
    class DebugHud;
	struct UIRect;

    class RenderSystem : public System, Receiver<SceneManager::OnSceneClosed> {

    public:
        RenderSystem();
        void start() override;
        void draw() override;

    private:
        void receive(const SceneManager::OnSceneClosed& info) override;

        void updateBatches();
        void updateDepthMaps();
        void renderEntities();
        void renderSkybox();
        void renderUI();
        void renderDebug();

        Actor getMainCamera();
        void updateDepthMapsDirectionalLights(const std::vector<Entity>& directionalLights);
        void updateDepthMapsPositionalLights (const std::vector<Entity>& pointLights);
        void updateShadowCastersBounds();
        utils::Bounds getCameraFrustrumBounds();

        void renderUIRect(Entity entity, UIRect& rect);
        glm::vec2 getWindowSize();
        float getUIScaleFactor();

        DepthMaps m_depthMaps;
        std::shared_ptr<ShaderProgram> m_directionalDepthShader;
        std::shared_ptr<ShaderProgram> m_positionalDepthShader;
        utils::Bounds m_shadowReceiversBounds;

        bool m_enableStaticBatching = true;
        std::unordered_map<std::shared_ptr<Material>, Mesh> m_batches;
        
        SkyboxRenderer m_skyboxRenderer;
        std::shared_ptr<Texture> m_defaultSkybox;

        VertexRenderer m_vertexRenderer;
        glm::vec2 m_referenceResolution;

        bool m_enableDebugOutput = false;
        std::unique_ptr<DebugHud> m_debugHud;
    };
}

#endif //SAXION_Y2Q1_CPP_RENDERSYSTEM_H
