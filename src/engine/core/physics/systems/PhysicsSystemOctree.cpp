//
// Created by Maksym Maisak on 2019-04-04.
//

#include "PhysicsSystemOctree.h"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <locale>
#include <stack>
#include "Transform.h"
#include "Rigidbody.h"
#include "Hit.h"
#include "Messaging.h"
#include "Collision.h"
#include "UIRect.h"
#include "Camera.h"

using namespace en;

namespace {

    const float CENTER_Y = 50.f;
    const float FIELD_HALF_SIDE = 55.f;
}

PhysicsSystemOctree::PhysicsSystemOctree() :
    m_volumeRenderer(32768 * 4),
    m_octreeRoot({0.f, CENTER_Y, 0.f}, glm::vec3(FIELD_HALF_SIDE), 4)
{}

void PhysicsSystemOctree::update(float dt) {

    using clock = std::chrono::high_resolution_clock;
    const auto start = clock::now();

    removeInvalidEntitiesFromTree();

    auto entities = m_registry->with<Transform, Rigidbody>();

    // Update the octree
    for (Entity entity : entities) {

        auto& rb = m_registry->get<Rigidbody>(entity);
        if (!rb.collider)
            continue;

        if (rb.isStatic && m_previousBounds.contains(entity))
            continue;

        const auto& tf = m_registry->get<Transform>(entity);
        rb.collider->updateTransform(tf.getWorldTransform());

        updateTree(entity, rb, tf);
    }

    // Move bodies
    for (Entity entity : entities) {

        auto& rb = m_registry->get<Rigidbody>(entity);
        if (rb.isStatic)
            continue;

        auto& tf = m_registry->get<Transform>(entity);
        if (rb.useGravity)
            addGravity(entity, tf, rb, dt);

        constexpr int maxNumSteps = 3;
        float timeToMove = dt;
        for (int i = 0; i < maxNumSteps; ++i) {

            bool didCollide;
            std::tie(didCollide, timeToMove) = move(entity, tf, rb, timeToMove);
            if (didCollide) {
                updateTree(entity, rb, tf);
                continue;
            }

            break;
        }
    }

    updateCurrentUpdateInfo(clock::now() - start);

    for (const Collision& collision : m_detectedCollisions)
        Receiver<Collision>::broadcast(collision);
    m_detectedCollisions.clear();
}

std::tuple<bool, float> PhysicsSystemOctree::move(Entity entity, Transform& tf, Rigidbody& rb, float dt) {

    const glm::vec3 movement = rb.velocity * dt;

    if (!rb.collider) {
        tf.move(movement);
        return {false, 0.f};
    }

    utils::Bounds bounds = m_octreeRoot.getBounds().clamp(rb.collider->getBounds());
    bounds.expandByMovement(movement);

    std::stack<OctreeNode*> nodes;
    nodes.push(&m_octreeRoot);

    while (!nodes.empty()) {

        OctreeNode* node = nodes.top();
        nodes.pop();

        if (!node->isLeafNode()) {

            for (const std::unique_ptr<OctreeNode>& childNodePtr : node->getChildren())
                if (childNodePtr)
                    if (bounds.intersects(childNodePtr->getBounds()))
                        nodes.push(childNodePtr.get());
        } else {

            for (const auto& [other, otherBounds] : node->getEntities()) {

                if (entity == other)
                    continue;

                auto& otherRb = m_registry->get<Rigidbody>(other);
                if (!otherRb.collider)
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
    }

    tf.move(movement);
    return {false, 0.f};
}

void PhysicsSystemOctree::removeInvalidEntitiesFromTree() {

    const auto isInvalidEntity = [this](Entity e, const utils::Bounds& bounds) -> bool {
        // TODO bool registry.has<Types...>()
        return !m_registry->tryGet<Rigidbody>(e) || !m_registry->tryGet<Transform>(e);
    };

    m_octreeRoot.removeIf(isInvalidEntity);
}

void PhysicsSystemOctree::updateTree(Entity entity, const Rigidbody& rb, const Transform& tf) {

    const utils::Bounds bounds = m_octreeRoot.getBounds().clamp(rb.collider->getBounds());
    const utils::Bounds* oldBounds = m_previousBounds.tryGet(entity);
    if (!oldBounds)
        m_octreeRoot.add(entity, bounds);
    else
        m_octreeRoot.update(entity, *oldBounds, bounds);

    // TODO ComponentPool::set(args...) (aka getOrAdd)
    if (!m_previousBounds.contains(entity))
        m_previousBounds.insert(entity, bounds);
    else
        m_previousBounds.get(entity) = bounds;
}

void PhysicsSystemOctree::draw() {

    Entity entity = m_registry->with<Transform, Camera>().tryGetOne();
    if (!entity)
        return;

    std::function<void(const OctreeNode&)> addToVolumeRenderer;
    addToVolumeRenderer = [&](const OctreeNode& node) {

        if (!node.isLeafNode()) {

            const auto& children = node.getChildren();
            for (int i = 0; i < 8; ++i)
                if (children[i])
                    addToVolumeRenderer(*children[i]);

            return;
        }

        const std::size_t numEntities = node.getEntities().size();
        if (numEntities == 0)
            return;

        const utils::Bounds& bounds = node.getBounds();
        const glm::vec3& center = (bounds.min + bounds.max) * 0.5f;
        const glm::vec3& halfSize = bounds.max - center;

        const float t = glm::saturate<float, glm::defaultp>((numEntities - 1.f) / node.getCapacity());
        const glm::vec4 color = glm::mix(glm::vec4(1, 1, 1, 0.8f), glm::vec4(1, 0, 0, 1), glm::vec4(t));

        m_volumeRenderer.addAABB(center, halfSize, color);
    };
    addToVolumeRenderer(m_octreeRoot);

    const glm::mat4 matrixView = glm::inverse(m_registry->get<Transform>(entity).getWorldTransform());
    const glm::mat4 matrixProjection = m_registry->get<Camera>(entity).getCameraProjectionMatrix(*m_engine);
    m_volumeRenderer.render(matrixProjection * matrixView);
}