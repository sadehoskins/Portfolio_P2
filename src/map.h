//
// Created by lrfri on 09.07.2024.
//

#ifndef RAYLIBSTARTER_MAP_H
#define RAYLIBSTARTER_MAP_H


#include <vector>
#include "raylib.h"
#include "config.h"

class map {
protected:
    int mapWidth{};
    int mapHeight{};
    std::vector<int> tileMap;
public:
    void update();
    void draw();
    map(const char * filename);
private:
};


#endif //RAYLIBSTARTER_MAP_H
