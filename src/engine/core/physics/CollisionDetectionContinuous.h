//
// Created by Maksym Maisak on 23/10/18.
//

#ifndef SAXION_Y2Q1_CPP_COLLISIONDETECTIONCONTINUOUS_H
#define SAXION_Y2Q1_CPP_COLLISIONDETECTIONCONTINUOUS_H

#include <optional>
#include "glm.hpp"
#include <glm/gtc/epsilon.hpp>

namespace en {

    struct HitContinuous {

        glm::vec3 normal;
        float timeOfImpact = 0.f;

        inline HitContinuous(const glm::vec3& normal, float timeOfImpact) : normal(normal), timeOfImpact(timeOfImpact) {}
    };

    inline std::optional<HitContinuous> sphereVsSphereContinuous(
        const glm::vec3& moverPosition, float moverRadius, const glm::vec3& movement,
        const glm::vec3& otherPosition, float otherRadius
    ) {

        glm::vec3 relativePosition = moverPosition - otherPosition;

        float a = glm::length2(movement);
        if (a <= glm::epsilon<float>()) return std::nullopt;

        float b = 2.f * dot(relativePosition, movement);
        float c = glm::length2(relativePosition) - glm::length2(moverRadius + otherRadius);

        // If moving out
        if (b >= 0.f)
            return std::nullopt;

        // If already overlapping.
        if (c < 0.f)
            return std::make_optional<HitContinuous>(glm::normalize(relativePosition), 0.f);

        float d = b * b - 4.f * a * c;
        if (d < 0.f)
            return std::nullopt;

        float t = (-b - glm::sqrt(d)) / (2.f * a);

        if (t <  0.f)
            return std::nullopt;
        if (t >= 1.f)
            return std::nullopt;

        return std::make_optional<HitContinuous>(glm::normalize(relativePosition + movement * t), t);
    }
}

#endif //SAXION_Y2Q1_CPP_COLLISIONDETECTIONCONTINUOUS_H
