//
// Created by Maksym Maisak on 29/9/19.
//

#include "Pathfinding.h"
#include <queue>
#include <stack>
#include <unordered_map>
#include "TileLayer.h"
#include "Engine.h"

using namespace ai;

namespace {

    bool isObstacle(en::Engine& engine, const glm::i64vec2& tileCoordinates) {

        const en::EntityRegistry& registry = engine.getRegistry();
        for (en::Entity e : registry.with<en::TileLayer>()) {
            if (registry.get<en::TileLayer>(e).at(tileCoordinates).isObstacle) {
                return true;
            }
        }

        return false;
    }

    const glm::i64vec2 neighborDeltas[] {
        { 1,  0},
        { 1,  1},
        { 0,  1},
        {-1,  1},
        {-1,  0},
        {-1, -1},
        { 0, -1},
        { 1, -1}
    };

    const float sqrt2 = glm::sqrt(2.f);

    using Coords = glm::i64vec2;

    enum NodeState {
        Unvisited = 0,
        Open,
        Closed
    };

    struct NodeInfo {

        Coords previous = {0, 0};
        NodeState state = Unvisited;
        float distance = std::numeric_limits<float>::infinity();
        float totalDistance = std::numeric_limits<float>::infinity();

        friend bool operator<(const NodeInfo& lhs, const NodeInfo& rhs) {
            return lhs.totalDistance < rhs.totalDistance;
        }
    };

    float manhattan(const Coords& a, const Coords& b) {
        return static_cast<float>(glm::abs(a.x - b.x) + glm::abs(a.y - b.y));
    }
}

std::optional<PathfindingPath> Pathfinding::getPath(en::Engine& engine, const Coords& start, const Coords& goal, std::size_t maxNumCheckedTiles) {

    if (start == goal) {
        return {};
    }

    if (isObstacle(engine, start) || isObstacle(engine, goal)) {
        return std::nullopt;
    }

    // TODO chunk-based node storage for performance.
    std::unordered_map<Coords, NodeInfo> nodes;

    const auto reconstructPath = [&nodes, start](const Coords& coords){

        std::deque<Coords> reversePath;

        const NodeInfo* currentNode = &nodes.at(coords);
        while (currentNode->previous != start) {
            reversePath.push_front(currentNode->previous);
            currentNode = &nodes.at(currentNode->previous);
        }

        return PathfindingPath(reversePath);
    };

    std::priority_queue<Coords, std::vector<Coords>, std::function<bool(const Coords&, const Coords&)>> frontier(
        [&nodes](const Coords& lhs, const Coords& rhs){
            return nodes.at(lhs).totalDistance > nodes.at(rhs).totalDistance;
        }
    );

    nodes[start] = {start, Open, 0.f, manhattan(start, goal)};
    frontier.push(start);

    std::size_t numCheckedTiles = 0;
    while (!frontier.empty()) {

        const glm::i64vec2 currentCoords = frontier.top();
        if (currentCoords == goal) {
            return reconstructPath(currentCoords);
        }

        numCheckedTiles += 1;
        if (numCheckedTiles > maxNumCheckedTiles) {
            return std::nullopt;
        }

        frontier.pop();
        NodeInfo& currentNode = nodes.at(currentCoords);
        currentNode.state = Closed;
        assert(nodes.at(currentCoords).state == Closed);

        // Walk through neighbors.
        for (int i = 0; i < 8; ++i) {

            const glm::i64vec2 neighborCoords = currentCoords + neighborDeltas[i];
            NodeInfo& neighborNode = nodes[neighborCoords];
            if (neighborNode.state != Closed && !isObstacle(engine, neighborCoords)) {

                const bool isDiagonal = i % 2;
                const float distanceThroughCurrent = currentNode.distance + (isDiagonal ? 1.f : sqrt2);
                if (distanceThroughCurrent < neighborNode.distance) {

                    neighborNode.distance = distanceThroughCurrent;
                    neighborNode.totalDistance = distanceThroughCurrent + manhattan(neighborCoords, goal);
                    neighborNode.previous = currentCoords;

                    if (neighborNode.state != Open) {
                        frontier.emplace(neighborCoords);
                        neighborNode.state = Open;
                        assert(nodes.at(neighborCoords).state == Open);
                    }
                }
            }
        }
    }

    return std::nullopt;
}
