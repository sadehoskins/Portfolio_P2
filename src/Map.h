#ifndef MAP_H
#define MAP_H

#include <vector>
#include "raylib.h"
#include "config.h"
#include <string>
#include <random>
#include "ItemBase.h"

enum TileType {Traversable, Blocked, Beginning, End, Forest, Water, TreasureChest};

struct Node {
    int x, y;
    int g, h;
    int f() const { return g + h; }
    Node*parent;

    Node(int x, int y) : x(x), y(y), g(0), h(0), parent(nullptr) {}
};

struct CompareNode {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->f() > rhs->f();
    }
};
class ItemBase;

class Map {

public:
    Map();
    Map(int width, int height);

    ~Map();

    void generateMap();
    void drawMap(int screenWidth, int screenHeight);
    TileType getTileType(int x, int y) const;
    bool isTraversable(int x, int y) const;
    void placeItem(ItemBase* item, int x, int y);

    int getWidth() const;
    int getHeight() const;
    bool isWall(int x, int y) const;
    void setWall(int x, int y, bool isWall);

    ItemBase* removeItem(int x, int y);
    ItemBase* getItem(int x, int y) const;

    const int mapsize=15;
    const int tilesize=32;



private:
    std::vector<std::vector<TileType>> map;
    std::vector<std::vector<ItemBase*>> items;
    std::mt19937 randomnumbergenerator;
    Texture2D tileTextures[7];  // Array to hold textures for each tile type

    void initializeMap();
    void placeStartAndEnd();
    void addObstacles();
    bool isValidMove(int x, int y) const;
    bool findPath();
    int calculateHeuristic(int x1, int y1, int x2, int y2) const;
    std::pair<int, int> findRandomTraversableTile();
    void loadTextures();
    void unloadTextures();
};

#endif // MAP_H