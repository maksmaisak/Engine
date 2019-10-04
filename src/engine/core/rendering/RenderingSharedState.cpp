//
// Created by Maksym Maisak on 2019-09-22.
//

#include "RenderingSharedState.h"
#include "LuaState.h"

using namespace en;

RenderingSharedState::RenderingSharedState() :
    depthMaps(4, {1024, 1024}, 10, {64, 64}),
    enableStaticBatching(true),
    enableDebugOutput(false),
    referenceResolution(1920, 1080)
{}

void RenderingSharedState::loadConfigFromLua(LuaState& lua) {

    lua_getglobal(lua, "Config");
    const auto popConfig = lua::PopperOnDestruct(lua);

    referenceResolution = lua.tryGetField<glm::vec2>("referenceResolution").value_or(referenceResolution);
    enableStaticBatching = lua.tryGetField<bool>("enableStaticBatching").value_or(enableStaticBatching);
    enableDebugOutput = lua.tryGetField<bool>("enableDebugOutput").value_or(enableDebugOutput);
}
