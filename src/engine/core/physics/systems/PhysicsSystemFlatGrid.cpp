//
// Created by Maksym Maisak on 2019-04-01.
//

#include "PhysicsSystemFlatGrid.h"
#include <sstream>
#include <chrono>
#include "Transform.h"
#include "Rigidbody.h"
#include "Hit.h"
#include "Messaging.h"
#include "Collision.h"
#include "Camera.h"

using namespace en;

namespace {

    const float GRID_CELL_SIZE = 10.f;
    const std::size_t NUM_GRID_CELLS = 20;

    using vec3Int = glm::vec<3, std::int64_t>;

    inline std::pair<vec3Int, vec3Int> getBoundsOnGrid(const utils::Bounds3D& bounds) {

        const auto size = glm::vec3(GRID_CELL_SIZE);
        const auto minIndex = glm::vec3(0);
        const auto maxIndex = glm::vec3(NUM_GRID_CELLS - 1);
        const glm::vec3 halfGridSize = glm::vec3(GRID_CELL_SIZE * NUM_GRID_CELLS * 0.5f);
        const glm::vec3 halfMaxIndex = maxIndex * 0.5f;

        static const auto toGrid = [](const glm::vec3& pos){

            const glm::vec3 a = pos / glm::vec3(GRID_CELL_SIZE);
            const glm::vec3 b = glm::floor(a);
            const vec3Int c = b + glm::vec3(NUM_GRID_CELLS * 0.5f);
            const vec3Int d = glm::clamp(c, vec3Int(0), vec3Int(NUM_GRID_CELLS - 1));

            return d;
        };

        return {
            toGrid(bounds.min),
            toGrid(bounds.max)
        };
    }
}

PhysicsSystemFlatGrid::PhysicsSystemFlatGrid() :
    m_grid(NUM_GRID_CELLS * NUM_GRID_CELLS * NUM_GRID_CELLS),
    m_volumeRenderer(32768 * 4)
{}

void PhysicsSystemFlatGrid::update(float dt) {

    using clock = std::chrono::high_resolution_clock;
    const auto start = clock::now();

    removeInvalidEntitiesFromGrid();

    auto entities = m_registry->with<Transform, Rigidbody>();

    for (Entity entity : entities) {

        auto& rb = m_registry->get<Rigidbody>(entity);
        if (!rb.collider)
            continue;

        if (rb.isStatic && m_previousBounds.contains(entity))
            continue;

        const auto& tf = m_registry->get<Transform>(entity);
        rb.collider->updateTransform(tf.getWorldTransform());

        updateGridCells(entity, rb, tf);
    }

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
                updateGridCells(entity, rb, tf);
                continue;
            }

            break;
        }
    }

    updateCurrentUpdateInfo(clock::now() - start);

    for (Collision& collision : m_detectedCollisions)
        Receiver<Collision>::broadcast(collision);
    m_detectedCollisions.clear();
}

std::tuple<bool, float> PhysicsSystemFlatGrid::move(Entity entity, Transform& tf, Rigidbody& rb, float dt) {

    const glm::vec3 movement = rb.velocity * dt;

    if (rb.collider) {

        //const utils::BoundingSphere sphereA = rb.collider->getBoundingSphere();
        utils::Bounds3D bounds = rb.collider->getBounds();
        bounds.expandByMovement(movement);
        auto [min, max] = getBoundsOnGrid(bounds);
        min = glm::max(vec3Int(min) - vec3Int(1), vec3Int(0));
        max = glm::min(vec3Int(max) + vec3Int(1), vec3Int(NUM_GRID_CELLS - 1));

        //const auto num = (max - min) + vec3Index(1);
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

                        auto& otherRb = m_registry->get<Rigidbody>(other);
                        if (!otherRb.collider)
                            continue;

//                        const utils::BoundingSphere sphereB = otherRb.collider->getBoundingSphere();
//                        if (glm::distance2(sphereA.position, sphereB.position) > glm::length2(sphereA.radius + sphereB.radius))
//                            continue;

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
        }
    }

    tf.move(movement);
    return {false, 0.f};
}

