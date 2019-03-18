//
// Created by Maksym Maisak on 2019-03-18.
//

#include "CollisionDetectionDiscrete.h"
#include "SphereCollider.h"
#include "AABBCollider.h"

using namespace en;
using namespace en::collisionDetectionDiscrete;

std::optional<Hit> en::collisionDetectionDiscrete::sphereVsSphere(SphereCollider& a, SphereCollider& b) {

    const float sumRadius = a.radius + b.radius;
    const glm::vec3 delta = a.position - b.position;
    const float distanceSqr = glm::length2(delta);
    if (distanceSqr > sumRadius * sumRadius)
        return std::nullopt;

    return Hit {delta / glm::sqrt(distanceSqr)};
}

std::optional<Hit> en::collisionDetectionDiscrete::AABBVsAABB(AABBCollider& a, AABBCollider& b) {

    return std::nullopt;
}

std::optional<Hit> en::collisionDetectionDiscrete::sphereVsAABB(SphereCollider& a, AABBCollider& b) {

    return std::nullopt;
}