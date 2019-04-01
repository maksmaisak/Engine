//
// Created by Maksym Maisak on 2019-03-26.
//

#include "OBBCollider.h"
#include "CollisionDetection.h"
#include <glm/gtx/matrix_decompose.hpp>

using namespace en;

OBBCollider::OBBCollider(const glm::vec3& halfSize) : halfSize(halfSize) {}

void OBBCollider::updateTransform(const glm::mat4& transform) {

    glm::vec3 scale;
    glm::quat orientation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform, scale, orientation, translation, skew, perspective);

    center   = transform[3];
    rotation = glm::mat3(orientation);
}

utils::Bounds OBBCollider::getBounds() {

    const glm::mat3 absRotation = {glm::abs(rotation[0]), glm::abs(rotation[1]), glm::abs(rotation[2])};
    const glm::vec3 rotatedHalfSize = absRotation * halfSize;
    return {center - rotatedHalfSize, center + rotatedHalfSize};
}

utils::BoundingSphere OBBCollider::getBoundingSphere() {
    return {center, glm::length(halfSize)};
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
