//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef SAXION_Y2Q2_RENDERING_HIT_H
#define SAXION_Y2Q2_RENDERING_HIT_H

#include "glm.hpp"

namespace en {

    struct Hit {

        glm::vec3 normal;
        float timeOfImpact = 1.f;

        inline Hit(const glm::vec3& normal, float timeOfImpact) : normal(normal), timeOfImpact(timeOfImpact) {}
    };
}

#endif //SAXION_Y2Q2_RENDERING_HIT_H
