//
// Created by Maksym Maisak on 2019-03-04.
//

#ifndef SAXION_Y2Q2_RENDERING_EASE_H
#define SAXION_Y2Q2_RENDERING_EASE_H

#include <functional>

namespace ease {

    using Ease = std::function<float(float)>;

    float linear(float t);

    float inQuad (float t);
    float inCubic(float t);
    float inQuart(float t);
    float inQuint(float t);
    float inExpo (float t);
    float inCirc (float t);
    float inSine (float t);

    float outQuad (float t);
    float outCubic(float t);
    float outQuart(float t);
    float outQuint(float t);
    float outExpo (float t);
    float outCirc (float t);
    float outSine (float t);

    float inOutQuad (float t);
    float inOutCubic(float t);
    float inOutQuart(float t);
    float inOutQuint(float t);
    float inOutExpo (float t);
    float inOutCirc (float t);
    float inOutSine (float t);

    float punch(float t);
    float fluctuate(float t);
}

#endif //SAXION_Y2Q2_RENDERING_EASE_H
