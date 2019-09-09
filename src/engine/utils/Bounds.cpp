//
// Created by Maksym Maisak on 2019-04-04.
//

#include "Bounds.h"

using namespace utils;

Bounds::Bounds(const Bounds2D& other) :
    min(other.min.x, 0.f, other.min.y),
    max(other.max.x, 0.f, other.max.y)
{}

Bounds::Bounds(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

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
        if (movement[i] > 0.f)
            max[i] += movement[i];
        else
            min[i] += movement[i];
    }
}

Bounds2D::Bounds2D(const glm::vec2& min, const glm::vec2& max) : min(min), max(max) {}
Bounds2D::Bounds2D(const Bounds& other) :
    min(other.min.x, other.min.z),
    max(other.max.x, other.max.z)
{}

bool Bounds2D::intersect(const Bounds2D& other) const {

    for (int i = 0; i < 2; ++i)
        if (other.min[i] > max[i] || other.max[i] < min[i])
            return false;

    return true;
}

glm::vec2 Bounds2D::clamp(const glm::vec2& other) const {

    return glm::clamp(other, min, max);
}

Bounds2D Bounds2D::clamp(const Bounds2D& other) const {

    const glm::vec2 otherHalfSize = (other.max - other.min) * 0.5f;

    const glm::vec2 shrunkMin = min + otherHalfSize;
    const glm::vec2 shrunkMax = max - otherHalfSize;
    const glm::vec2 center = glm::clamp(other.max - otherHalfSize, shrunkMin, shrunkMax);

    return {center - otherHalfSize, center + otherHalfSize};
}

void Bounds2D::expandByMovement(const glm::vec2& movement) {

    for (int i = 0; i < 2; ++i) {
        if (movement[i] > 0.f)
            max[i] += movement[i];
        else
            min[i] += movement[i];
    }
}