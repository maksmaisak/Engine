//
// Created by Maksym Maisak on 24/12/18.
//

#ifndef SAXION_Y2Q2_RENDERING_CAMERA_H
#define SAXION_Y2Q2_RENDERING_CAMERA_H

#include "glm.hpp"
#include "ComponentsToLua.h"

namespace en {

    struct Camera {

        LUA_TYPE(Camera)

        static void initializeMetatable(LuaState&);

        bool isOrthographic = false;
        float fov = 60.f;
        float orthographicHalfSize = 5;
        float nearPlaneDistance = 0.1f;
        float farPlaneDistance  = 1000.f;
    };
}

#endif //SAXION_Y2Q2_RENDERING_CAMERA_H
