//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef ENGINE_HIT_H
#define ENGINE_HIT_H

#include "glm.hpp"

namespace en {

    struct Hit {

        glm::vec3 normal;

        float timeOfImpact = 1.f;

        glm::vec3 depenetrationOffset;

        //Hit() = default;
        //inline Hit(const glm::vec3& normal, float timeOfImpact = 1.f) : normal(normal), timeOfImpact(timeOfImpact) {}
    };
}

#endif //ENGINE_HIT_H
