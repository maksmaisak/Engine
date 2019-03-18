//
// Created by Maksym Maisak on 24/12/18.
//

#ifndef SAXION_Y2Q2_RENDERING_LIGHT_H
#define SAXION_Y2Q2_RENDERING_LIGHT_H

#include "ComponentsToLua.h"
#include "glm.hpp"
#include <GL/glew.h>

namespace en {

    class Light final {

        LUA_TYPE(Light);

    public:

        enum class Kind {

            POINT = 0,
            DIRECTIONAL,
            SPOT,

            COUNT
        };

        static void addFromLua(Actor& actor, LuaState& lua);
        static void initializeMetatable(LuaState& lua);

        explicit Light(Kind kind = Kind::POINT);

        struct Falloff {
            float constant  = 1;
            float linear    = 0;
            float quadratic = 1;
        };

        glm::vec3 colorAmbient = {0, 0, 0};
        glm::vec3 color = {1, 1, 1};
        float intensity = 1;
        float range = 10;
        Falloff falloff = {1, 0, 1};

        Kind kind = Kind::POINT;
        // cos of angle in radians. For spotlights only.
        float spotlightInnerCutoff = glm::cos(glm::radians(40.f));
        float spotlightOuterCutoff = glm::cos(glm::radians(50.f));

        // Shadowmapping data
        float nearPlaneDistance = 0.1f;
        float farPlaneDistance = 100.f;
        glm::mat4 matrixPV; // The P*V matrix for directional shadowmapping
    };

    struct FalloffFunctions {

        static constexpr Light::Falloff Constant  = {1, 0, 0};
        static constexpr Light::Falloff Linear    = {1, 1, 0};
        static constexpr Light::Falloff Quadratic = {1, 0, 1};
    };
}

#endif //SAXION_Y2Q2_RENDERING_LIGHT_H
