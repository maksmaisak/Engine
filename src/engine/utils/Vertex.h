//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef ENGINE_VERTEX_H
#define ENGINE_VERTEX_H

#include "glm.h"

namespace en {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;
    };
}

#endif //ENGINE_VERTEX_H
