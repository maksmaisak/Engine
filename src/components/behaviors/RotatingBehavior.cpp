#include "RotatingBehavior.hpp"
#include "Transform.h"

RotatingBehavior::RotatingBehavior(en::Actor actor, const glm::vec3& rotationAxis, float rotationSpeed)
	: Behavior(actor), m_rotationAxis(rotationAxis), m_rotationSpeed(rotationSpeed) {}

void RotatingBehavior::update(float dt) {
    
	m_actor.get<en::Transform>().rotate(dt * m_rotationSpeed, m_rotationAxis);
}
