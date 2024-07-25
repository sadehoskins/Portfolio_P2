//
// Created by sadeh on 25.07.2024.
//

#include "Pathfinding.h"
#include <algorithm>

std::vector<std::pair<int, int>> Pathfinding::findPath(const Map& map, int startX, int startY, int endX, int endY) {
    std::vector<std::vector<Node>> nodes(map.getHeight(), std::vector<Node>(map.getWidth(), Node(0, 0)));

    auto compare = [](const Node* a, const Node* b) { return a->f > b->f; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openSet(compare);

    std::unordered_map<Node*, bool> closedSet;

    Node* startNode = &nodes[startY][startX];
    startNode->g = 0;
    startNode->h = heuristic(startX, startY, endX, endY);
    startNode->f = startNode->g + startNode->h;

    openSet.push(startNode);

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == endX && current->y == endY) {
            return reconstructPath(current);
        }

        closedSet[current] = true;

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                int newX = current->x + dx;
                int newY = current->y + dy;

                if (!isValid(map, newX, newY)) continue;

                Node* neighbor = &nodes[newY][newX];
                if (closedSet[neighbor]) continue;

                double tentativeG = current->g + 1; // Assuming all moves cost 1

                if (tentativeG < neighbor->g || !neighbor->parent) {
                    neighbor->parent = current;
                    neighbor->g = tentativeG;
                    neighbor->h = heuristic(newX, newY, endX, endY);
                    neighbor->f = neighbor->g + neighbor->h;

                    openSet.push(neighbor);
                }
            }
        }
    }

    return {}; // No path found
}

double Pathfinding::heuristic(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

bool Pathfinding::isValid(const Map& map, int x, int y) {
    return x >= 0 && x < map.getWidth() && y >= 0 && y < map.getHeight() && !map.isWall(x, y);
}

std::vector<std::pair<int, int>> Pathfinding::reconstructPath(Node* endNode) {
    std::vector<std::pair<int, int>> path;
    Node* current = endNode;
    while (current) {
        path.emplace_back(current->x, current->y);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}