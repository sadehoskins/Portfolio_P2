#include "Map.h"
#include <queue>
#include <unordered_set>
#include <algorithm>

Map::Map() : randomnumbergenerator(std::random_device{}()) {
    items.resize(mapsize, std::vector<ItemBase*>(mapsize, nullptr));
    loadTextures();
}

Map::~Map() {
    for (auto& row : items) {
        for (auto& item : row) {
            delete item;
        }
    }
    unloadTextures();
}

void Map::initializeMap() {
    map = std::vector<std::vector<TileType>>(mapsize, std::vector<TileType>(mapsize, TileType::Traversable));
}

void Map::placeStartAndEnd() {
    int startX = std::uniform_int_distribution<>(0, mapsize - 1)(randomnumbergenerator);
    int endX = std::uniform_int_distribution<>(0, mapsize - 1)(randomnumbergenerator);
    map[mapsize - 1][startX] = TileType::Beginning;
    map[0][endX] = TileType::End;
}

void Map::addObstacles() {
    for (int i = 0; i < mapsize; ++i) {
        for (int j = 0; j < mapsize; ++j) {
            if (map[i][j] == TileType::Traversable) {
                double random = std::uniform_real_distribution<>()(randomnumbergenerator);
                if (random < 0.2) {
                    map[i][j] = TileType::Blocked;
                } else if (random < 0.3) {
                    map[i][j] = TileType::Forest;
                } else if (random < 0.4) {
                    map[i][j] = TileType::Water;
                } else if (random < 0.45) {
                    map[i][j] = TileType::TreasureChest;
                }
            }
        }
    }
}

bool Map::isValidMove(int x, int y) const {
    return x >= 0 && x < mapsize && y >= 0 && y < mapsize &&
           (map[y][x] == TileType::Traversable || map[y][x] == TileType::Beginning ||
            map[y][x] == TileType::End || map[y][x] == TileType::TreasureChest);
}

bool Map::findPath() {
    std::vector<std::vector<Node *>> nodes(mapsize, std::vector<Node *>(mapsize, nullptr));
    std::priority_queue<Node *, std::vector<Node *>, CompareNode> openSet;
    std::unordered_set<Node *> openSetHash;

    // Find start position
    int startX, startY, endX, endY;
    for (int i = 0; i < mapsize; ++i) {
        if (map[mapsize - 1][i] == TileType::Beginning) {
            startX = i;
            startY = mapsize - 1;
        }
        if (map[0][i] == TileType::Beginning) {
            endX = i;
            endY = 0;
        }
    }

    // create start node and add to open set
    Node *startNode = new Node(startX, startY);
    startNode->h = calculateHeuristic(startX, startY, endX, endY);
    openSet.push(startNode);
    openSetHash.insert(startNode);
    nodes[startY][startX] = startNode;

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    while (!openSet.empty()) {
        Node *current = openSet.top();
        openSet.pop();
        openSetHash.erase(current);

        if (current->x == endX && current->y == endY) {
            // Path found, clean up and return true
            for (auto &row: nodes) {
                for (auto &node: row) {
                    delete node;
                }
            }
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (isValidMove(newX, newY)) {
                int newG = current->g + 1;

                Node *neighbor = nodes[newY][newX];
                if (!neighbor) {
                    neighbor = new Node(newX, newY);
                    neighbor->h = calculateHeuristic(newX, newY, endX, endY);
                    nodes[newY][newX] = neighbor;
                }

                if (newG < neighbor->g || neighbor->g == 0) {
                    neighbor->parent = current;
                    neighbor->g = newG;
                    if (openSetHash.find(neighbor) == openSetHash.end()) {
                        openSet.push(neighbor);
                        openSetHash.insert(neighbor);
                    }
                }
            }
        }
    }
// No path found, clean up and return false
    for (auto &row: nodes) {
        for (auto &node: row) {
            delete node;
        }
    }
    return false;
}

int Map::calculateHeuristic(int x1, int y1, int x2, int y2) const {
    // Manhattan distance
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

void Map::generateMap() {
    do {
        initializeMap();
        placeStartAndEnd();
        addObstacles();
    } while (!findPath());

    // Place items
    int itemCount = 0;
    while (itemCount < 5) {
        auto [x, y] = findRandomTraversableTile();
        if (items[y][x] == nullptr) {
            items[y][x] = new ItemBase("Random Item", 1.0f, "A random item", 10);
            itemCount++;
        }
    }
}
std::pair<int, int> Map::findRandomTraversableTile() {
    std::vector<std::pair<int, int>> traversableTiles;
    for (int y = 0; y < mapsize; ++y) {
        for (int x = 0; x < mapsize; ++x) {
            if (isTraversable(x, y)) {
                traversableTiles.emplace_back(x, y);
            }
        }
    }
    std::uniform_int_distribution<> dis(0, traversableTiles.size() - 1);
    return traversableTiles[dis(randomnumbergenerator)];
}

void Map::drawMap(int screenWidth, int screenHeight) {
    int mapWidth = mapsize * tilesize;
    int mapHeight = mapsize * tilesize;
    int offsetX = (screenWidth - mapWidth) / 2;
    int offsetY = (screenHeight - mapHeight) / 2;

    for (int y = 0; y < mapsize; ++y) {
        for (int x = 0; x < mapsize; ++x) {
            Rectangle tileRect = {
                    static_cast<float>(offsetX + x * tilesize),
                    static_cast<float>(offsetY + y * tilesize),
                    static_cast<float>(tilesize), static_cast<float>(tilesize)
            };
            DrawTexture(tileTextures[map[y][x]], tileRect.x, tileRect.y, WHITE, Texture2D());

            if (items[y][x] != nullptr) {
                // Draw item icon or indicator
                DrawRectangle(tileRect.x + 4, tileRect.y + 4, tilesize - 8, tilesize - 8, YELLOW);
            }
        }
    }
}

TileType Map::getTileType(int x, int y) const {
    if (x >= 0 && x < mapsize && y >= 0 && y < mapsize) {
        return map[y][x];
    }
    return TileType::Blocked;
}

bool Map::isTraversable(int x, int y) const {
    TileType type = getTileType(x, y);
    return type == TileType::Traversable || type == TileType::Beginning ||
           type == TileType::End || type == TileType::TreasureChest;
}

void Map::placeItem(ItemBase* item, int x, int y) {
    if (isTraversable(x, y) && items[y][x] == nullptr) {
        items[y][x] = item;
    }
}

ItemBase* Map::removeItem(int x, int y) {
    if (isTraversable(x, y) && items[y][x] != nullptr) {
        ItemBase* item = items[y][x];
        items[y][x] = nullptr;
        return item;
    }
    return nullptr;
}

ItemBase* Map::getItem(int x, int y) const {
    if (isTraversable(x, y)) {
        return items[y][x];
    }
    return nullptr;
}

void Map::loadTextures() {
    tileTextures[Traversable] = LoadTexture("assets/traversable.png");
    tileTextures[Blocked] = LoadTexture("assets/blocked.png");
    tileTextures[Beginning] = LoadTexture("assets/beginning.png");
    tileTextures[End] = LoadTexture("assets/end.png");
    tileTextures[Forest] = LoadTexture("assets/forest.png");
    tileTextures[Water] = LoadTexture("assets/water.png");
    tileTextures[TreasureChest] = LoadTexture("assets/treasure_chest.png");
}

void Map::unloadTextures() {
    for (int i = 0; i < 7; ++i) {
        UnloadTexture(tileTextures[i]);
    }
}
