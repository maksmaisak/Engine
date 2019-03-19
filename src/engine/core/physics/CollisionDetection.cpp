//
// Created by Maksym Maisak on 2019-03-18.
//

#include "CollisionDetection.h"
#include "SphereCollider.h"
#include "AABBCollider.h"

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

    const glm::vec3 spherePosition = a.position + movement;
    const glm::vec3 closestPoint = glm::clamp(spherePosition, b.center - b.halfSize, b.center + b.halfSize);
    const glm::vec3 delta = spherePosition - closestPoint;
    const float distanceSqr = glm::length2(delta);
    if (distanceSqr > a.radius * a.radius)
        return std::nullopt;

    const float distance = glm::sqrt(distanceSqr);
    const glm::vec3 normal = delta / distance;
    return Hit {normal, 1.f, normal * (a.radius - distance)};
}

std::optional<Hit> en::collisionDetection::AABBVsSphere(AABBCollider& a, SphereCollider& b, const glm::vec3& movement) {

    const glm::vec3 aabbPosition = a.center + movement;
    const glm::vec3 closestPoint = glm::clamp(b.position, aabbPosition - a.halfSize, aabbPosition + a.halfSize);
    const glm::vec3 delta = closestPoint - b.position;
    const float distanceSqr = glm::length2(delta);
    if (distanceSqr > b.radius * b.radius)
        return std::nullopt;

    const float distance = glm::sqrt(distanceSqr);
    const glm::vec3 normal = delta / distance;
    return Hit {normal, 1.f, normal * (b.radius - distance)};
}
