//
// Created by Maksym Maisak on 2019-02-13.
//

#ifndef SAXION_Y2Q2_RENDERING_SPRITE_H
#define SAXION_Y2Q2_RENDERING_SPRITE_H

#include <memory>
#include "Material.h"
#include "ComponentsToLua.h"

namespace en {

    struct Sprite {

        LUA_TYPE(Sprite);
        static void initializeMetatable(LuaState& lua);
        static Sprite& addFromLua(Actor& actor, LuaState& lua);

        std::shared_ptr<Material> material;
        bool isEnabled = true;
    };
}

#endif //SAXION_Y2Q2_RENDERING_SPRITE_H
