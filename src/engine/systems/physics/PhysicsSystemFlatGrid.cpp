//
// Created by Maksym Maisak on 2019-04-01.
//

#include "PhysicsSystemFlatGrid.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <chrono>
#include <locale>
#include "Transform.h"
#include "Rigidbody.h"
#include "Hit.h"
#include "Messaging.h"
#include "Collision.h"
#include "Name.h"
#include "UIRect.h"
#include "Text.h"

using namespace en;

void PhysicsSystemFlatGrid::update(float dt) {

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

    const auto time = clock::now() - start;
    m_currentUpdateInfo.time = time;
    m_diagnosticsInfo.updateTimeAverage.addSample(time);
    m_diagnosticsInfo.updateTimeMin = std::min(m_diagnosticsInfo.updateTimeMin, time);
    m_diagnosticsInfo.updateTimeMax = std::max(m_diagnosticsInfo.updateTimeMax, time);

    for (Collision& collision : m_detectedCollisions)
        Receiver<Collision>::broadcast(collision);
    m_detectedCollisions.clear();

    flushCurrentUpdateInfo();
}
