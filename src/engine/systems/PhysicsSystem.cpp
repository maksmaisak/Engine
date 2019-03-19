//
// Created by Maksym Maisak on 20/10/18.
//

#include <SFML/Graphics.hpp>
#include "PhysicsSystem.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Hit.h"
#include "Messaging.h"
#include "Collision.h"

using namespace en;

PhysicsSystem& PhysicsSystem::setGravity(const glm::vec3& gravity) {

    m_gravity = gravity;
    return *this;
}

void PhysicsSystem::update(float dt) {

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

    for (Collision& collision : m_detectedCollisions)
        Receiver<Collision>::broadcast(collision);
    m_detectedCollisions.clear();
}

namespace {

    /// A helper for resolving collisions between physical bodies
    /// in a way which obeys conservation of momentum.
    /// Assumes `normal` is normalized.
    inline void resolveCollision(
        glm::vec3& aVelocity, float aInverseMass,
        glm::vec3& bVelocity, float bInverseMass,
        glm::vec3 normal, float bounciness = 1.f
    ) {
        float aSpeedAlongNormal = glm::dot(normal, aVelocity);
        float bSpeedAlongNormal = glm::dot(normal, bVelocity);

        if (aSpeedAlongNormal - bSpeedAlongNormal > 0.f)
            return;

        float u =
            (aSpeedAlongNormal * bInverseMass + bSpeedAlongNormal * aInverseMass) /
            (aInverseMass + bInverseMass);

        float aDeltaSpeedAlongNormal = -(1.f + bounciness) * (aSpeedAlongNormal - u);
        aVelocity += normal * aDeltaSpeedAlongNormal;

        float bDeltaSpeedAlongNormal = -(1.f + bounciness) * (bSpeedAlongNormal - u);
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

            std::optional<Hit> optionalHit = rb.collider->collide(*otherRb.collider, movement);
            if (!optionalHit)
                continue;
            const Hit& hit = *optionalHit;

            float otherInvMass = otherRb.isKinematic ? 0.f : otherRb.invMass;
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
