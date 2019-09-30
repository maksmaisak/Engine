//
// Created by Maksym Maisak on 2019-09-27.
//

#include "CameraControls2DSystem.h"

#include <algorithm>
#include "KeyboardHelper.h"
#include "MouseHelper.h"
#include "Engine.h"
#include "Transform.h"
#include "Camera.h"

namespace {

    constexpr float MinCameraSize = 5.f;
    constexpr float MaxCameraSize = 40.f;
    constexpr float ZoomSpeedMultiplier = 20.f;

    constexpr float MinMovementSpeed = 3.f;
    constexpr float MaxMovementSpeed = 24.f;

    using Key = sf::Keyboard::Key;

    const Key forwardKeys[] = { Key::W, Key::Up };
    const Key backKeys[]    = { Key::S, Key::Down };
    const Key rightKeys[]   = { Key::D, Key::Right };
    const Key leftKeys[]    = { Key::A, Key::Left };

    glm::vec2 getMoveInput() {

        glm::vec2 result = {};

        if (std::any_of(std::begin(rightKeys), std::end(rightKeys), sf::Keyboard::isKeyPressed)) {
            result.x += 1.f;
        }

        if (std::any_of(std::begin(leftKeys), std::end(leftKeys), sf::Keyboard::isKeyPressed)) {
            result.x -= 1.f;
        }

        if (std::any_of(std::begin(forwardKeys), std::end(forwardKeys), sf::Keyboard::isKeyPressed)) {
            result.y += 1.f;
        }

        if (std::any_of(std::begin(backKeys), std::end(backKeys), sf::Keyboard::isKeyPressed)) {
            result.y -= 1.f;
        }

        return result;
    }

    float getZoomDelta() {

        float scrollDelta = utils::MouseHelper::getScrollDelta();

        if (sf::Keyboard::isKeyPressed(Key::LBracket)) {
            scrollDelta += 1.f;
        }

        if (sf::Keyboard::isKeyPressed(Key::RBracket)) {
            scrollDelta -= 1.f;
        }

        return scrollDelta;
    }
}

void CameraControls2DSystem::update(float dt) {

    en::Actor cameraActor = m_engine->getMainCamera();
    if (!cameraActor) {
        return;
    }

    auto& camera = cameraActor.get<en::Camera>();
    camera.orthographicHalfSize += getZoomDelta() * ZoomSpeedMultiplier * dt;
    camera.orthographicHalfSize = glm::clamp(camera.orthographicHalfSize, MinCameraSize, MaxCameraSize);

    const float t = (camera.orthographicHalfSize - MinCameraSize) / (MaxCameraSize - MinCameraSize);
    float movementSpeed = glm::lerp(MinMovementSpeed, MaxMovementSpeed, t);

    auto& transform = cameraActor.get<en::Transform>();
    transform.move(glm::vec3(getMoveInput() * (movementSpeed * dt), 0.f));
}
