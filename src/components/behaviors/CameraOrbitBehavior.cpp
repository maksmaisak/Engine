//
// Created by Maksym Maisak on 16/12/18.
//

#include "CameraOrbitBehavior.h"
#include <SFML/Window.hpp>
#include "glm.hpp"
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#include "components/Transform.h"

CameraOrbitBehavior& CameraOrbitBehavior::addFromLua(en::Actor& actor, en::LuaState& lua) {

    auto& behavior = actor.add<CameraOrbitBehavior>();

    auto targetName = lua.tryGetField<std::string>("target");
    if (targetName) behavior.m_target = actor.getEngine().findByName(*targetName);

    auto distance = lua.tryGetField<float>("distance");
    if (distance) behavior.m_distance = *distance;

    auto minTilt = lua.tryGetField<float>("minTilt");
    if (minTilt) behavior.m_minTilt = *minTilt;

    auto maxTilt = lua.tryGetField<float>("maxTilt");
    if (maxTilt) behavior.m_maxTilt = *maxTilt;

    auto rotationSpeed = lua.tryGetField<float>("rotationSpeed");
    if (rotationSpeed) behavior.m_rotationSpeed = *rotationSpeed;

    return behavior;
}

CameraOrbitBehavior::CameraOrbitBehavior(
    en::Actor actor,
    float distance,
    float minTilt,
    float maxTilt,
    float rotationSpeed
) :
    Behavior(actor),
    m_distance(distance),
    m_minTilt(minTilt),
    m_maxTilt(maxTilt),
    m_rotationSpeed(rotationSpeed)
{
    m_previousMousePosition = sf::Mouse::getPosition();
}

void CameraOrbitBehavior::update(float dt) {

    if (!m_target) return;

    sf::Vector2i input = updateMouseInput();

    const auto& targetTransform = m_target.get<en::Transform>();
    auto& ownTransform = m_actor.get<en::Transform>();

    glm::vec3 targetPosition = targetTransform.getWorldPosition();
    glm::vec3 offsetDirection = glm::normalize(ownTransform.getLocalPosition() - targetPosition);
    if (glm::any(glm::isnan(offsetDirection))) offsetDirection = glm::vec3(0, 0, 1);
    offsetDirection = glm::rotate(offsetDirection, -input.x * m_rotationSpeed * dt, glm::vec3(0, 1, 0));

    glm::vec3 flatOffsetDirection = glm::normalize(glm::vec3(offsetDirection.x, 0, offsetDirection.z));
    if (glm::any(glm::isnan(flatOffsetDirection))) flatOffsetDirection = glm::vec3(0, 0, 1);
    glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), flatOffsetDirection);

    float elevationAngle = glm::orientedAngle(offsetDirection, flatOffsetDirection, right);
    elevationAngle -= input.y * m_rotationSpeed * dt;
    elevationAngle = glm::clamp(elevationAngle, glm::radians(m_minTilt), glm::radians(m_maxTilt));
    offsetDirection = glm::rotate(flatOffsetDirection, -elevationAngle, right);

    glm::vec3 forward = offsetDirection;
    glm::vec3 up = glm::cross(forward, right);

    /*ownTransform.setLocalTransform(glm::mat4(
        glm::vec4(right, 0),
        glm::vec4(up, 0),
        glm::vec4(offsetDirection, 0),
        glm::vec4(targetPosition + offsetDirection * m_distance, 1)
    ));*/

    ownTransform.setLocalPosition(targetPosition + offsetDirection * m_distance);
    ownTransform.setLocalRotation(glm::quatLookAt(-forward, up));
}

sf::Vector2i CameraOrbitBehavior::updateMouseInput() {

    sf::Vector2i currentMousePosition = sf::Mouse::getPosition();
    sf::Vector2i deltaMousePosition = currentMousePosition - m_previousMousePosition;

    // Wrap around screen
    // Doesn't seem to work on macOS, setPosition moves mouse out of screen bounds.
    /*
    auto desktop = sf::VideoMode::getDesktopMode();
    if (currentMousePosition.x > desktop.width) currentMousePosition.x -= desktop.width;
    if (currentMousePosition.x < 0) currentMousePosition.x += desktop.width;
    if (currentMousePosition.y > desktop.height) currentMousePosition.y -= desktop.height;
    if (currentMousePosition.y < 0) currentMousePosition.y += desktop.height;
    sf::Mouse::setPosition(currentMousePosition);
     */

    m_previousMousePosition = currentMousePosition;

    return deltaMousePosition;
}

void CameraOrbitBehavior::setTarget(const en::Actor& target) {

    m_target = target;
}
