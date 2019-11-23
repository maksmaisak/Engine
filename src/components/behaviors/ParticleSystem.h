//
// Created by Maksym Maisak on 14/10/18.
//

#ifndef SAXION_Y2Q1_CPP_PARTICLESYSTEM_H
#define SAXION_Y2Q1_CPP_PARTICLESYSTEM_H

#include "Engine.h"
#include "Behavior.h"
#include "glm.h"
#include "Transform.h"
#include <SFML/System.hpp>

using namespace en;

class ParticleSystem : public Behavior {

    using ParticleIndex = std::size_t;
    static const ParticleIndex DEFAULT_MAX_PARTICLES = 1024 * 8;

public:
    struct Settings {

        float emissionRadius = 128.f;
        sf::Time emissionInterval = sf::milliseconds(1);
        sf::Time particleLifetime = sf::milliseconds(1000);
        glm::vec3 startVelocity = {0, 100, 0};
        float startVelocityRandomness = 10.f;
    };

    explicit ParticleSystem(Actor actor, ParticleIndex maxNumParticles = DEFAULT_MAX_PARTICLES) :
        Behavior(actor),
        m_maxNumParticles(maxNumParticles),
        m_particles(maxNumParticles) {}

    void draw() override;
    void update(float dt) override;

    const Settings& getSettings() const;
    void setSettings(const Settings& settings);

    const bool getIsEmissionActive() const;
    void setIsEmissionActive(bool isEmissionActive);

private:
    struct Particle {
        glm::mat4 transformMatrix;
        sf::Time  timeToDestroy;
        glm::vec3 velocity;
    };

    std::vector<Particle> m_particles;
    ParticleIndex m_numActiveParticles = 0;
    ParticleIndex m_maxNumParticles;

    bool m_isEmissionActive = true;
    sf::Clock m_emissionTimer;
    Settings m_settings = {};

    ParticleIndex emitParticle();
    void updateParticle(ParticleIndex particleIndex, float dt);
    void destroyParticle(ParticleIndex particleIndex);

    void destroyOldestParticle();
};

#endif //SAXION_Y2Q1_CPP_PARTICLESYSTEM_H
