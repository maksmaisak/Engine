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

        return Hit{glm::normalize(relativePosition + movement * t), t};
    }

    inline std::optional<Hit> sphereVsAABBInternal(const glm::vec3& spherePosition, float radius, const glm::vec3& boxCenter, const glm::vec3& boxHalfSize, const glm::vec3& movement) {

        const glm::vec3 movedSpherePosition = spherePosition + movement;

        const glm::vec3 closestPoint = glm::clamp(movedSpherePosition, boxCenter - boxHalfSize, boxCenter + boxHalfSize);
        const glm::vec3 delta = movedSpherePosition - closestPoint;
        const float distanceSqr = glm::length2(delta);
        if (distanceSqr > radius * radius)
            return std::nullopt;

        if (distanceSqr > glm::epsilon<float>()) {

            const float distance = glm::sqrt(distanceSqr);
            const glm::vec3 normal = delta / (distance + glm::epsilon<float>());
            return Hit{normal, 1.f, normal * (radius - distance)};

        } else {

            // If spherePosition is inside the box
            const glm::vec3 fromCenter = glm::normalize(spherePosition - boxCenter);
            const float distanceFromCenter = glm::length(fromCenter);
            const glm::vec3 normal = fromCenter / (distanceFromCenter + glm::epsilon<float>());
            return Hit{normal, 1.f, normal * (radius + (glm::dot(boxHalfSize, glm::abs(normal)) - distanceFromCenter))};
        }
    }

    inline std::optional<Hit> AABBVsSphereInternal(const glm::vec3& boxCenter, const glm::vec3& boxHalfSize, const glm::vec3& spherePosition, float radius, const glm::vec3& movement) {

        const glm::vec3 aabbPosition = boxCenter + movement;
        const glm::vec3 closestPoint = glm::clamp(spherePosition, aabbPosition - boxHalfSize, aabbPosition + boxHalfSize);
        const glm::vec3 delta = closestPoint - spherePosition;
        const float distanceSqr = glm::length2(delta);
        if (distanceSqr > radius * radius)
            return std::nullopt;

        if (distanceSqr > glm::epsilon<float>()) {

            const float distance = glm::sqrt(distanceSqr);
            const glm::vec3 normal = delta / (distance + glm::epsilon<float>());
            return Hit{normal, 1.f, normal * (radius - distance)};

        } else {

            // If spherePosition is inside the box
            const glm::vec3 fromCenter = glm::normalize(aabbPosition - spherePosition);
            const float distanceFromCenter = glm::length(fromCenter);
            const glm::vec3 normal = fromCenter / (distanceFromCenter + glm::epsilon<float>());
            return Hit{normal, 1.f, normal * (radius + (glm::dot(boxHalfSize, glm::abs(normal)) - distanceFromCenter))};
        }
    }

    inline glm::vec3 getAxis(glm::length_t i) {

        glm::vec3 axis {};
        axis[i] = 1.f;
        return axis;
    }

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
    inline glm::mat<C, R, T, Q> absMatrix(const glm::mat<C,R,T,Q>& m) {

        glm::mat<C,R,T,Q> result{};
        for (glm::length_t c = 0; c < C; ++c)
            result[c] = glm::abs(m[c]);
        return result;
    }

    // For an OBBvsOBB (or AABBvsOBB) test, does a SAT test on the axis that is the cross product of `axisIndexA`th axis of box A with the `axisIndexB`th axis of box B.
    // All calculations are performed in the local space of the box B, as if it was an aabb centered at origin.
    template<int axisIndexA, int axisIndexB>
    inline bool satOBBAxisTest(const glm::vec3& aHalfSize, const glm::vec3& bHalfSize, const glm::vec3& delta, const glm::mat3& a2B, const glm::mat3& absA2B, float& minPenetrationDepth, glm::vec3& minPenetrationAxis) {

        static_assert(axisIndexA >= 0 && axisIndexA < 3);
        static_assert(axisIndexB >= 0 && axisIndexB < 3);

    #ifndef ENGINE_FAST_OBB_TEST

        const glm::vec3 axisA = a2B    [axisIndexA];
        const glm::vec3 axisB = getAxis(axisIndexB);
        // If the axes are pointing in the same direction, their cross will be zero, so we can't use that as a normal.
        if (glm::epsilonEqual(glm::abs(glm::dot(axisA, axisB)), 1.f, glm::epsilon<float>()))
            return true;

        const glm::vec3 axis = glm::normalize(glm::cross(axisA, axisB));
        const float rb = glm::epsilon<float>() + glm::dot(bHalfSize, glm::abs(axis));
        const float ra = glm::epsilon<float>() + glm::dot(aHalfSize, glm::abs(glm::transpose(a2B) * axis));
        const float distanceProjected = glm::abs(glm::dot(delta, axis));

        const float penetrationDepth = ra + rb - distanceProjected;
        if (penetrationDepth < minPenetrationDepth) {

            if (penetrationDepth < 0.f)
                return false;

            minPenetrationAxis  = axis;
            minPenetrationDepth = penetrationDepth;
        }

    #else

        // Optimized, but buggy. with axes 0 and 2 in the test scene penetration seems way too low.
        // (During the first box-wall collision)

        // Indices evaluated at compile time.
        constexpr int b1 = axisIndexB == 0 ? 1 : 0;
        constexpr int b2 = axisIndexB == 2 ? 1 : 2;
        constexpr int a1 = axisIndexA == 0 ? 1 : 0;
        constexpr int a2 = axisIndexA == 2 ? 1 : 2;
        const float rb = bHalfSize[b1] * absA2B[b2][axisIndexA] + bHalfSize[b2] * absA2B[b1][axisIndexA];
        const float ra = aHalfSize[a1] * absA2B[axisIndexB][a2] + aHalfSize[a2] * absA2B[axisIndexB][a1];

        constexpr int x1 = (axisIndexB + 2) % 3;
        constexpr int x2 = (axisIndexB + 1) % 3;
        const float distanceProjected = glm::abs(delta[x1] * a2B[x2][axisIndexA] - delta[x2] * a2B[x1][axisIndexA]);

        const float penetrationDepth = ra + rb - distanceProjected;
        if (penetrationDepth < minPenetrationDepth) {

            if (penetrationDepth < 0.f)
                return false;

            const glm::vec3 axisA = a2B[axisIndexA];
            const glm::vec3 axisB = getAxis(axisIndexB);
            // If the axes are pointing in the same direction, their cross will be zero, so we can't use that as a normal.
            if (glm::epsilonEqual(glm::abs(glm::dot(axisA, axisB)), 1.f, glm::epsilon<float>()))
                return true;

            // Will be normalized in the end
            minPenetrationAxis = glm::cross(axisA, axisB);
            minPenetrationDepth = penetrationDepth;
        }

    #endif

        return true;
    }

    inline std::optional<Hit> obbVsAABBInternal(const glm::vec3& aHalfSize, const glm::vec3& bHalfSize, const glm::mat3 a2B, const glm::vec3& initialDelta, const glm::vec3& movement) {

        // Add epsilon to prevent a zero vector being recognized as a separating axis
        const glm::mat3 absA2B = absMatrix(a2B) + glm::epsilon<float>();
        const glm::vec3 delta = initialDelta + movement;

        glm::vec3 minPenetrationAxis {};
        float minPenetrationDepth = std::numeric_limits<float>::infinity();

        // The axes of B
        for (int i = 0; i < 3; ++i) {

            const float rb = bHalfSize[i];
            const float ra = glm::dot(aHalfSize, glm::row(absA2B, i));
            const float penetrationDepth = ra + rb - glm::abs(delta[i]);

            if (penetrationDepth < minPenetrationDepth) {

                if (penetrationDepth < 0.f)
                    return std::nullopt;

                minPenetrationAxis = getAxis(i);
                minPenetrationDepth = penetrationDepth;
            }
        }

        // The axes of A
        for (int i = 0; i < 3; ++i) {

            const float rb = glm::dot(bHalfSize, absA2B[i]);
            const float ra = aHalfSize[i];
            const float penetrationDepth = ra + rb - glm::abs(glm::dot(delta, absA2B[i]));

            if (penetrationDepth < minPenetrationDepth) {

                if (penetrationDepth < 0.f)
                    return std::nullopt;

                minPenetrationAxis  = a2B[i];
                minPenetrationDepth = penetrationDepth;
            }
        }

        if (!satOBBAxisTest<0, 0>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<1, 0>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<2, 0>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<0, 1>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<1, 1>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<2, 1>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<0, 2>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<1, 2>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;
        if (!satOBBAxisTest<2, 2>(aHalfSize, bHalfSize, delta, a2B, absA2B, minPenetrationDepth, minPenetrationAxis)) return std::nullopt;

        const float directionMultiplier = glm::dot(delta, minPenetrationAxis) < 0.f ? -1.f : 1.f;
        const glm::vec3 normal = glm::normalize(minPenetrationAxis * directionMultiplier);
        return Hit {normal, 1.f, normal * minPenetrationDepth};
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

    const glm::mat3 world2B = glm::transpose(b.rotation);
    const glm::vec3& spherePosition = world2B * (a.position - b.center) + b.center;
    const glm::vec3& localMovement  = world2B * movement;

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

    const glm::mat3 world2B = glm::transpose(b.rotation);
    std::optional<Hit> hit = obbVsAABBInternal(a.halfSize, b.halfSize, world2B, world2B * (a.center - b.center), world2B * movement);
    if (!hit)
        return hit;

    hit->normal              = b.rotation * hit->normal;
    hit->depenetrationOffset = b.rotation * hit->depenetrationOffset;
    return hit;
}

std::optional<Hit> en::collisionDetection::OBBVsAABB(OBBCollider& a, AABBCollider& b, const glm::vec3& movement) {

    return obbVsAABBInternal(a.halfSize, b.halfSize, a.rotation, a.center - b.center, movement);
}

std::optional<Hit> en::collisionDetection::OBBVsOBB(OBBCollider& a, OBBCollider& b, const glm::vec3& movement) {

    const glm::mat3 world2B = glm::transpose(b.rotation);
    const glm::mat3 a2B = world2B * a.rotation;
    std::optional<Hit> hit = obbVsAABBInternal(a.halfSize, b.halfSize, a2B, world2B * (a.center - b.center), world2B * movement);
    if (!hit)
        return hit;

    assert(glm::isOrthogonal(b.rotation, 0.00001f));
    hit->normal              = b.rotation * hit->normal;
    hit->depenetrationOffset = b.rotation * hit->depenetrationOffset;

    //std::cout << hit->normal << " " << glm::length(hit->depenetrationOffset) << std::endl;

    return hit;
}
