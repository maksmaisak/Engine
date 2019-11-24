//
// Created by Maksym Maisak on 2019-09-22.
//

#include "RenderUISystem.h"

#include "Engine.h"
#include "Transform.h"
#include "UIRect.h"
#include "UISprite.h"
#include "Text.h"
#include "RenderingSharedState.h"

using namespace en;

namespace {

    void updateUIRect(Engine& engine, EntityRegistry& registry, Entity e, const glm::vec2& parentSize, const glm::vec2& parentPivot, float scaleFactor) {

        auto* const rect = registry.tryGet<UIRect>(e);
        if (!rect) {
            return;
        }

        auto* const tf = registry.tryGet<Transform>(e);
        if (!tf) {
            return;
        }

        const glm::vec2 parentMinToLocalMin = rect->anchorMin * parentSize + rect->offsetMin * scaleFactor;
        const glm::vec2 parentMinToLocalMax = rect->anchorMax * parentSize + rect->offsetMax * scaleFactor;
        rect->computedSize = parentMinToLocalMax - parentMinToLocalMin;

        const glm::vec2 parentMinToLocalPivot = glm::lerp(parentMinToLocalMin, parentMinToLocalMax, rect->pivot);
        const glm::vec2 parentPivotToParentMin = -parentSize * parentPivot;
        const glm::vec2 parentPivotToLocalPivot = parentPivotToParentMin + parentMinToLocalPivot;
        tf->setLocalPosition(glm::vec3(parentPivotToLocalPivot, tf->getLocalPosition().z));

        for (Entity child : tf->getChildren()) {
            updateUIRect(engine, registry, child, rect->computedSize, rect->pivot, scaleFactor);
        }
    }
}

RenderUISystem::RenderUISystem(std::shared_ptr<RenderingSharedState> renderingSharedState) :
    m_renderingSharedState(std::move(renderingSharedState))
{
    assert(m_renderingSharedState);
}

void RenderUISystem::start() {

    LuaState& lua = m_engine->getLuaState();

    lua_getglobal(lua, "Game");
    const auto popGame = lua::PopperOnDestruct(lua);

    lua.setField("getUIScaleFactor", [this](){return getUIScaleFactor();});
}

void RenderUISystem::draw() {

    glDisable(GL_DEPTH_TEST);

    for (Entity e : m_registry->with<Transform, UIRect>()) {
        if (!m_registry->get<Transform>(e).getParent()) {
            updateUIRect(*m_engine, *m_registry, e, getWindowSize(), {0, 0}, getUIScaleFactor());
        }
    }

    for (Entity e : m_registry->with<Transform, UIRect>()) {
        if (!m_registry->get<Transform>(e).getParent()) {
            renderUIRect(e, m_registry->get<UIRect>(e));
        }
    }

    glEnable(GL_DEPTH_TEST);
}

void RenderUISystem::renderUIRect(Entity e, const UIRect& rect) {

    if (!rect.isEnabled) {
        return;
    }

    const auto* transform = m_registry->tryGet<Transform>(e);
    if (!transform) {
        return;
    }

    const glm::vec2 windowSize = getWindowSize();
    const glm::mat4 matrixProjection = glm::ortho(0.f, windowSize.x, 0.f, windowSize.y);

    auto* sprite = m_registry->tryGet<UISprite>(e);
    if (sprite && sprite->isEnabled && sprite->material) {
        renderSprite(*sprite, *transform, rect, matrixProjection);
    }

    auto* text = m_registry->tryGet<Text>(e);
    if (text && text->getMaterial()) {
        renderText(*text, *transform, rect, matrixProjection);
    }

    for (Entity child : transform->getChildren()) {
        if (auto* childRect = m_registry->tryGet<UIRect>(child)) {
            renderUIRect(child, *childRect);
        }
    }
}

float RenderUISystem::getUIScaleFactor() {

    const glm::vec2 scale = getWindowSize() / m_renderingSharedState->referenceResolution;
    return std::sqrt(scale.x * scale.y);
}

glm::vec2 RenderUISystem::getWindowSize() {

    return m_engine->getWindow().getSize();
}

void RenderUISystem::renderSprite(const UISprite& sprite, const Transform& transform, const UIRect& rect, const glm::mat4& matrixProjection) {

    assert(sprite.isEnabled && sprite.material);

    const glm::mat4& matrixModel = transform.getWorldTransform();
    const glm::vec2 localMin = -rect.computedSize * rect.pivot;
    const glm::vec2 localMax =  rect.computedSize * (1.f - rect.pivot);
    const glm::vec3 corners[] {
        matrixModel * glm::vec4(localMin              , 0, 1),
        matrixModel * glm::vec4(localMin.x, localMax.y, 0, 1),
        matrixModel * glm::vec4(localMax.x, localMin.y, 0, 1),
        matrixModel * glm::vec4(localMax              , 0, 1)
    };
    const std::vector<Vertex> vertices = {
        {corners[1], {0, 1}},
        {corners[0], {0, 0}},
        {corners[2], {1, 0}},

        {corners[1], {0, 1}},
        {corners[2], {1, 0}},
        {corners[3], {1, 1}},
    };

    sprite.material->use(m_engine, &m_renderingSharedState->depthMaps, glm::mat4(1), glm::mat4(1), matrixProjection);
    m_renderingSharedState->vertexRenderer.renderVertices(vertices);
}

void RenderUISystem::renderText(const Text& text, const Transform& transform, const UIRect& rect, const glm::mat4& matrixProjection) {

    assert(text.getMaterial());

    const std::vector<Vertex>& vertices = text.getVertices();

    const glm::vec2& alignment = text.getAlignment();
    const glm::vec2 boundsAlignPoint = glm::lerp(text.getBoundsMin(), text.getBoundsMax(), alignment);
    const glm::vec2 offsetInRect = rect.computedSize * (alignment - rect.pivot);

    // Scale the bounds by the scale factor and bring them to the rect's position.
    glm::mat4 matrix = glm::translate(glm::vec3(-boundsAlignPoint, 0.f));
    matrix = glm::scale(glm::vec3(getUIScaleFactor())) * matrix;
    matrix = glm::translate(glm::vec3(offsetInRect, 0.f)) * matrix;
    matrix = transform.getWorldTransform() * matrix;

    text.getMaterial()->use(m_engine, &m_renderingSharedState->depthMaps, glm::mat4(1), glm::mat4(1), matrixProjection * matrix);
    m_renderingSharedState->vertexRenderer.renderVertices(vertices);
}

