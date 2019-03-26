//
// Created by Maksym Maisak on 2019-02-13.
//

#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

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

#endif //ENGINE_SPRITE_H
