//
// Created by Maksym Maisak on 2019-09-28.
//

#include "MoveAction.h"
#include "Transform.h"
#include "GLMExtensions.h"

using namespace ai;

MoveAction::MoveAction(const glm::i64vec2& targetPosition) :
    m_targetPosition(targetPosition)
{}

ActionOutcome MoveAction::execute() {

    auto& transform = actor.get<en::Transform>();
    const glm::vec2 position = transform.getWorldPosition();
    const glm::vec<2, int64_t> gridPosition = position;

    if (gridPosition == m_targetPosition) {
        return ActionOutcome::Success;
    }

    const glm::vec2 delta = glm::vec2(m_targetPosition) - position;

    constexpr float speed = 2.f;
    const glm::vec2 newPosition = glm::moveTowards(position, glm::vec2(m_targetPosition), speed * actor.getEngine().getDeltaTime());
    transform.setLocalPosition2D(newPosition);

    return ActionOutcome::InProgress;
}