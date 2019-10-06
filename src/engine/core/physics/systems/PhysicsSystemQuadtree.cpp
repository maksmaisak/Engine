//
// Created by Maksym Maisak on 2019-04-21.
//

#include "PhysicsSystemQuadtree.h"
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
#include "Name.h"

#include "UIRect.h"
#include "Camera.h"

using namespace en;

namespace {
    const float FIELD_HALF_SIDE = 55.f;
}

PhysicsSystemQuadtree::PhysicsSystemQuadtree() :
    m_volumeRenderer(32768 * 4),
    m_quadtreeRoot({0.f, 0.f}, glm::vec2(FIELD_HALF_SIDE), 4)
{}

void PhysicsSystemQuadtree::update(float dt) {

    using clock = std::chrono::high_resolution_clock;
    const auto start = clock::now();

    removeInvalidEntitiesFromTree();

    auto entities = m_registry->with<Transform, Rigidbody>();

    // Update the quadtree
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

std::tuple<bool, float> PhysicsSystemQuadtree::move(Entity entity, Transform& tf, Rigidbody& rb, float dt) {

    const glm::vec3 movement = rb.velocity * dt;

    if (!rb.collider) {
        tf.move(movement);
        return {false, 0.f};
    }

    utils::Bounds2D bounds = m_quadtreeRoot.getBounds().clamp(rb.collider->getBounds());
    bounds.expandByMovement({movement.x, movement.z});

    std::stack<QuadtreeNode*> nodes;
    nodes.push(&m_quadtreeRoot);

    while (!nodes.empty()) {

        QuadtreeNode* node = nodes.top();
        nodes.pop();

        if (!node->isLeafNode()) {

            for (const std::unique_ptr<QuadtreeNode>& childNodePtr : node->getChildren())
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

void PhysicsSystemQuadtree::removeInvalidEntitiesFromTree() {

    const auto isInvalidEntity = [this](Entity e, const utils::Bounds& bounds) -> bool {
        // TODO bool registry.has<Types...>()
        return !m_registry->tryGet<Rigidbody>(e) || !m_registry->tryGet<Transform>(e);
    };

    m_quadtreeRoot.removeIf(isInvalidEntity);
}

void PhysicsSystemQuadtree::updateTree(Entity entity, const Rigidbody& rb, const Transform& tf) {

    const utils::Bounds2D bounds = m_quadtreeRoot.getBounds().clamp(rb.collider->getBounds());
    const utils::Bounds2D* oldBounds = m_previousBounds.tryGet(entity);
    if (!oldBounds)
        m_quadtreeRoot.add(entity, bounds);
    else
        m_quadtreeRoot.update(entity, *oldBounds, bounds);

    // TODO ComponentPool::set(args...) (aka getOrAdd)
    if (!m_previousBounds.contains(entity))
        m_previousBounds.insert(entity, bounds);
    else
        m_previousBounds.get(entity) = bounds;
}

void PhysicsSystemQuadtree::draw() {

    Entity entity = m_registry->with<Transform, Camera>().tryGetOne();
    if (!entity)
        return;

    std::function<void(const QuadtreeNode&)> addToVolumeRenderer;
    addToVolumeRenderer = [&](const QuadtreeNode& node) {

        if (!node.isLeafNode()) {

            const auto& children = node.getChildren();
            for (const auto& childPtr : node.getChildren())
                if (childPtr)
                    addToVolumeRenderer(*childPtr);

            return;
        }

        const std::size_t numEntities = node.getEntities().size();
        if (numEntities == 0)
            return;

        const utils::Bounds2D& bounds = node.getBounds();
        const glm::vec2& center = (bounds.min + bounds.max) * 0.5f;
        const glm::vec2& halfSize = bounds.max - center;

        const float t = glm::saturate<float, glm::defaultp>((numEntities - 1.f) / node.getCapacity());
        const glm::vec4 color = glm::mix(glm::vec4(1, 1, 1, 0.8f), glm::vec4(1, 0, 0, 1), glm::vec4(t));

        m_volumeRenderer.addAABB({center.x, 0.f, center.y}, {halfSize.x, 1.f, halfSize.y}, color);
    };
    addToVolumeRenderer(m_quadtreeRoot);

    const glm::mat4 matrixView = glm::inverse(m_registry->get<Transform>(entity).getWorldTransform());
    const glm::mat4 matrixProjection = m_registry->get<Camera>(entity).getCameraProjectionMatrix(*m_engine);
    m_volumeRenderer.render(matrixProjection * matrixView);
}
