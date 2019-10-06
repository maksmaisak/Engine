//
// Created by Maksym Maisak on 2/10/19.
//

#ifndef ENGINE_SHADOWMAPPER_H
#define ENGINE_SHADOWMAPPER_H

#include <memory>
#include <vector>
#include "Bounds.h"
#include "Entity.h"

namespace en {

    /// Updates the shadowmapping depth maps in the given rendering shared state
    class ShadowMapper {

    public:
        explicit ShadowMapper(class Engine& engine, std::shared_ptr<struct RenderingSharedState> renderingSharedState);
        void updateDepthMaps();

    private:
        void updateDepthMapsDirectionalLights(const std::vector<Entity>& lightEntities);
        void updateDepthMapsPositionalLights(const std::vector<Entity>& lightEntities);

        Engine* m_engine;
        std::shared_ptr<RenderingSharedState> m_renderingSharedState;

        std::shared_ptr<class ShaderProgram> m_directionalDepthShader;
        std::shared_ptr<class ShaderProgram> m_positionalDepthShader;
        utils::Bounds m_shadowReceiversBounds;
    };
}

#endif //ENGINE_SHADOWMAPPER_H
