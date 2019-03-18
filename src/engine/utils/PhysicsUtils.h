//
// Created by Maksym Maisak on 23/10/18.
//

#ifndef SAXION_Y2Q1_CPP_PHYSICSUTILS_H
#define SAXION_Y2Q1_CPP_PHYSICSUTILS_H

#include <optional>
#include "MyMath.h"
#include "glm.hpp"
#include <glm/gtc/epsilon.hpp>

namespace en {

    struct Hit {

        glm::vec3 normal;
        float timeOfImpact = 0.f;

        inline Hit(const glm::vec3& normal, float timeOfImpact) : normal(normal), timeOfImpact(timeOfImpact) {}
    };

    inline std::optional<Hit> sphereVsSphereContinuous(
        const glm::vec3& moverPosition, float moverRadius, const glm::vec3& movement,
        const glm::vec3& otherPosition, float otherRadius
    ) {

        glm::vec3 relativePosition = moverPosition - otherPosition;

        float a = glm::length2(movement);
        if (a <= glm::epsilon<float>()) return std::nullopt;

        float b = 2.f * dot(relativePosition, movement);
        float c = glm::length2(relativePosition) - glm::length2(moverRadius + otherRadius);

        // If moving out
        if (b >= 0.f) return std::nullopt;

        // If already overlapping.
        if (c < 0.f) return std::make_optional<Hit>(glm::normalize(relativePosition), 0.f);

        float d = b * b - 4.f * a * c;
        if (d < 0.f) return std::nullopt;

        float t = (-b - glm::sqrt(d)) / (2.f * a);

        if (t <  0.f) return std::nullopt;
        if (t >= 1.f) return std::nullopt;

        return std::make_optional<Hit>(glm::normalize(relativePosition + movement * t), t);
    }

    /// A helper for resolving collisions between physical bodies
    /// in a way which obeys conservation of momentum.
    /// Assumes `normal` is normalized.
    inline void resolve(
        glm::vec3& aVelocity, float aInverseMass,
        glm::vec3& bVelocity, float bInverseMass,
        glm::vec3 normal, float bounciness = 1.f
    ) {
        float aSpeedAlongNormal = glm::dot(normal, aVelocity);
        float bSpeedAlongNormal = glm::dot(normal, bVelocity);

        if (aSpeedAlongNormal - bSpeedAlongNormal > 0.f) return;

        float u =
            (aSpeedAlongNormal * bInverseMass + bSpeedAlongNormal * aInverseMass) /
            (aInverseMass + bInverseMass);

        float aDeltaSpeedAlongNormal = -(1.f + bounciness) * (aSpeedAlongNormal - u);
        aVelocity += normal * aDeltaSpeedAlongNormal;

        float bDeltaSpeedAlongNormal = -(1.f + bounciness) * (bSpeedAlongNormal - u);
        bVelocity += normal * bDeltaSpeedAlongNormal;
    }
}

#endif //SAXION_Y2Q1_CPP_PHYSICSUTILS_H
