//
// Created by Maksym Maisak on 14/10/18.
//

#include <cmath>
#include <iostream>
#include <algorithm>
#include "Transform.h"
#include "ParticleSystem.h"
#include "EntityRegistry.h"
#include "GameTime.h"
#include "Actor.h"

ParticleSystem::ParticleSystem(Actor actor, ParticleSystem::particleIndex_t maxNumParticles) :
    Behavior(actor),
    m_numActiveParticles(0),
    m_maxNumParticles(maxNumParticles),
    m_particles(maxNumParticles),
    m_timeOfLastEmission(Clock::now())
{}

void ParticleSystem::draw() {

    throw "Not implemented";

    /*if (!m_pDrawable) return;

    auto& target = m_engine->getWindow();
    for (std::size_t i = 0; i < m_numActiveParticles; ++i) {

        target.draw(
            *m_pDrawable,
            sf::RenderStates(sf::BlendAlpha, m_particles.at(i).transform, nullptr, nullptr)
        );
    }*/
}

void ParticleSystem::update(float dt) {

    if (m_isEmissionActive) {

        Duration timeSinceEmission = Clock::now() - m_timeOfLastEmission;

        if (timeSinceEmission > m_settings.emissionInterval) {
            do {
                timeSinceEmission -= m_settings.emissionInterval;
                updateParticle(emitParticle(), GameTime::asSeconds(timeSinceEmission));
            } while (timeSinceEmission > m_settings.emissionInterval);
        }

        m_timeOfLastEmission = Clock::now();
    }

    for (particleIndex_t i = 0; i < m_numActiveParticles; ++i) {
        updateParticle(i, dt);
    }
}

const ParticleSystem::Settings& ParticleSystem::getSettings() const {
    return m_settings;
}

void ParticleSystem::setSettings(const ParticleSystem::Settings& settings) {
    m_settings = settings;
}

bool ParticleSystem::getIsEmissionActive() const {
    return m_isEmissionActive;
}

void ParticleSystem::setIsEmissionActive(bool isEmissionActive) {

    if (isEmissionActive && !m_isEmissionActive) {
        m_timeOfLastEmission = Clock::now();
    }

    m_isEmissionActive = isEmissionActive;
}

ParticleSystem::particleIndex_t ParticleSystem::emitParticle() {

    if (m_numActiveParticles >= m_maxNumParticles) {
        destroyOldestParticle();
    } else if (m_particles.size() <= m_numActiveParticles) {
        m_particles.emplace_back();
    }

    Particle& particle = m_particles.at(m_numActiveParticles);

    particle.transformMatrix = m_actor.get<en::Transform>().getWorldTransform();
    particle.transformMatrix = glm::translate(particle.transformMatrix, glm::sphericalRand(m_settings.emissionRadius));
    particle.timeToDestroy = Clock::now() + m_settings.particleLifetime;
    particle.velocity = m_settings.startVelocity + glm::sphericalRand(m_settings.startVelocityRandomness);

    return m_numActiveParticles++;
}

void ParticleSystem::updateParticle(particleIndex_t i, float dt) {

    Particle& particle = m_particles.at(i);

    if (Clock::now() >= particle.timeToDestroy) {
        destroyParticle(i);
        return;
    }

    particle.transformMatrix = glm::translate(particle.transformMatrix, particle.velocity * dt);
}

void ParticleSystem::destroyParticle(particleIndex_t i) {

    assert(m_numActiveParticles > 0);
    m_numActiveParticles -= 1;

    std::swap(m_particles.at(i), m_particles.at(m_numActiveParticles));

    // Default-initialize for safety. Can just remove to improve performance, as long as everything gets reinitialized when emitting.
    m_particles.at(m_numActiveParticles) = {};
}

void ParticleSystem::destroyOldestParticle() {

    const auto it = std::min_element(m_particles.begin(), m_particles.end(), [](Particle& a, Particle& b){return a.timeToDestroy < b.timeToDestroy;});
    const auto index = static_cast<particleIndex_t>(std::distance(m_particles.begin(), it));
    destroyParticle(index);
}