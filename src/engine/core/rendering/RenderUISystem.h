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
        explicit RenderUISystem(RenderingSharedState& renderingSharedState);
        void start() override;
        void draw() override;

    private:

        void renderUIRect(Entity entity, const class UIRect& rect);
        void renderSprite(const class Sprite& sprite, const class Transform& transform, const class UIRect& rect, const glm::mat4& matrixProjection);
        void renderText(const class Text& text, const class Transform& transform, const class UIRect& rect, const glm::mat4& matrixProjection);

        float getUIScaleFactor();
        glm::vec2 getWindowSize();

        RenderingSharedState* m_renderingSharedState;
    };
}

#endif //ENGINE_RENDERUISYSTEM_H
