//
// Created by Maksym Maisak on 20/10/18.
//

#include <SFML/Graphics.hpp>
#include "PhysicsSystem.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "PhysicsUtils.h"
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
        if (rb.isKinematic) continue;

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

    for (Collision& collision : m_detectedCollisions) {
        Receiver<Collision>::broadcast(collision);
    }
    m_detectedCollisions.clear();
}

std::tuple<bool, float> PhysicsSystem::move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities) {

    glm::vec3 movement = rb.velocity * dt;

    for (Entity other : entities) {

        if (entity == other) continue;

        auto& otherRb = m_registry->get<Rigidbody>(other);
        auto& otherTf = m_registry->get<Transform>(other);

        std::optional<Hit> hit = en::sphereVsSphereContinuous(
            tf.getWorldPosition(), rb.radius, movement,
            otherTf.getWorldPosition(), otherRb.radius
        );

        if (hit.has_value()) {

            float otherInvMass = otherRb.isKinematic ? 0.f : otherRb.invMass;

            en::resolve(
                rb.velocity,      rb.invMass,
                otherRb.velocity, otherInvMass,
                hit->normal, std::min(rb.bounciness, otherRb.bounciness)
            );

            if (otherRb.isKinematic) {
                otherRb.velocity = glm::vec3(0);
            }

            tf.move(movement * hit->timeOfImpact);

            m_detectedCollisions.emplace_back(*hit, entity, other);
            return {true, dt * (1.f - hit->timeOfImpact)};
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
