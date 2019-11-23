//
// Created by Maksym Maisak on 2019-02-14.
//

#include "UIEventSystem.h"
#include "Engine.h"
#include "EntityRegistry.h"
#include "Transform.h"
#include "UIRect.h"
#include "UIEvents.h"
#include "Mouse.h"

using namespace en;

void UIEventSystem::update(float dt) {

    for (Entity e : m_registry->with<UIRect>()) {
        auto& rect = m_registry->get<UIRect>(e);
        rect.wasEnabled = rect.isEnabled;
    }

    const glm::vec2 mousePosition = utils::Mouse::getPosition(m_engine->getWindow());
    for (Entity e : m_registry->with<UIRect, Transform>()) {
        if (!m_registry->get<Transform>(e).getParent()) {
            updateRect(e, m_registry->get<UIRect>(e), m_registry->get<Transform>(e), mousePosition);
        }
    }
}

void UIEventSystem::updateRect(Entity e, UIRect& rect, Transform& transform, const glm::vec2& mousePosition) {

    rect.wasMouseOver = rect.isMouseOver;

    const Bounds2D localBounds = {
        -rect.computedSize * rect.pivot,
        rect.computedSize * (1.f - rect.pivot)
    };
    const glm::vec2 localMousePosition = glm::inverse(transform.getWorldTransform()) * glm::vec4(mousePosition, 0.f, 1.f);
    rect.isMouseOver = localBounds.contains(localMousePosition);

    if (!rect.isEnabled || !rect.wasEnabled) {
        return;
    }

    if (rect.isMouseOver && !rect.wasMouseOver) {
        Receiver<MouseEnter>::broadcast(e);
    }

    if (rect.isMouseOver) {
        Receiver<MouseOver>::broadcast(e);
    }

    if (!rect.isMouseOver && rect.wasMouseOver) {
        Receiver<MouseLeave>::broadcast(e);
    }

    if (rect.isMouseOver) {
        for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i) {

            if (utils::Mouse::isDown(i)) {
                Receiver<MouseDown>::broadcast(e, i);
            }

            if (utils::Mouse::isHeld(i)) {
                Receiver<MouseHold>::broadcast(e, i);
            }

            if (utils::Mouse::isUp(i)) {
                Receiver<MouseUp>::broadcast(e, i);
            }
        }
    }

    if (auto* const tf = m_registry->tryGet<Transform>(e)) {
        for (Entity const child : tf->getChildren()) {
            if (auto* const childRect = m_registry->tryGet<UIRect>(child)) {
                if (auto* const childTf = m_registry->tryGet<Transform>(child)) {
                    updateRect(child, *childRect, *childTf, mousePosition);
                }
            }
        }
    }
}

