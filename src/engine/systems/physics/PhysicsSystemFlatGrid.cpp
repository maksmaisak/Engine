//
// Created by Maksym Maisak on 2019-04-01.
//

#include "PhysicsSystemFlatGrid.h"
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

namespace {

    const float GRID_CELL_SIZE = 2.f;
    const std::size_t NUM_GRID_CELLS = 100;

    using vec3Index = glm::vec<3, std::size_t>;

    inline std::pair<vec3Index, vec3Index> getBoundsOnGrid(const utils::Bounds& bounds) {

        const auto size = glm::vec3(GRID_CELL_SIZE);
        const auto minIndex = vec3Index(0);
        const auto maxIndex = vec3Index(NUM_GRID_CELLS - 1);
        const glm::vec3 halfMaxIndex = glm::ceil(glm::vec3(maxIndex) * 0.5f);

        return {
            glm::clamp(vec3Index(glm::floor(bounds.min / size + halfMaxIndex)), minIndex, maxIndex),
            glm::clamp(vec3Index(glm::floor(bounds.max / size + halfMaxIndex)), minIndex, maxIndex),
        };
    }
}

PhysicsSystemFlatGrid::PhysicsSystemFlatGrid() : m_grid(NUM_GRID_CELLS * NUM_GRID_CELLS * NUM_GRID_CELLS) {}

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

        // TODO handle a case where the rigidbody component is removed
        updateGrid(entity, rb, tf);
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
            std::tie(didCollide, timeToMove) = move(entity, tf, rb, timeToMove);
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

std::tuple<bool, float> PhysicsSystemFlatGrid::move(Entity entity, Transform& tf, Rigidbody& rb, float dt) {

    const glm::vec3 movement = rb.velocity * dt;

    if (rb.collider) {

        //const utils::BoundingSphere sphereA = rb.collider->getBoundingSphere();
        auto [min, max] = getBoundsOnGrid(rb.collider->getBounds());
        min = glm::max(min - vec3Index(1), vec3Index(0));
        max = glm::min(max + vec3Index(1), vec3Index(NUM_GRID_CELLS - 1));

        const auto num = (max - min) + vec3Index(1);
        //std::cout << num.x * num.y * num.z << " cells\n";

        for (std::size_t x = min.x; x <= max.x; ++x) {
            const std::size_t xIndexOffset = x * NUM_GRID_CELLS * NUM_GRID_CELLS;
            for (std::size_t y = min.y; y <= max.y; ++y) {
                const std::size_t indexOffset = xIndexOffset + y * NUM_GRID_CELLS;
                for (std::size_t z = min.z; z <= max.z; ++z) {

                    //std::cout << m_grid[indexOffset + z].size() << " entities in cell\n";

                    for (Entity other : m_grid[indexOffset + z]) {

                        if (entity == other)
                            continue;

                        // TODO Handle entities being destroyed
                        auto* otherRb = m_registry->tryGet<Rigidbody>(other);
                        if (!otherRb || !otherRb->collider)
                            continue;

            //            const utils::BoundingSphere sphereB = otherRb.collider->getBoundingSphere();
            //            if (glm::distance2(sphereA.position, sphereB.position) > glm::length2(sphereA.radius + sphereB.radius))
            //                continue;

                        m_currentUpdateInfo.numCollisionChecks += 1;
                        std::optional<Hit> optionalHit = rb.collider->collide(*otherRb->collider, movement);
                        if (!optionalHit)
                            continue;

                        m_currentUpdateInfo.numCollisions += 1;
                        const Hit& hit = *optionalHit;
                        resolve(*optionalHit, tf, rb, *otherRb, movement);
                        m_detectedCollisions.emplace_back(hit, entity, other);
                        return {true, dt * (1.f - hit.timeOfImpact)};
                    }
                }
            }
        }
    }

    tf.move(movement);
    return {false, 0.f};
}

void PhysicsSystemFlatGrid::updateGrid(Entity entity, const Rigidbody& rb, const Transform& tf) {

    const auto [min, max] = getBoundsOnGrid(rb.collider->getBounds());

    // Remove from old grid cells
    if (auto* oldBounds = m_previousBounds.tryGet(entity)) {

        const auto [oldMin, oldMax] = *oldBounds;

        for (std::size_t x = oldMin.x; x <= oldMax.x; ++x) {

            if (x >= min.x || x <= max.x)
                continue;

            const std::size_t xIndexOffset = x * NUM_GRID_CELLS * NUM_GRID_CELLS;
            for (std::size_t y = oldMin.y; y <= oldMax.y; ++y) {

                if (y >= min.y || y <= max.y)
                    continue;

                const std::size_t indexOffset = xIndexOffset + y * NUM_GRID_CELLS;
                for (std::size_t z = oldMin.z; z <= oldMax.z; ++z) {

                    if (z >= min.z || z <= max.z)
                        continue;

                    auto& collection = m_grid[indexOffset + z];
                    auto it = std::find(collection.begin(), collection.end(), entity);
                    if (it != collection.end()) {
                        collection.erase(it);
                    }
                }
            }
        }
    }

    // Add to new grid cells
    for (std::size_t x = min.x; x <= max.x; ++x) {

        const std::size_t xIndexOffset = x * NUM_GRID_CELLS * NUM_GRID_CELLS;
        for (std::size_t y = min.y; y <= max.y; ++y) {

            const std::size_t indexOffset = xIndexOffset + y * NUM_GRID_CELLS;
            for (std::size_t z = min.z; z <= max.z; ++z) {

                auto& collection = m_grid[indexOffset + z];
                if (std::find(collection.begin(), collection.end(), entity) == collection.end()) {
                    collection.push_back(entity);
                }
            }
        }
    }

    // TODO ComponentPool::set(args...) (aka getOrAdd)
    if (!m_previousBounds.contains(entity))
        m_previousBounds.insert(entity, min, max);
    else
        m_previousBounds.get(entity) = {min, max};
}
