//
// Created by Maksym Maisak on 11/10/18.
//

#ifndef SAXION_Y2Q1_CPP_VECTORMATH_H
#define SAXION_Y2Q1_CPP_VECTORMATH_H

#include <SFML/Graphics.hpp>
#include <cmath>

namespace en {

    inline constexpr float PI = 3.14159265359f;
    inline constexpr float PI2 = PI * 2.0f;
    inline constexpr float RAD2DEG = 360.0f / PI2;
    inline constexpr float DEG2RAD = PI2 / 360.0f;
    inline constexpr float EPSILON = 0.001f;

    float angleDegrees(sf::Vector2f vec);
    sf::Vector2f& normalize(sf::Vector2f& vec);
    sf::Vector2f normalized(const sf::Vector2f& vec);
    sf::Vector2f& truncate(sf::Vector2f& vec, float maxMagnitude);
    sf::Vector2f truncated(const sf::Vector2f& vec, float maxMagnitude);
    float magnitude(const sf::Vector2f& vec);
    float sqrMagnitude(const sf::Vector2f& vec);
    bool isZero(float value);
    bool isZero(const sf::Vector2f& vec);
    sf::Vector2f polar2Cartesian(float angle, float radius);

    inline float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
        return a.x * b.x + a.y * b.y;
    }

    float random(float min = 0.f, float max = 1.f);
    sf::Vector2f randomInCircle(float radius = 1.f);

    inline sf::Vector2f getPosition(const sf::Transform& transform) {
        return transform.transformPoint(0, 0);
    }

    inline sf::Vector2f getForward(const sf::Transform& transform) {
        sf::Vector2f forward = transform.transformPoint(0, -1) - transform.transformPoint(0, 0);
        return normalize(forward);
    }
}


#endif //SAXION_Y2Q1_CPP_VECTORMATH_H
