//
// Created by Maksym Maisak on 2019-09-28.
//

#include "MoveAction.h"
#include "Transform.h"
#include "GLMExtensions.h"
#include "Pathfinding.h"

using namespace ai;

MoveAction::MoveAction(const glm::i64vec2& targetPosition) :
    m_targetPosition(targetPosition)
{}

ActionOutcome MoveAction::execute() {

    auto& transform = actor.get<en::Transform>();
    const glm::vec2 position = transform.getWorldPosition();
    const glm::i64vec2 gridPosition = position;
    if (gridPosition == m_targetPosition) {
        return ActionOutcome::Success;
    }

    if (!m_pathfindingPath) {
        m_pathfindingPath = Pathfinding::getPath(actor.getEngine(), gridPosition, m_targetPosition);
        if (!m_pathfindingPath) {
            return ActionOutcome::Fail;
        }
    }

    if (m_pathfindingPath->empty()) {
        return ActionOutcome::Success;
    }

    constexpr float speed = 2.f;
    const glm::vec2 nextPosition = m_pathfindingPath->front();
    const glm::vec2 newPosition = glm::moveTowards(position, nextPosition, speed * actor.getEngine().getDeltaTime());
    transform.setLocalPosition2D(newPosition);

    if (glm::distance2(newPosition, nextPosition) < glm::epsilon<float>()) {

        m_pathfindingPath->pop();
        if (m_pathfindingPath->empty()) {
            return ActionOutcome::Success;
        }
    }

    return ActionOutcome::InProgress;
}