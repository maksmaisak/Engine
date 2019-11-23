//
// Created by Maksym Maisak on 2019-01-13.
//

#include "LightPropertyAnimator.h"
#include "Light.h"
#include "glm.h"

void LightPropertyAnimator::start() {

    m_initialLight = getActor().get<en::Light>();
    m_startTime = en::Clock::now();
}

void LightPropertyAnimator::update(float dt) {

    auto& light = m_actor.get<en::Light>();

    const float time = en::GameTime::asSeconds(en::Clock::now() - m_startTime);
    const float sinTime = glm::sin(time);

    const auto rotateColor = [&](const glm::vec3& color) {
        return glm::abs(glm::rotate(color, time, {1, 1, 1}));
    };

    light.color = rotateColor(m_initialLight.color);
    light.colorAmbient = rotateColor(m_initialLight.colorAmbient);
    light.spotlightInnerCutoff = m_initialLight.spotlightInnerCutoff * sinTime;
    light.spotlightOuterCutoff = m_initialLight.spotlightOuterCutoff * sinTime;
    light.falloff.linear    = glm::abs(sinTime);
    light.falloff.quadratic = 1.f - glm::abs(sinTime);

    light.kind = static_cast<en::Light::Kind>(
        ((int)m_initialLight.kind + (int)(time * 0.25f)) % (int)en::Light::Kind::COUNT
    );
}
