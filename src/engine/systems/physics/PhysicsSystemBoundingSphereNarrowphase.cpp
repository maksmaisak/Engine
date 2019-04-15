//
// Created by Maksym Maisak on 2019-04-01.
//

#include "PhysicsSystemBoundingSphereNarrowphase.h"
#include <SFML/Graphics.hpp>
#include <utility>
#include "Transform.h"
#include "Rigidbody.h"
#include "Hit.h"
#include "Messaging.h"
#include "Collision.h"
#include "Name.h"
#include "UIRect.h"
#include "Text.h"

using namespace en;

void PhysicsSystemBoundingSphereNarrowphase::update(float dt) {

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

        constexpr int maxNumSteps = 3;
        float timeToMove = dt;
        for (int i = 0; i < maxNumSteps; ++i) {

            bool didCollide;
            std::tie(didCollide, timeToMove) = move(entity, tf, rb, timeToMove, entities);
            if (didCollide)
                continue;

            break;
        }
    }

    updateCurrentUpdateInfo(clock::now() - start);

    for (Collision& collision : m_detectedCollisions)
        Receiver<Collision>::broadcast(collision);
    m_detectedCollisions.clear();
}

std::tuple<bool, float> PhysicsSystemBoundingSphereNarrowphase::move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities) {

    const glm::vec3 movement = rb.velocity * dt;

    if (rb.collider) {

        utils::BoundingSphere sphereA = rb.collider->getBoundingSphere();
        sphereA.radius += glm::length(movement);

        for (Entity other : entities) {

            if (entity == other)
                continue;

            auto& otherRb = m_registry->get<Rigidbody>(other);
            if (!otherRb.collider)
                continue;

            const utils::BoundingSphere sphereB = otherRb.collider->getBoundingSphere();
            if (glm::distance2(sphereA.position, sphereB.position) > glm::length2(sphereA.radius + sphereB.radius))
                continue;

            m_currentUpdateInfo.numCollisionChecks += 1;
            std::optional<Hit> optionalHit = rb.collider->collide(*otherRb.collider, movement);
            if (!optionalHit)
                continue;

            m_currentUpdateInfo.numCollisions += 1;
            const Hit& hit = *optionalHit;
            resolve(*optionalHit, tf, rb, otherRb, movement);
            m_detectedCollisions.emplace_back(hit, entity, other);
            return {true, dt * (1.f - hit.timeOfImpact)};
        }
    }

    tf.move(movement);
    return {false, 0.f};
}
