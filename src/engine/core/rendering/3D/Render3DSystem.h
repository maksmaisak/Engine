//
// Created by Maksym Maisak on 4/10/19.
//

#ifndef ENGINE_RENDER3DSYSTEM_H
#define ENGINE_RENDER3DSYSTEM_H

#include <memory>
#include "System.h"
#include "ShadowMapper.h"

namespace en {

    /// Each frame:
    /// * Updates static batches, if needed.
    /// * Renders 3D entities (ones with both a Transform and a RenderInfo).
    class Render3DSystem : public System {

    public:
        explicit Render3DSystem(std::shared_ptr<struct RenderingSharedState> renderingSharedState);
        void start() override;
        void draw() override;

    private:
        void updateBatches();
        void render();
        void renderBatches(const glm::mat4& matrixView, const glm::mat4& matrixProjection);
        void renderEntities(const glm::mat4& matrixView, const glm::mat4& matrixProjection);

        std::shared_ptr<RenderingSharedState> m_renderingSharedState;
        std::unique_ptr<ShadowMapper> m_shadowMapper;
    };
}

#endif //ENGINE_RENDER3DSYSTEM_H
