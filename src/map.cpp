//
// Created by lrfri on 09.07.2024.
//

#include "map.h"

void map::update() {}

void map::draw() {

    //Map besteht aus einzelnen Tiles (1a) und ist mindestens 15x15 Tiles groß (1a)
    //Mindestens 4 Arten Tiles (1a), Ziel und Ende erkennbar (1a)
    //Begehbarer Weg von Start (unterste Zeile) zu Ziel (oberste Zeile) (1a)

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tile = tileMap[y * mapWidth + x];
            DrawRectangle(x * 16, y * 16, 16, 16, tile == 0 ? WHITE : tile == 1 ? GRAY : tile == 2 ? DARKGRAY : BLACK);
        }
    }
}

map::map(const char *filename) {
    if (filename == nullptr) {
        mapWidth = Game::ScreenWidth / 16;
        mapHeight = Game::ScreenHeight / 16;
        for (int i = 0; i < mapWidth * mapHeight; i++) {
            //verteilt die Tiles zufällig auf der Map
            tileMap.push_back(GetRandomValue(0,3));
        }
    }
}



