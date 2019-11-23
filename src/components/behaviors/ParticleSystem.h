//
// Created by Maksym Maisak on 14/10/18.
//

#ifndef ENGINE_PARTICLESYSTEM_H
#define ENGINE_PARTICLESYSTEM_H

#include <chrono>
#include "Engine.h"
#include "Behavior.h"
#include "glm.h"
#include "Transform.h"
#include "GameTime.h"

using namespace en;
using namespace std::chrono_literals;

namespace en {

    class ParticleSystem : public Behavior {

        using particleIndex_t = std::size_t;
        static constexpr particleIndex_t DEFAULT_MAX_PARTICLES = 1024 * 8;

    public:
        struct Settings {

            float emissionRadius = 128.f;
            Duration emissionInterval = 1ms;
            Duration particleLifetime = 1s;
            glm::vec3 startVelocity = {0, 100, 0};
            float startVelocityRandomness = 10.f;
        };

        explicit ParticleSystem(Actor actor, particleIndex_t maxNumParticles = DEFAULT_MAX_PARTICLES);

        void draw() override;
        void update(float dt) override;

        const Settings& getSettings() const;
        void setSettings(const Settings& settings);

        bool getIsEmissionActive() const;
        void setIsEmissionActive(bool isEmissionActive);

    private:
        struct Particle {
            glm::mat4 transformMatrix;
            TimePoint  timeToDestroy;
            glm::vec3 velocity;
        };

        std::vector<Particle> m_particles;
        particleIndex_t m_numActiveParticles;
        particleIndex_t m_maxNumParticles;

        bool m_isEmissionActive = true;
        TimePoint m_timeOfLastEmission;
        Settings m_settings = {};

        particleIndex_t emitParticle();
        void updateParticle(particleIndex_t particleIndex, float dt);
        void destroyParticle(particleIndex_t particleIndex);

        void destroyOldestParticle();
    };
}

#endif //ENGINE_PARTICLESYSTEM_H
