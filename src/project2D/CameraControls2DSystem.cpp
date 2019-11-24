//
// Created by Maksym Maisak on 2019-09-27.
//

#include "CameraControls2DSystem.h"

#include <algorithm>
#include "Keyboard.h"
#include "Mouse.h"
#include "Engine.h"
#include "Transform.h"
#include "Camera.h"

namespace {

    constexpr float MinCameraSize = 5.f;
    constexpr float MaxCameraSize = 40.f;
    constexpr float ZoomSpeedMultiplier = 20.f;

    constexpr float MinMovementSpeed = 3.f;
    constexpr float MaxMovementSpeed = 24.f;

    const int forwardKeys[] = { GLFW_KEY_W, GLFW_KEY_UP };
    const int backKeys[]    = { GLFW_KEY_S, GLFW_KEY_DOWN };
    const int rightKeys[]   = { GLFW_KEY_D, GLFW_KEY_RIGHT };
    const int leftKeys[]    = { GLFW_KEY_A, GLFW_KEY_LEFT };

    glm::vec2 getMoveInput() {

        glm::vec2 result = {};

        const auto isKeyPressed = [window = en::Engine::get().getWindow().getUnderlyingWindow()](int keyCode) {
            return glfwGetKey(window, keyCode);
        };

        if (std::any_of(std::begin(rightKeys), std::end(rightKeys), isKeyPressed)) {
            result.x += 1.f;
        }

        if (std::any_of(std::begin(leftKeys), std::end(leftKeys), isKeyPressed)) {
            result.x -= 1.f;
        }

        if (std::any_of(std::begin(forwardKeys), std::end(forwardKeys), isKeyPressed)) {
            result.y += 1.f;
        }

        if (std::any_of(std::begin(backKeys), std::end(backKeys), isKeyPressed)) {
            result.y -= 1.f;
        }

        return result;
    }

    float getZoomDelta() {

        float scrollDelta = en::Mouse::getScrollDelta();

        if (en::Keyboard::isHeld(GLFW_KEY_LEFT_BRACKET)) {
            scrollDelta += 1.f;
        }

        if (en::Keyboard::isHeld(GLFW_KEY_RIGHT_BRACKET)) {
            scrollDelta -= 1.f;
        }

        return scrollDelta;
    }
}

void CameraControls2DSystem::update(float) {

    en::Actor cameraActor = m_engine->getMainCamera();
    if (!cameraActor) {
        return;
    }

    const float dt = m_engine->getDeltaTimeRealtime();

    auto& camera = cameraActor.get<en::Camera>();
    camera.orthographicHalfSize += getZoomDelta() * ZoomSpeedMultiplier * dt;
    camera.orthographicHalfSize = glm::clamp(camera.orthographicHalfSize, MinCameraSize, MaxCameraSize);

    const float t = (camera.orthographicHalfSize - MinCameraSize) / (MaxCameraSize - MinCameraSize);
    float movementSpeed = glm::lerp(MinMovementSpeed, MaxMovementSpeed, t);

    auto& transform = cameraActor.get<en::Transform>();
    transform.move(glm::vec3(getMoveInput() * (movementSpeed * dt), 0.f));
}
