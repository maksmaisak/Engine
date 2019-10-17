//
// Created by Maksym Maisak on 2019-09-28.
//

#include "MoveAction.h"
#include "Transform.h"
#include "GLMExtensions.h"
#include "Pathfinding.h"
#include "LineRenderer.h"
#include "Grid.h"
#include "Blackboard.h"

using namespace ai;

MoveAction::MoveAction() :
    m_targetPositionBlackboardKey("targetPosition")
{}

MoveAction::MoveAction(const en::Name& targetPositionBlackboardKey) :
    m_targetPositionBlackboardKey(targetPositionBlackboardKey)
{}

ActionOutcome MoveAction::execute() {

    if (!m_blackboard) {
        return ActionOutcome::Fail;
    }

    std::optional<en::GridPosition> targetPositionOptional = m_blackboard->get<en::GridPosition>(m_targetPositionBlackboardKey);
    if (!targetPositionOptional) {
        return ActionOutcome::Fail;
    }

    const en::GridPosition targetPosition = *targetPositionOptional;
    const en::GridPosition currentPosition = m_actor.get<en::Transform>().getWorldPosition();
    if (currentPosition == targetPosition) {
        return ActionOutcome::Success;
    }

    if (!m_pathfindingPath) {

        PathfindingParams params;
        params.allowObstacleGoal = true;
        m_pathfindingPath = Pathfinding::getPath(m_actor.getEngine(), currentPosition, targetPosition, params);
        if (!m_pathfindingPath) {
            return ActionOutcome::Fail;
        }

        if (!m_pathfindingPath->empty()) {
            const en::GridPosition& last = m_pathfindingPath->back();
            assert(last == targetPosition);
        }
    }

    if (m_pathfindingPath->empty()) {
        return ActionOutcome::Success;
    }

    return followPathfindingPath();
}

void MoveAction::reset() {

    Action::reset();

    m_pathfindingPath = std::nullopt;
}

ActionOutcome MoveAction::followPathfindingPath() {

    assert(m_pathfindingPath);
    drawPathfindingPath();

    const glm::vec2 nextPosition = m_pathfindingPath->front();
    if (ai::Pathfinding::isObstacle(m_actor.getEngine(), nextPosition)) {
        return ActionOutcome::Fail;
    }

    constexpr float speed = 2.f;
    auto& transform = m_actor.get<en::Transform>();
    const glm::vec2 currentPosition = transform.getWorldPosition();
    const glm::vec2 newPosition = glm::moveTowards(currentPosition, nextPosition, speed * m_actor.getEngine().getDeltaTime());
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

    auto& lineRenderer = en::LineRenderer::get(m_actor.getEngine());

    const ai::PathfindingPath& path = *m_pathfindingPath;
    std::optional<en::GridPosition> previousPosition = std::nullopt;
    for (const en::GridPosition& position : path) {

        if (previousPosition) {
            lineRenderer.addLineSegment(glm::vec2(*previousPosition) + 0.5f, glm::vec2(position) + 0.5f, {0,1,0,1});
        }

        previousPosition = position;
    }
}
