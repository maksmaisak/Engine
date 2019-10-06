//
// Created by Maksym Maisak on 2019-02-13.
//

#ifndef ENGINE_UISPRITE_H
#define ENGINE_UISPRITE_H

#include <memory>
#include "Material.h"
#include "ComponentsToLua.h"

namespace en {

    struct UISprite {

        LUA_TYPE(UISprite);
        static void initializeMetatable(LuaState& lua);
        static UISprite& addFromLua(Actor& actor, LuaState& lua);

        std::shared_ptr<Material> material;
        bool isEnabled = true;
    };
}

#endif //ENGINE_UISPRITE_H
