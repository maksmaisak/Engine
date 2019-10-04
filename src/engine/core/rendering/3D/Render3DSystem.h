//
// Created by Maksym Maisak on 4/10/19.
//

#ifndef ENGINE_RENDER3DSYSTEM_H
#define ENGINE_RENDER3DSYSTEM_H

#include <memory>
#include "System.h"
#include "ShadowMapper.h"

namespace en {

    class Render3DSystem : public System {

    public:
        explicit Render3DSystem(class RenderingSharedState& renderingSharedState);
        void start() override;
        void draw() override;

    private:
        void updateBatches();
        void renderEntities();

        RenderingSharedState* m_renderingSharedState;
        std::unique_ptr<ShadowMapper> m_shadowMapper;
    };
}

#endif //ENGINE_RENDER3DSYSTEM_H
