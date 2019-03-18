//
// Created by Maksym Maisak on 2019-03-18.
//

#include "AABBCollider.h"

using namespace en;

AABBCollider::AABBCollider(const glm::vec3& halfSize) : halfSize(halfSize) {}

void AABBCollider::updateTransform(const glm::mat4& transform) {
    center = transform[3];
}

std::optional<Hit> AABBCollider::collide(Collider& other, const glm::vec3& movement) {
    return other.collide(*this, movement);
}

std::optional<Hit> AABBCollider::collide(AABBCollider& other, const glm::vec3& movement) {
    return collisionDetection::AABBVsAABB(other, *this, movement);
}

std::optional<Hit> AABBCollider::collide(SphereCollider& other, const glm::vec3& movement) {
    return collisionDetection::sphereVsAABB(other, *this, movement);
}