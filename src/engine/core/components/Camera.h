//
// Created by Maksym Maisak on 24/12/18.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

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

#endif //ENGINE_CAMERA_H
