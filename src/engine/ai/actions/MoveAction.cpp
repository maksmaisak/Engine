//
// Created by Maksym Maisak on 2019-09-28.
//

#include "MoveAction.h"
#include "Transform.h"
#include "GLMExtensions.h"
#include "Pathfinding.h"
#include "LineRenderer.h"

using namespace ai;

MoveAction::MoveAction(const glm::i64vec2& targetPosition) :
    m_targetPosition(targetPosition)
{}

ActionOutcome MoveAction::execute() {

    const glm::i64vec2 gridPosition = actor.get<en::Transform>().getWorldPosition();
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

    return followPathfindingPath();
}

ActionOutcome MoveAction::followPathfindingPath() {

    assert(m_pathfindingPath);
    drawPathfindingPath();

    constexpr float speed = 2.f;
    auto& transform = actor.get<en::Transform>();
    const glm::vec2 currentPosition = transform.getWorldPosition();
    const glm::vec2 nextPosition = m_pathfindingPath->front();
    const glm::vec2 newPosition = glm::moveTowards(currentPosition, nextPosition, speed * actor.getEngine().getDeltaTime());
    transform.setLocalPosition2D(newPosition);

    if (glm::distance2(newPosition, nextPosition) < glm::epsilon<float>()) {

        m_pathfindingPath->pop_front();
        if (m_pathfindingPath->empty()) {
            return ActionOutcome::Success;
        }
    }

    return ActionOutcome::InProgress;
}

void MoveAction::drawPathfindingPath() {

    assert(m_pathfindingPath);

    auto& lineRenderer = en::LineRenderer::get(actor.getEngine());

    ai::PathfindingPath& path = *m_pathfindingPath;
    std::optional<glm::i64vec2> previousPosition = std::nullopt;
    for (en::TileLayer::Coordinates position : path) {

        if (previousPosition) {
            lineRenderer.addLineSegment(glm::vec2(*previousPosition) + 0.5f, glm::vec2(position) + 0.5f, {0,1,0,1});
        }

        previousPosition = position;
    }
}
