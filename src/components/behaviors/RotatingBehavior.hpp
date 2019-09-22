#ifndef ROTATINGBEHAVIOUR_HPP
#define ROTATINGBEHAVIOUR_HPP

#include "Behavior.h"
#include "glm.h"
#include "ComponentsToLua.h"

/// Simply rotates the object around its origin with a fixed speed.
class RotatingBehavior : public en::Behavior {

	LUA_TYPE(RotatingBehavior)
public:
	explicit RotatingBehavior(en::Actor actor, const glm::vec3& rotationAxis = {1,1,0}, float rotationSpeed = glm::radians(45.f));
	virtual ~RotatingBehavior() = default;

	void update(float dt) override;

private:
	glm::vec3 m_rotationAxis = {1,1,0};
	float m_rotationSpeed = glm::radians(45.f);
};

#endif // ROTATINGBEHAVIOUR_HPP
