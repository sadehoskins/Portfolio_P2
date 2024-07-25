//
// Created by sadeh on 25.07.2024.
//

#ifndef RAYLIBSTARTER_PATHFINDING_H
#define RAYLIBSTARTER_PATHFINDING_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include "Map.h" // Assuming you have a Map class from previous tasks


class Pathfinding {
public:
    static std::vector<std::pair<int, int>> findPath(const Map& map, int startX, int startY, int endX, int endY);

private:
    static double heuristic(int x1, int y1, int x2, int y2);
    static bool isValid(const Map& map, int x, int y);
    static std::vector<std::pair<int, int>> reconstructPath(Node* endNode);
};

#endif //RAYLIBSTARTER_PATHFINDING_H
