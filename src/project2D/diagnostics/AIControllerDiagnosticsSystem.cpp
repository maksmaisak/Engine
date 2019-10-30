//
// Created by Maksym Maisak on 30/10/19.
//

#include "AIControllerDiagnosticsSystem.h"
#include <imgui.h>
#include "LineRenderer.h"
#include "AIController.h"
#include "Sprite.h"
#include "Transform.h"
#include "MouseHelper.h"
#include "Camera.h"

namespace {

    glm::vec2 getMousePositionWorldspace(const en::Actor& cameraActor) {

        assert(cameraActor);

        const glm::mat4 cameraToWorldspace = cameraActor.get<en::Transform>().getWorldTransform();
        const glm::mat4 cameraToClipspace = cameraActor.get<en::Camera>().getCameraProjectionMatrix(cameraActor.getEngine());
        const glm::mat4 clipspaceToWorldspace = cameraToWorldspace * glm::inverse(cameraToClipspace);

        const sf::RenderWindow& window = cameraActor.getEngine().getWindow();
        const sf::Vector2i mousePositionPixels = sf::Mouse::getPosition(window);
        const sf::Vector2u windowSize = window.getSize();
        const glm::vec2 mousePositionNormalized {
            mousePositionPixels.x / static_cast<float>(windowSize.x),
            1.f - mousePositionPixels.y / static_cast<float>(windowSize.y)
        };
        const glm::vec2 mousePositionClipspace = mousePositionNormalized * 2.f - 1.f;

        return clipspaceToWorldspace * glm::vec4(mousePositionClipspace, 0.f, 1.f);
    }

    bool intersectsSprite(const glm::vec2& position, const en::Actor& actor) {

        const glm::mat4 localToWorld = actor.get<en::Sprite>().getRenderMatrix(actor.get<en::Transform>().getWorldTransform());
        const glm::vec2 mousePositionInSprite = glm::inverse(localToWorld) * glm::vec4(position, 0.f, 1.f);
        return en::Bounds2D(glm::vec2(0.f), glm::vec2(1.f)).contains(mousePositionInSprite);
    }
}

void AIControllerDiagnosticsSystem::draw() {

    if (en::Actor camera = m_engine->getMainCamera()) {

        const glm::vec2 mousePosition = getMousePositionWorldspace(camera);
        en::LineRenderer::get(*m_engine).addAABB({mousePosition - glm::vec2(0.1f), mousePosition + glm::vec2(0.1f)});

        for (en::Entity e : m_registry->with<en::Transform, en::Sprite, ai::AIController>()) {

            const en::Actor actor = m_engine->actor(e);
            const bool isMouseOver = intersectsSprite(mousePosition, actor);
            if (isMouseOver) {
                showAIController(actor);
                break;
            }
        }
    }
}

void AIControllerDiagnosticsSystem::showAIController(const en::Actor& actor) {

    if (const ai::AIController* const aiController = actor.tryGet<ai::AIController>()) {
        if (const ai::BehaviorTree* const behaviorTree = aiController->getBehaviorTree()) {
            behaviorTree->display();
        }
    }
}
