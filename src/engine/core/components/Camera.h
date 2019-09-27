//
// Created by Maksym Maisak on 24/12/18.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <utility>
#include "glm.h"
#include "ComponentsToLua.h"

namespace en {

    class Engine;

    struct Camera {

        LUA_TYPE(Camera)

        static void initializeMetatable(LuaState&);

        bool isOrthographic = false;
        float fov = 60.f;
        float orthographicHalfSize = 5.f;
        float nearPlaneDistance = 0.1f;
        float farPlaneDistance  = 1000.f;

        glm::vec2 getOrthographicExtents(Engine& engine) const;
        glm::mat4 getCameraProjectionMatrix(Engine& engine, std::optional<float> rangeLimit = std::nullopt) const;
    };
}

#endif //ENGINE_CAMERA_H
