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

    const en::GridPosition neighborDeltas[] {
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

    enum NodeState {
        Unvisited = 0,
        Open,
        Closed
    };

    struct NodeInfo {

        en::GridPosition previous = {0, 0};
        NodeState state = Unvisited;
        float distance = std::numeric_limits<float>::infinity();
        float totalDistance = std::numeric_limits<float>::infinity();

        friend bool operator<(const NodeInfo& lhs, const NodeInfo& rhs) {
            return lhs.totalDistance < rhs.totalDistance;
        }
    };

    inline float manhattan(const en::GridPosition& a, const en::GridPosition& b) {
        return static_cast<float>(glm::abs(a.x - b.x) + glm::abs(a.y - b.y));
    }

    PathfindingPath reconstructPath(const en::GridPosition& end, const en::GridPosition& start,
        const std::unordered_map<en::GridPosition, NodeInfo>& nodes) {

        if (start == end) {
            return {};
        }

        PathfindingPath path {end};

        const NodeInfo* currentNode = &nodes.at(end);
        while (currentNode->previous != start) {
            path.push_front(currentNode->previous);
            currentNode = &nodes.at(currentNode->previous);
        }

        return path;
    };

    bool hasNonObstacleNeighbors(en::Engine& engine, const en::GridPosition& pos) {

        for (int i = 0; i < 8; ++i) {
            if (!Pathfinding::isObstacle(engine, pos + neighborDeltas[i])) {
                return true;
            }
        }

        return false;
    }

}

PathfindingParams::PathfindingParams() :
    maxNumCheckedTiles(10000),
    allowObstacleGoal(false)
{}

std::optional<PathfindingPath> Pathfinding::getPath(en::Engine& engine, const en::GridPosition& start,
    const en::GridPosition& goal,
    const PathfindingParams& params
) {
    if (start == goal) {
        return {};
    }

    if (isObstacle(engine, start)) {
        return std::nullopt;
    }

    if (isObstacle(engine, goal)) {

        if (!params.allowObstacleGoal) {
            return std::nullopt;
        }

        if (!hasNonObstacleNeighbors(engine, goal)) {
            return std::nullopt;
        }
    }

    const auto isGoal = [goal](const en::GridPosition& pos) { return pos == goal; };
    const auto heuristic = [goal](const en::GridPosition& pos) { return manhattan(pos, goal); };
    return getPath(engine, start, isGoal, heuristic, params);
}

std::optional<PathfindingPath> Pathfinding::getPath(en::Engine& engine, const en::GridPosition& start,
    const std::function<bool(const en::GridPosition&)>& isGoal,
    const std::function<float(const en::GridPosition&)>& heuristic,
    const PathfindingParams& params
) {

    if (isGoal(start)) {
        return {};
    }

    if (isObstacle(engine, start)) {
        return std::nullopt;
    }

    // TODO chunk-based node storage for performance.
    std::unordered_map<en::GridPosition, NodeInfo> nodes;
    const auto comparePositions = [&nodes](const en::GridPosition& lhs, const en::GridPosition& rhs) {
        return nodes.at(lhs).totalDistance > nodes.at(rhs).totalDistance;
    };
    std::priority_queue<en::GridPosition, std::vector<en::GridPosition>, decltype(comparePositions)> frontier(comparePositions);

    nodes[start] = {start, Open, 0.f, heuristic(start)};
    frontier.push(start);

    std::size_t numCheckedTiles = 0;
    while (!frontier.empty()) {

        const en::GridPosition currentCoords = frontier.top();
        if (isGoal(currentCoords)) {
            return reconstructPath(currentCoords, start, nodes);
        }

        numCheckedTiles += 1;
        if (numCheckedTiles > params.maxNumCheckedTiles) {
            return std::nullopt;
        }

        frontier.pop();
        NodeInfo& currentNode = nodes.at(currentCoords);
        currentNode.state = Closed;

        // Walk through neighbors.
        for (int i = 0; i < 8; ++i) {

            const en::GridPosition neighborCoords = currentCoords + neighborDeltas[i];
            NodeInfo& neighborNode = nodes[neighborCoords];
            if (neighborNode.state != Closed) {

                if (!isObstacle(engine, neighborCoords) || (params.allowObstacleGoal && isGoal(neighborCoords))) {

                    const bool isDiagonal = i % 2;
                    const float distanceThroughCurrent = currentNode.distance + (isDiagonal ? sqrt2 : 1.f);
                    if (distanceThroughCurrent < neighborNode.distance) {

                        neighborNode.distance = distanceThroughCurrent;
                        neighborNode.totalDistance = distanceThroughCurrent + heuristic(neighborCoords);
                        neighborNode.previous = currentCoords;

                        if (neighborNode.state != Open) {
                            frontier.emplace(neighborCoords);
                            neighborNode.state = Open;
                        }
                    }
                }
            }
        }
    }

    return std::nullopt;
}


bool Pathfinding::isObstacle(en::Engine& engine, const en::GridPosition& tileCoordinates) {

    const en::EntityRegistry& registry = engine.getRegistry();
    for (en::Entity e : registry.with<en::TileLayer>()) {
        if (registry.get<en::TileLayer>(e).at(tileCoordinates).isObstacle) {
            return true;
        }
    }

    return false;
}
