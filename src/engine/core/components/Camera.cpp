//
// Created by Maksym Maisak on 2019-02-12.
//

#include "Camera.h"
#include "Engine.h"

using namespace en;

void Camera::initializeMetatable(LuaState& lua) {

    lua::addProperty(lua, "isOrthographic", lua::property(&Camera::isOrthographic));
    lua::addProperty(lua, "fov", lua::property(&Camera::fov));
    lua::addProperty(lua, "orthographicHalfSize" , lua::property(&Camera::orthographicHalfSize));
    lua::addProperty(lua, "nearPlaneDistance", lua::property(&Camera::nearPlaneDistance));
    lua::addProperty(lua, "farPlaneDistance" , lua::property(&Camera::farPlaneDistance));
}

glm::mat4 Camera::getCameraProjectionMatrix(Engine& engine, std::optional<float> rangeLimit) const {

    const float aspectRatio = engine.getWindow().getAspectRatio();
    const float farPlaneDistanceEffective = std::min(rangeLimit.value_or(farPlaneDistance), farPlaneDistance);

    if (isOrthographic) {

        const glm::vec2 halfSize = {
            orthographicHalfSize * aspectRatio,
            orthographicHalfSize
        };

        return glm::ortho(
            -halfSize.x, halfSize.x,
            -halfSize.y, halfSize.y,
            nearPlaneDistance, farPlaneDistanceEffective
        );
    }

    return glm::perspective(
        glm::radians(fov),
        aspectRatio,
        nearPlaneDistance,
        farPlaneDistanceEffective
    );
}

glm::vec2 Camera::getOrthographicExtents(Engine& engine) const {

    return {
        orthographicHalfSize * engine.getWindow().getAspectRatio(),
        orthographicHalfSize
    };
}