void PhysicsSystemFlatGrid::removeInvalidEntitiesFromGrid() {

    const auto isInvalidEntity = [this](Entity e) -> bool {
        // TODO bool registry.has<Types...>()
        return !m_registry->tryGet<Rigidbody>(e) || !m_registry->tryGet<Transform>(e);
    };

    for (std::size_t x = 0; x < NUM_GRID_CELLS; ++x) {

        const std::size_t xIndexOffset = x * NUM_GRID_CELLS * NUM_GRID_CELLS;
        for (std::size_t y = 0; y < NUM_GRID_CELLS; ++y) {

            const std::size_t indexOffset = xIndexOffset + y * NUM_GRID_CELLS;
            for (std::size_t z = 0; z < NUM_GRID_CELLS; ++z) {

                std::vector<Entity>& vec = m_grid[indexOffset + z];
                vec.erase(std::remove_if(vec.begin(), vec.end(), isInvalidEntity), vec.end());
            }
        }
    }
}

void PhysicsSystemFlatGrid::updateGridCells(Entity entity, const Rigidbody& rb, const Transform& tf) {

    const auto [min, max] = getBoundsOnGrid(rb.collider->getBounds());

    // Remove from old grid cells
    if (auto* oldBounds = m_previousBounds.tryGet(entity)) {

        const auto [oldMin, oldMax] = *oldBounds;

        if (oldMin != min || oldMax != max) {

            for (std::size_t x = oldMin.x; x <= oldMax.x; ++x) {

                const bool xInsideCurrentBounds = x >= min.x && x <= max.x;
                const std::size_t xIndexOffset = x * NUM_GRID_CELLS * NUM_GRID_CELLS;
                for (std::size_t y = oldMin.y; y <= oldMax.y; ++y) {

                    const bool xyInsideCurrentBounds = xInsideCurrentBounds && y >= min.y && y <= max.y;
                    const std::size_t indexOffset = xIndexOffset + y * NUM_GRID_CELLS;
                    for (std::size_t z = oldMin.z; z <= oldMax.z; ++z) {

                        if (xyInsideCurrentBounds && z >= min.z && z <= max.z)
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

namespace {

    glm::vec4 getGridCellColor(std::size_t numEntities) {

        constexpr float MAX_NUM_ENTITIES_IN_CELL = 10.f;

        const float t = glm::saturate<float, glm::defaultp>((numEntities - 1.f) / MAX_NUM_ENTITIES_IN_CELL);
        return glm::mix(glm::vec4(1, 1, 1, 0.8f), glm::vec4(1, 0, 0, 1), glm::vec4(t));
    }
}

void PhysicsSystemFlatGrid::draw() {

    Entity entity = m_registry->with<Transform, Camera>().tryGetOne();
    if (!entity)
        return;

    // Add to new grid cells
    for (std::size_t x = 0; x < NUM_GRID_CELLS; ++x) {

        const std::size_t xIndexOffset = x * NUM_GRID_CELLS * NUM_GRID_CELLS;
        for (std::size_t y = 0; y < NUM_GRID_CELLS; ++y) {

            const std::size_t indexOffset = xIndexOffset + y * NUM_GRID_CELLS;
            for (std::size_t z = 0; z < NUM_GRID_CELLS; ++z) {

                const std::size_t numEntities = m_grid[indexOffset + z].size();
                if (numEntities == 0)
                    continue;

                const glm::vec3& center = (glm::vec3(x, y, z) - glm::floor(glm::vec3(NUM_GRID_CELLS) * 0.5f) + 0.5f) * GRID_CELL_SIZE;
                const glm::vec4 color = getGridCellColor(numEntities);
                m_volumeRenderer.addAABB(center, glm::vec3(GRID_CELL_SIZE * 0.5f - 0.01f), color);
            }
        }
    }

    const glm::mat4 matrixView = glm::inverse(m_registry->get<Transform>(entity).getWorldTransform());
    const glm::mat4 matrixProjection = m_registry->get<Camera>(entity).getCameraProjectionMatrix(*m_engine);
    m_volumeRenderer.render(matrixProjection * matrixView);
}
