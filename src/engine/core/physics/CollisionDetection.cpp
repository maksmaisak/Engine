//
// Created by Maksym Maisak on 2019-03-18.
//

#include "CollisionDetection.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

using namespace en;
using namespace en::collisionDetection;

namespace {

    inline std::optional<Hit> sphereVsSphereContinuous(
        const glm::vec3& moverPosition, float moverRadius, const glm::vec3& movement,
        const glm::vec3& otherPosition, float otherRadius
    ) {

        glm::vec3 relativePosition = moverPosition - otherPosition;

        float a = glm::length2(movement);
        if (a <= glm::epsilon<float>())
            return std::nullopt;

        float b = 2.f * dot(relativePosition, movement);
        float c = glm::length2(relativePosition) - glm::length2(moverRadius + otherRadius);

        // If moving out
        if (b >= 0.f)
            return std::nullopt;

        // If already overlapping.
        if (c < 0.f)
            return Hit {glm::normalize(relativePosition), 0.f};

        float d = b * b - 4.f * a * c;
        if (d < 0.f)
            return std::nullopt;

        float t = (-b - glm::sqrt(d)) / (2.f * a);

        if (t <  0.f)
            return std::nullopt;
        if (t >= 1.f)
            return std::nullopt;

        return Hit {glm::normalize(relativePosition + movement * t), t};
    }

    inline std::optional<Hit> sphereVsAABBInternal(const glm::vec3& spherePosition, float radius, const glm::vec3& boxCenter, const glm::vec3& boxHalfSize, const glm::vec3& movement) {

        const glm::vec3 movedSpherePosition = spherePosition + movement;
        const glm::vec3 closestPoint = glm::clamp(movedSpherePosition, boxCenter - boxHalfSize, boxCenter + boxHalfSize);
        const glm::vec3 delta = movedSpherePosition - closestPoint;
        const float distanceSqr = glm::length2(delta);
        if (distanceSqr > radius * radius)
            return std::nullopt;

        const float distance = glm::sqrt(distanceSqr);
        const glm::vec3 normal = delta / (distance + glm::epsilon<float>());
        return Hit{normal, 1.f, normal * (radius - distance)};
    }

    inline std::optional<Hit> AABBVsSphereInternal(const glm::vec3& boxCenter, const glm::vec3& boxHalfSize, const glm::vec3& spherePosition, float radius, const glm::vec3& movement) {

        const glm::vec3 aabbPosition = boxCenter + movement;
        const glm::vec3 closestPoint = glm::clamp(spherePosition, aabbPosition - boxHalfSize, aabbPosition + boxHalfSize);
        const glm::vec3 delta = closestPoint - spherePosition;
        const float distanceSqr = glm::length2(delta);
        if (distanceSqr > radius * radius)
            return std::nullopt;

        const float distance = glm::sqrt(distanceSqr);
        const glm::vec3 normal = delta / (distance + glm::epsilon<float>());
        return Hit{normal, 1.f, normal * (radius - distance)};
    }
}

std::optional<Hit> en::collisionDetection::sphereVsSphere(SphereCollider& a, SphereCollider& b, const glm::vec3& movement) {

    return sphereVsSphereContinuous(a.position, a.radius, movement, b.position, b.radius);

//    const float sumRadius = a.radius + b.radius;
//    const glm::vec3 delta = a.position - b.position;
//    const float distanceSqr = glm::length2(delta);
//    if (distanceSqr > sumRadius * sumRadius)
//        return std::nullopt;
//
//    return Hit {delta / glm::sqrt(distanceSqr), 1.f};
}

std::optional<Hit> en::collisionDetection::AABBVsAABB(AABBCollider& a, AABBCollider& b, const glm::vec3& movement) {

    const glm::vec3 delta = (a.center + movement) - b.center;
    const glm::vec3 penetration = a.halfSize + b.halfSize - glm::abs(delta);
    if (penetration.x <= 0.f || penetration.y <= 0.f || penetration.z <= 0.f)
        return std::nullopt;

    Hit hit {};
    if (penetration.x < penetration.y) {
        if (penetration.z < penetration.x) {
            hit.normal.z = glm::sign(delta.z);
            hit.depenetrationOffset.z = penetration.z * hit.normal.z;
        } else {
            hit.normal.x = glm::sign(delta.x);
            hit.depenetrationOffset.x = penetration.x * hit.normal.x;
        }
    } else {
        if (penetration.z < penetration.y) {
            hit.normal.z = glm::sign(delta.z);
            hit.depenetrationOffset.z = penetration.z * hit.normal.z;
        } else {
            hit.normal.y = glm::sign(delta.y);
            hit.depenetrationOffset.y = penetration.y * hit.normal.y;
        }
    }
    return hit;
}

std::optional<Hit> en::collisionDetection::sphereVsAABB(SphereCollider& a, AABBCollider& b, const glm::vec3& movement) {
    return sphereVsAABBInternal(a.position, a.radius, b.center, b.halfSize, movement);
}

std::optional<Hit> en::collisionDetection::AABBVsSphere(AABBCollider& a, SphereCollider& b, const glm::vec3& movement) {
    return AABBVsSphereInternal(a.center, a.halfSize, b.position, b.radius, movement);
}

std::optional<Hit> en::collisionDetection::sphereVsOBB(SphereCollider& a, OBBCollider& b, const glm::vec3& movement) {

    const glm::mat3 inverseRotation = glm::transpose(b.rotation);
    const glm::vec3& spherePosition = inverseRotation * (a.position - b.center) + b.center;
    const glm::vec3& localMovement  = inverseRotation * movement;

    std::optional<Hit> hit = sphereVsAABBInternal(spherePosition, a.radius, b.center, b.halfSize, localMovement);
    if (!hit)
        return hit;

    hit->normal              = b.rotation * hit->normal;
    hit->depenetrationOffset = b.rotation * hit->depenetrationOffset;
    return hit;
}

std::optional<Hit> en::collisionDetection::OBBVsSphere(OBBCollider& a, SphereCollider& b, const glm::vec3& movement) {

    const glm::mat3 inverseRotation = glm::transpose(a.rotation);
    const glm::vec3& spherePosition = inverseRotation * (b.position - a.center) + a.center;
    const glm::vec3& localMovement  = inverseRotation * movement;

    std::optional<Hit> hit = AABBVsSphereInternal(a.center, a.halfSize, spherePosition, b.radius, localMovement);
    if (!hit)
        return hit;

    hit->normal              = a.rotation * hit->normal;
    hit->depenetrationOffset = a.rotation * hit->depenetrationOffset;
    return hit;
}

std::optional<Hit> en::collisionDetection::AABBVsOBB(AABBCollider& a, OBBCollider& b, const glm::vec3& movement) {

    return std::optional<Hit>();
}

std::optional<Hit> en::collisionDetection::OBBVsAABB(OBBCollider& a, AABBCollider& b, const glm::vec3& movement) {

    return std::optional<Hit>();
}

std::optional<Hit> en::collisionDetection::OBBVsOBB(OBBCollider& a, OBBCollider& b, const glm::vec3& movement) {

    return std::optional<Hit>();
}
