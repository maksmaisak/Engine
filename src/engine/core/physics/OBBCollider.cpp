//
// Created by Maksym Maisak on 2019-03-26.
//

#include "OBBCollider.h"
#include "CollisionDetection.h"

using namespace en;

void OBBCollider::updateTransform(const glm::mat4& transform) {

    center   = transform[3];
    rotation = glm::mat3(transform);
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
