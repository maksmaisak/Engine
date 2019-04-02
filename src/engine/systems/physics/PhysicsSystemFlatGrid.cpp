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
#include "Camera.h"

using namespace en;

namespace {

    const float GRID_CELL_SIZE = 20.f;
    const std::size_t NUM_GRID_CELLS = 10;

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

PhysicsSystemFlatGrid::PhysicsSystemFlatGrid() :
    m_grid(NUM_GRID_CELLS * NUM_GRID_CELLS * NUM_GRID_CELLS),
    m_wireframeShader(Resources<ShaderProgram>::get("wireframe"))
{}

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

void PhysicsSystemFlatGrid::draw() {

    static const std::array<Vertex, 36> cubeVertices {
        Vertex {{-1.0f,  1.0f, -1.0f}},
        Vertex {{ 1.0f, -1.0f, -1.0f}},
        Vertex {{-1.0f, -1.0f, -1.0f}},

        Vertex {{ 1.0f, -1.0f, -1.0f}},
        Vertex {{-1.0f,  1.0f, -1.0f}},
        Vertex {{ 1.0f,  1.0f, -1.0f}},

        Vertex {{-1.0f, -1.0f,  1.0f}},
        Vertex {{-1.0f,  1.0f, -1.0f}},
        Vertex {{-1.0f, -1.0f, -1.0f}},

        Vertex {{-1.0f,  1.0f, -1.0f}},
        Vertex {{-1.0f, -1.0f,  1.0f}},
        Vertex {{-1.0f,  1.0f,  1.0f}},

        Vertex {{ 1.0f, -1.0f, -1.0f}},
        Vertex {{ 1.0f,  1.0f,  1.0f}},
        Vertex {{ 1.0f, -1.0f,  1.0f}},

        Vertex {{ 1.0f,  1.0f,  1.0f}},
        Vertex {{ 1.0f, -1.0f, -1.0f}},
        Vertex {{ 1.0f,  1.0f, -1.0f}},

        Vertex {{-1.0f, -1.0f,  1.0f}},
        Vertex {{ 1.0f,  1.0f,  1.0f}},
        Vertex {{-1.0f,  1.0f,  1.0f}},

        Vertex {{ 1.0f,  1.0f,  1.0f}},
        Vertex {{-1.0f, -1.0f,  1.0f}},
        Vertex {{ 1.0f, -1.0f,  1.0f}},

        Vertex {{-1.0f,  1.0f, -1.0f}},
        Vertex {{ 1.0f,  1.0f,  1.0f}},
        Vertex {{ 1.0f,  1.0f, -1.0f}},

        Vertex {{ 1.0f,  1.0f,  1.0f}},
        Vertex {{-1.0f,  1.0f, -1.0f}},
        Vertex {{-1.0f,  1.0f,  1.0f}},

        Vertex {{-1.0f, -1.0f, -1.0f}},
        Vertex {{ 1.0f, -1.0f, -1.0f}},
        Vertex {{-1.0f, -1.0f,  1.0f}},

        Vertex {{ 1.0f, -1.0f, -1.0f}},
        Vertex {{ 1.0f, -1.0f,  1.0f}},
        Vertex {{-1.0f, -1.0f,  1.0f}}
    };

    Entity entity = m_registry->with<Transform, Camera>().tryGetOne();
    if (!entity)
        return;

    std::vector<Vertex> cellVertices(36, Vertex{});

    m_wireframeShader->use();
    m_wireframeShader->setUniformValue("matrixProjection", m_registry->get<Camera>(entity).getCameraProjectionMatrix(*m_engine) * glm::inverse(m_registry->get<Transform>(entity).getWorldTransform()));
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Add to new grid cells
    for (std::size_t x = 0; x < NUM_GRID_CELLS; ++x) {

        const std::size_t xIndexOffset = x * NUM_GRID_CELLS * NUM_GRID_CELLS;
        for (std::size_t y = 0; y < NUM_GRID_CELLS; ++y) {

            const std::size_t indexOffset = xIndexOffset + y * NUM_GRID_CELLS;
            for (std::size_t z = 0; z < NUM_GRID_CELLS; ++z) {

                const std::size_t numEntities = m_grid[indexOffset + z].size();
                if (numEntities == 0)
                    continue;

                const glm::vec4 color = glm::lerp(glm::vec4(0, 1, 0, 1), glm::vec4(1, 0, 0, 1), glm::vec4(glm::saturate<float, glm::defaultp>((numEntities - 1.f) / 10.f)));
                //const glm::vec4 color = {1,1,0,1};
                m_wireframeShader->setUniformValue("color", color);

                const glm::mat4 matrix =
                    glm::translate((glm::vec3(x, y, z) - glm::floor(glm::vec3(NUM_GRID_CELLS) * 0.5f) + 0.5f) * GRID_CELL_SIZE) *
                    glm::scale(glm::vec3(GRID_CELL_SIZE));

                std::transform(cubeVertices.begin(), cubeVertices.end(), cellVertices.begin(), [&matrix](const Vertex& vertex) -> Vertex {
                    return {matrix * glm::vec4(vertex.position * 0.5f, 1.f), vertex.uv};
                });

                m_vertexRenderer.renderVertices(cellVertices);
            }
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
