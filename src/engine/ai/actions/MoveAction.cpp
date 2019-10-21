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

namespace {

    std::optional<en::GridPosition> getTargetPosition(const Blackboard& blackboard, const en::Name& key) {

        if (const auto positionOptional = blackboard.get<en::GridPosition>(key)) {
            return positionOptional;
        }

        if (const auto actorOptional = blackboard.get<en::Actor>(key)) {
            if (const en::Actor actor = *actorOptional) {
                if (const auto* const transform = actor.tryGet<en::Transform>()) {
                    return en::GridPosition(glm::floor(transform->getWorldPosition()));
                }
            }
        }

        return std::nullopt;
    }
}

MoveAction::MoveAction() :
    m_targetBlackboardKey("targetPosition")
{}

MoveAction::MoveAction(const en::Name& targetBlackboardKey) :
    m_targetBlackboardKey(targetBlackboardKey)
{}

ActionOutcome MoveAction::execute() {

    const std::optional<en::GridPosition> targetPositionOptional = getTargetPosition(*m_blackboard, m_targetBlackboardKey);
    if (!targetPositionOptional) {
        return ActionOutcome::Fail;
    }

    const en::GridPosition targetPosition = *targetPositionOptional;
    const en::GridPosition currentPosition = glm::floor(m_actor.get<en::Transform>().getWorldPosition());
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

    const en::GridPosition nextGridPosition = m_pathfindingPath->front();
    if (ai::Pathfinding::isObstacle(m_actor.getEngine(), nextGridPosition)) {
        return ActionOutcome::Fail;
    }

    const glm::vec2 nextPosition = glm::vec2(nextGridPosition) + 0.5f;

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
