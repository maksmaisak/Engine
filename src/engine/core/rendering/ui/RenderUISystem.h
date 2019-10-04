//
// Created by Maksym Maisak on 2019-09-22.
//

#ifndef ENGINE_RENDERUISYSTEM_H
#define ENGINE_RENDERUISYSTEM_H

#include <memory>
#include "System.h"
#include "Entity.h"
#include "glm.h"

namespace en {

    class RenderUISystem : public System {

    public:
        explicit RenderUISystem(std::shared_ptr<struct RenderingSharedState> renderingSharedState);
        void start() override;
        void draw() override;

    private:
        void renderUIRect(Entity entity, const class UIRect& rect);
        void renderSprite(const class UISprite& sprite, const class Transform& transform, const class UIRect& rect, const glm::mat4& matrixProjection);
        void renderText(const class Text& text, const class Transform& transform, const class UIRect& rect, const glm::mat4& matrixProjection);

        float getUIScaleFactor();
        glm::vec2 getWindowSize();

        std::shared_ptr<RenderingSharedState> m_renderingSharedState;
    };
}

#endif //ENGINE_RENDERUISYSTEM_H
