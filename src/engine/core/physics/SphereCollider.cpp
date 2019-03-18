//
// Created by Maksym Maisak on 2019-03-18.
//

#include "SphereCollider.h"

using namespace en;

SphereCollider::SphereCollider(float radius) : radius(radius) {}

void SphereCollider::updateTransform(const glm::mat4& transform) {
    position = transform[3];
}

std::optional<Hit> SphereCollider::collide(Collider& other) {
    return other.collide(*this);
}

std::optional<Hit> SphereCollider::collide(SphereCollider& other) {
    return collisionDetectionDiscrete::sphereVsSphere(other, *this);
}

std::optional<Hit> SphereCollider::collide(AABBCollider& other) {

    return std::nullopt;
}