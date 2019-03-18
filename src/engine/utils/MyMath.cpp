//
// Created by Maksym Maisak on 11/10/18.
//

#include "MyMath.h"
#include <cmath>
#include <random>
#include <functional>

namespace en {

    float angleDegrees(sf::Vector2f vec) {

        return atan2f(vec.y, vec.x) * RAD2DEG;
    }

    bool isZero(float value) {

        return std::abs(value) < EPSILON;
    }

    bool isZero(const sf::Vector2f& vec) {

        return isZero(vec.x) && isZero(vec.y);
    }

    sf::Vector2f polar2Cartesian(float angle, float radius) {

        return sf::Vector2f(
            cosf(angle) * radius,
            sinf(angle) * radius
        );
    }

    sf::Vector2f& normalize(sf::Vector2f& vec) {

        float magnitude = std::hypot(vec.x, vec.y);
        vec.x /= magnitude;
        vec.y /= magnitude;
        return vec;
    }

    sf::Vector2f normalized(const sf::Vector2f& vec) {

        sf::Vector2f result = vec;
        return normalize(result);
    }

    sf::Vector2f& truncate(sf::Vector2f& vec, float maxMagnitude) {

        if (en::sqrMagnitude(vec) > maxMagnitude * maxMagnitude) {
            en::normalize(vec) *= maxMagnitude;
        }

        return vec;
    }

    sf::Vector2f truncated(const sf::Vector2f& vec, float maxMagnitude) {

        sf::Vector2f result = vec;
        return truncate(result, maxMagnitude);
    }

    float magnitude(const sf::Vector2f& vec) {

        return hypotf(vec.x, vec.y);
    }

    float sqrMagnitude(const sf::Vector2f& vec) {

        return vec.x * vec.x + vec.y * vec.y;
    }

    std::default_random_engine randomEngine;
    std::uniform_real_distribution<float> distribution;
    auto randomSample = std::bind(distribution, randomEngine);

    sf::Vector2f randomInCircle(float radius) {

        return polar2Cartesian(randomSample() * 2.f * PI, randomSample() * radius);
    }

    float random(float min, float max) {

        return min + (max - min) * randomSample();
    }
}
