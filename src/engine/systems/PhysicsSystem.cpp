//
// Created by Maksym Maisak on 20/10/18.
//

#include "PhysicsSystem.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <chrono>
#include "Transform.h"
#include "Rigidbody.h"
#include "Hit.h"
#include "Messaging.h"
#include "Collision.h"

#include "UIRect.h"
#include "Text.h"

using namespace en;

PhysicsSystem& PhysicsSystem::setGravity(const glm::vec3& gravity) {

    m_gravity = gravity;
    return *this;
}

void PhysicsSystem::update(float dt) {

    using clock = std::chrono::high_resolution_clock;
    const auto start = clock::now();

    auto entities = m_registry->with<Transform, Rigidbody>();

    for (Entity entity : entities) {

        auto& rb = m_registry->get<Rigidbody>(entity);
        if (!rb.collider)
            continue;

        const auto& tf = m_registry->get<Transform>(entity);
        rb.collider->updateTransform(tf.getWorldTransform());
    }

    for (Entity entity : entities) {

        auto& rb = m_registry->get<Rigidbody>(entity);
        if (rb.isKinematic)
            continue;

        auto& tf = m_registry->get<Transform>(entity);

        if (rb.useGravity)
            addGravity(entity, tf, rb, dt);

        constexpr int maxNumSteps = 10;
        float timeToMove = dt;
        for (int i = 0; i < maxNumSteps; ++i) {

            bool didCollide;
            std::tie(didCollide, timeToMove) = move(entity, tf, rb, timeToMove, entities);
            if (didCollide)
                continue;

            break;
        }
    }

    std::chrono::duration<double, std::milli> time  = clock::now() - start;
    m_diagnosticsInfo.updateTime = clock::now() - start;
    m_averageUpdateTime.addSample(std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start));

    for (Collision& collision : m_detectedCollisions)
        Receiver<Collision>::broadcast(collision);
    m_detectedCollisions.clear();

    flushDiagnosticsInfo();
}

namespace {

    /// A helper for resolving collisions between physical bodies
    /// in a way that obeys conservation of momentum.
    /// Assumes `normal` is normalized.
    inline void resolveCollision(
        glm::vec3& aVelocity, float aInverseMass,
        glm::vec3& bVelocity, float bInverseMass,
        const glm::vec3& normal, float bounciness = 1.f
    ) {
        const float aSpeedAlongNormal = glm::dot(normal, aVelocity);
        const float bSpeedAlongNormal = glm::dot(normal, bVelocity);
        if (aSpeedAlongNormal - bSpeedAlongNormal > 0.f)
            return;

        const float u =
            (aSpeedAlongNormal * bInverseMass + bSpeedAlongNormal * aInverseMass) /
            (aInverseMass + bInverseMass);

        const float aDeltaSpeedAlongNormal = -(1.f + bounciness) * (aSpeedAlongNormal - u);
        aVelocity += normal * aDeltaSpeedAlongNormal;

        const float bDeltaSpeedAlongNormal = -(1.f + bounciness) * (bSpeedAlongNormal - u);
        bVelocity += normal * bDeltaSpeedAlongNormal;
    }
}

std::tuple<bool, float> PhysicsSystem::move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities) {

    const glm::vec3 movement = rb.velocity * dt;

    if (rb.collider) {

        for (Entity other : entities) {

            if (entity == other)
                continue;

            auto& otherRb = m_registry->get<Rigidbody>(other);
            if (!otherRb.collider)
                continue;

            m_diagnosticsInfo.numCollisionChecks += 1;
            std::optional<Hit> optionalHit = rb.collider->collide(*otherRb.collider, movement);
            if (!optionalHit)
                continue;
            m_diagnosticsInfo.numCollisions += 1;
            const Hit& hit = *optionalHit;

            const float otherInvMass = otherRb.isKinematic ? 0.f : otherRb.invMass;
            resolveCollision(
                rb.velocity, rb.invMass,
                otherRb.velocity, otherInvMass,
                hit.normal, std::min(rb.bounciness, otherRb.bounciness)
            );
            if (otherRb.isKinematic)
                otherRb.velocity = glm::vec3(0);

            tf.move(movement * hit.timeOfImpact + hit.depenetrationOffset);
            rb.collider->updateTransform(tf.getWorldTransform());

            m_detectedCollisions.emplace_back(hit, entity, other);
            return {true, dt * (1.f - hit.timeOfImpact)};
        }
    }

    tf.move(movement);
    return {false, 0.f};
}

void PhysicsSystem::addGravity(Entity entity, Transform& tf, Rigidbody& rb, float dt) {

    rb.velocity += m_gravity * dt;

    /*for (Entity other : m_registry->with<Transform, Rigidbody>()) {

        if (entity == other) continue;

        auto& otherRb = m_registry->get<Rigidbody>(other);
        auto& otherTf = m_registry->get<Transform>(other);

        rb.velocity += dt * 1.f / (otherRb.invMass * glm::distance2(tf.getWorldPosition(), otherTf.getWorldPosition()));
    }*/
}

void PhysicsSystem::flushDiagnosticsInfo() {

    using namespace std::literals::string_literals;
    using namespace std::chrono;

    const auto& i = m_diagnosticsInfo;
    std::stringstream s;
    s <<
        "Physics:\n" <<
        "update time: " << i.updateTime.count() << "ms" << std::endl <<
        "update time (average): " << duration_cast<duration<double, std::milli>>(m_averageUpdateTime.get()).count() << "ms" << std::endl <<
        "collision checks: " << i.numCollisionChecks << std::endl <<
        "collisions      : " << i.numCollisions << std::endl;
    //std::cout << s.str();
    ensureDebugText().setString(s.str());

    m_diagnosticsInfo = {};
}

Text& PhysicsSystem::ensureDebugText() {

    if (m_debugTextActor)
        return m_debugTextActor.get<Text>();

    m_debugTextActor = m_engine->makeActor("PhysicsSystemDebug");
    m_debugTextActor.add<Transform>();
    m_debugTextActor.add<UIRect>();
    return m_debugTextActor.add<Text>()
        .setString("Test")
        .setAlignment({0, 1})
        .setFont(Resources<sf::Font>::get(config::FONT_PATH + "Menlo.ttc"));
}
