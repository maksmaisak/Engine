//
// Created by Maksym Maisak on 2019-01-19.
//

#include "RenderInfo.h"
#include <memory>
#include "Material.h"
#include "Resources.h"
#include "Exception.h"

using namespace en;

namespace {

    std::shared_ptr<Material> readMaterial(RenderInfo& renderInfo, LuaState& lua) {

        lua_getfield(lua, -1, "material");
        auto p = lua::PopperOnDestruct(lua);

        if (lua.is<std::shared_ptr<Material>>()) {
            return lua.to<std::shared_ptr<Material>>();
        }

        return std::make_shared<Material>(lua);
    }
}

RenderInfo& RenderInfo::addFromLua(Actor& actor, LuaState& lua) {

    auto& renderInfo = actor.add<en::RenderInfo>();

    renderInfo.material = readMaterial(renderInfo, lua);
    //renderInfo.isBatchingStatic = lua.tryGetField<bool>("isBatchingStatic").value_or(false);

    return renderInfo;
}

void RenderInfo::initializeMetatable(LuaState& lua) {

    lua::addProperty(lua, "isEnabled", lua::property(&RenderInfo::isEnabled));

    lua::addProperty(lua, "mesh", lua::writeonlyProperty([](ComponentReference<RenderInfo>& renderInfo, const std::string& value) {
        renderInfo->model = Resources<Model>::get("assets/" + value);
    }));

    lua::addProperty(lua, "isBatchingStatic", lua::property(
        [](ComponentReference<RenderInfo>& ref) {return ref->isBatchingStatic;},
        [](ComponentReference<RenderInfo>& ref, bool isBatchingStatic) {

            auto& renderInfo = *ref;
            if (renderInfo.isInBatch && !isBatchingStatic)
                throw utils::Exception("You can't set isBatchingStatic to false after the mesh has already been batched.");

            renderInfo.isBatchingStatic = isBatchingStatic;
        }
    ));
}
