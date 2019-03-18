//
// Created by Maksym Maisak on 2019-03-18.
//

#include "AABBCollider.h"

using namespace en;

AABBCollider::AABBCollider(const glm::vec3& halfSize) : halfSize(halfSize) {}

void AABBCollider::updateTransform(const glm::mat4& transform) {
    center = transform[3];
}

std::optional<Hit> AABBCollider::collide(Collider& other) {
    return other.collide(*this);
}

std::optional<Hit> AABBCollider::collide(AABBCollider& other) {
    return collisionDetectionDiscrete::AABBVsAABB(other, *this);
}

std::optional<Hit> AABBCollider::collide(SphereCollider& other) {
    return collisionDetectionDiscrete::sphereVsAABB(other, *this);
}