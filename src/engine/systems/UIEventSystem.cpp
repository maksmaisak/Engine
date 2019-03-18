//
// Created by Maksym Maisak on 2019-02-14.
//

#include "UIEventSystem.h"
#include "Engine.h"
#include "EntityRegistry.h"
#include "Transform.h"
#include "UIRect.h"
#include "UIEvents.h"
#include "MouseHelper.h"

using namespace en;

void UIEventSystem::update(float dt) {

    for (Entity e : m_registry->with<UIRect>()) {
        auto& rect = m_registry->get<UIRect>(e);
        rect.wasEnabled = rect.isEnabled;
    }

    glm::vec2 mousePosition = utils::MouseHelper::getPosition(m_engine->getWindow());
    for (Entity e : m_registry->with<UIRect, Transform>())
        if (!m_registry->get<Transform>(e).getParent())
            updateRect(e, m_registry->get<UIRect>(e), m_registry->get<Transform>(e), mousePosition);
}

void UIEventSystem::updateRect(Entity e, UIRect& rect, Transform& transform, const glm::vec2& mousePosition) {

    rect.wasMouseOver = rect.isMouseOver;

    const glm::vec2 localMin = -rect.computedSize * rect.pivot;
    const glm::vec2 localMax =  rect.computedSize * (1.f - rect.pivot);
    const glm::vec2 localMousePosition = glm::inverse(transform.getWorldTransform()) * glm::vec4(mousePosition, 0.f, 1.f);
    rect.isMouseOver = glm::all(glm::lessThan(localMin, localMousePosition)) && glm::all(glm::lessThan(localMousePosition, localMax));

    if (!rect.isEnabled || !rect.wasEnabled)
        return;

    if (rect.isMouseOver && !rect.wasMouseOver)
        Receiver<MouseEnter>::broadcast(e);

    if (rect.isMouseOver)
        Receiver<MouseOver>::broadcast(e);

    if (!rect.isMouseOver && rect.wasMouseOver)
        Receiver<MouseLeave>::broadcast(e);

    if (rect.isMouseOver) {
        for (int i = 0; i < sf::Mouse::ButtonCount; ++i) {
            auto buttonCode = (sf::Mouse::Button)i;

            if (utils::MouseHelper::isDown(buttonCode))
                Receiver<MouseDown>::broadcast(e, i + 1);

            if (utils::MouseHelper::isHeld(buttonCode))
                Receiver<MouseHold>::broadcast(e, i + 1);

            if (utils::MouseHelper::isUp(buttonCode))
                Receiver<MouseUp>::broadcast(e, i + 1);
        }
    }

    if (auto* tf = m_registry->tryGet<Transform>(e))
        for (Entity child : tf->getChildren())
            if (auto* childRect = m_registry->tryGet<UIRect>(child))
                if (auto* childTf = m_registry->tryGet<Transform>(child))
                    updateRect(child, *childRect, *childTf, mousePosition);
}

