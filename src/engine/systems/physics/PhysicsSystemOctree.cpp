//
// Created by Maksym Maisak on 2019-04-04.
//

#include "PhysicsSystemOctree.h"
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
#include "Camera.h"

using namespace en;

namespace {

    const float FIELD_HALF_SIDE = 50.f;
}

PhysicsSystemOctree::PhysicsSystemOctree() :
    m_volumeRenderer(32768 * 4),
    m_octreeRoot({0.f, FIELD_HALF_SIDE, 0.f}, glm::vec3(FIELD_HALF_SIDE))
{
    m_octreeRoot.add(1, {{30.f, 30.f, 30.f}, {32.f, 32.f, 32.f}});
}

void PhysicsSystemOctree::update(float dt) {
    System::update(dt);
}

void PhysicsSystemOctree::removeInvalidEntitiesFromTree() {

}

void PhysicsSystemOctree::updateGridCells(Entity entity, const Rigidbody& rb, const Transform& tf) {

}

std::tuple<bool, float> PhysicsSystemOctree::move(Entity entity, Transform& tf, Rigidbody& rb, float dt) {

    const glm::vec3 movement = rb.velocity * dt;

    if (rb.collider) {

        //const utils::BoundingSphere sphereA = rb.collider->getBoundingSphere();

        //const auto num = (max - min) + vec3Index(1);
        //std::cout << num.x * num.y * num.z << " cells\n";

        //std::cout << m_grid[indexOffset + z].size() << " entities in cell\n";

        for (Entity other : m_registry->with<Rigidbody, Transform>()) {

            if (entity == other)
                continue;

            auto& otherRb = m_registry->get<Rigidbody>(other);
            if (!otherRb.collider)
                continue;

//          const utils::BoundingSphere sphereB = otherRb.collider->getBoundingSphere();
//          if (glm::distance2(sphereA.position, sphereB.position) > glm::length2(sphereA.radius + sphereB.radius))
//              continue;

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

void PhysicsSystemOctree::draw() {

    Entity entity = m_registry->with<Transform, Camera>().tryGetOne();
    if (!entity)
        return;

    std::function<void(const OctreeNode&, const glm::vec3&, const glm::vec3&)> add;
    add = [&](const OctreeNode& node, const glm::vec3& center, const glm::vec3& halfSize) {

        m_volumeRenderer.addAABB(center, halfSize, node.getTotalNumEntities());

        const glm::vec3 childHalfSize = halfSize * 0.5f;
        const std::array<std::unique_ptr<OctreeNode>, 8>& children = node.getChildren();
        for (int i = 0; i < 8; ++i) {

            if (!children[i])
                continue;

            const glm::vec3 offset = {
                (i & 1) ? childHalfSize.x : -childHalfSize.x,
                (i & 2) ? childHalfSize.y : -childHalfSize.y,
                (i & 4) ? childHalfSize.z : -childHalfSize.z
            };

            add(*children[i], center + offset, childHalfSize);
        }
    };
    add(m_octreeRoot, {0, 50, 0}, glm::vec3(50.f));

    const glm::mat4 matrixView = glm::inverse(m_registry->get<Transform>(entity).getWorldTransform());
    const glm::mat4 matrixProjection = m_registry->get<Camera>(entity).getCameraProjectionMatrix(*m_engine);
    m_volumeRenderer.render(matrixProjection * matrixView);
}