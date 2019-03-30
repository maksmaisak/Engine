//
// Created by Maksym Maisak on 2019-03-26.
//

#include "OBBCollider.h"
#include "CollisionDetection.h"

using namespace en;

OBBCollider::OBBCollider(const glm::vec3& halfSize) : halfSize(halfSize) {}

void OBBCollider::updateTransform(const glm::mat4& transform) {

    center   = transform[3];
    rotation = glm::mat3(glm::extractMatrixRotation(transform));
}

std::optional<Hit> OBBCollider::collide(Collider& other, const glm::vec3& movement) {return other.collide(*this, movement);}

std::optional<Hit> OBBCollider::collide(SphereCollider& other, const glm::vec3& movement) {
    return collisionDetection::sphereVsOBB(other, *this, movement);
}

std::optional<Hit> OBBCollider::collide(AABBCollider& other, const glm::vec3& movement) {
    return collisionDetection::AABBVsOBB(other, *this, movement);
}

std::optional<Hit> OBBCollider::collide(OBBCollider& other, const glm::vec3& movement) {
    return collisionDetection::OBBVsOBB(other, *this, movement);
}