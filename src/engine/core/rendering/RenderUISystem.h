//
// Created by Maksym Maisak on 2019-09-22.
//

#ifndef ENGINE_RENDERUISYSTEM_H
#define ENGINE_RENDERUISYSTEM_H

#include "System.h"

#include "System.h"
#include "Mesh.h"
#include "DebugVolumeRenderer.h"
#include "RenderingSharedState.h"

namespace en {

    class RenderUISystem : public System {

    public:
        RenderUISystem(RenderingSharedState& renderingSharedState);
        void start() override;
        void draw() override;

    private:

        void renderUIRect(Entity entity, class UIRect& rect);
        float getUIScaleFactor();
        glm::vec2 getWindowSize();

        RenderingSharedState* m_renderingSharedState;
    };
}

#endif //ENGINE_RENDERUISYSTEM_H
