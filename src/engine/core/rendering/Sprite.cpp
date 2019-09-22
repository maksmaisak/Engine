//
// Created by Maksym Maisak on 2019-02-13.
//

#include "Sprite.h"
#include <memory>
#include "Texture.h"

using namespace en;

std::shared_ptr<Material> readMaterial(LuaState& lua) {

    lua_getfield(lua, -1, "material");
    auto p = lua::PopperOnDestruct(lua);

    if (lua.is<std::shared_ptr<Material>>())
        return lua.to<std::shared_ptr<Material>>();

    return std::make_shared<Material>(lua);
}

Sprite& Sprite::addFromLua(Actor& actor, LuaState& lua) {

    auto& sprite = actor.add<Sprite>();
    sprite.material = readMaterial(lua);
    return sprite;
}

void Sprite::initializeMetatable(LuaState& lua) {

    lua::addProperty(lua, "isEnabled", lua::property(&Sprite::isEnabled));

    lua::addProperty(lua, "textureSize", lua::readonlyProperty([](ComponentReference<Sprite>& ref) {

        auto& material = ref->material;
        if (!material)
            return glm::vec2(0);

        const auto& size = material->getUniformValue<std::shared_ptr<Texture>>("spriteTexture")->getSize();
        return glm::vec2(size.x, size.y);
    }));
}
