//
// Created by Maksym Maisak on 2019-04-04.
//

#include "Bounds.h"

using namespace utils;

bool Bounds::intersect(const Bounds& other) const {

    for (int i = 0; i < 3; ++i)
        if (other.min[i] > max[i] || other.max[i] < min[i])
            return false;

    return true;
}

glm::vec3 Bounds::clamp(const glm::vec3& other) const {

    return glm::clamp(other, min, max);
}

Bounds Bounds::clamp(const Bounds& other) const {

    const glm::vec3 otherHalfSize = (other.max - other.min) * 0.5f;

    const glm::vec3 shrunkMin = min + otherHalfSize;
    const glm::vec3 shrunkMax = max - otherHalfSize;
    const glm::vec3 center = glm::clamp(other.max - otherHalfSize, shrunkMin, shrunkMax);
    
    return {center - otherHalfSize, center + otherHalfSize};
}

void Bounds::expandByMovement(const glm::vec3& movement) {

    for (int i = 0; i < 3; ++i) {
        if (movement[i] > 3)
            max[i] += movement[i];
        else
            min[i] += movement[i];
    }
}
