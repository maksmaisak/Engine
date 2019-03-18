//
// Created by Maksym Maisak on 2019-02-14.
//

#ifndef SAXION_Y2Q2_RENDERING_UIRECT_H
#define SAXION_Y2Q2_RENDERING_UIRECT_H

#include "glm.hpp"
#include "ComponentsToLua.h"

namespace en {

    struct UIRect {

        LUA_TYPE(UIRect);
        static void initializeMetatable(LuaState& lua);

        // In normalized coordinates [0..1]
        glm::vec2 anchorMin = {0, 0};
        glm::vec2 anchorMax = {1, 1};

        // In pixels
        glm::vec2 offsetMin = {0, 0};
        glm::vec2 offsetMax = {0, 0};

        // Rotation and scale are applied from this pivot.
        // In normalized coordinates [0..1]
        glm::vec2 pivot = {0.5f, 0.5f};

        bool isEnabled  = true;
        bool wasEnabled = false;

        glm::vec2 computedSize;
        bool isMouseOver  = false;
        bool wasMouseOver = false;
    };
}

#endif //SAXION_Y2Q2_RENDERING_UIRECT_H
