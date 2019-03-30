//
// Created by Maksym Maisak on 2019-03-18.
//

#include "SphereCollider.h"

using namespace en;

SphereCollider::SphereCollider(float radius) : radius(radius) {}

void SphereCollider::updateTransform(const glm::mat4& transform) {
    position = transform[3];
}

std::optional<Hit> SphereCollider::collide(Collider& other, const glm::vec3& movement) {
    return other.collide(*this, movement);
}

std::optional<Hit> SphereCollider::collide(SphereCollider& other, const glm::vec3& movement) {
    return collisionDetection::sphereVsSphere(other, *this, movement);
}

std::optional<Hit> SphereCollider::collide(AABBCollider& other, const glm::vec3& movement) {
    return collisionDetection::AABBVsSphere(other, *this, movement);
}

std::optional<Hit> SphereCollider::collide(OBBCollider& other, const glm::vec3& movement) {
    return collisionDetection::OBBVsSphere(other, *this, movement);
}