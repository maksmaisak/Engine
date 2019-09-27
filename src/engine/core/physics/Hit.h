//
// Created by Maksym Maisak on 2019-03-18.
//

#ifndef ENGINE_HIT_H
#define ENGINE_HIT_H

#include "glm.h"

namespace en {

    struct Hit {

        glm::vec3 normal;

        float timeOfImpact = 1.f;

        // TODO Replace with a float. It's always along the normal anyway
        glm::vec3 depenetrationOffset;
    };
}

#endif //ENGINE_HIT_H
