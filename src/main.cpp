#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "raylib.h"
#include "config.h"
#include "Map.h"
#include "PlayerChar.h"
#include "UISystem.h"
#include "Pathfinding.h"

const int screenWidth = 800;
const int screenHeight = 600;

void printMapWithPath(const Map& map, const std::vector<std::pair<int, int>>& path) {
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            if (map.isWall(x, y)) {
                std::cout << "█";
            } else if (std::find(path.begin(), path.end(), std::make_pair(x, y)) != path.end()) {
                std::cout << "*";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h.in file
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(Game::ScreenWidth, Game::ScreenHeight, "Portfolio_Programming2");
    SetTargetFPS(60);

#ifdef GAME_START_FULLSCREEN
    ToggleFullscreen();
#endif

    RenderTexture2D canvas = LoadRenderTexture(Game::ScreenWidth, Game::ScreenHeight);
    float renderScale{};
    Rectangle renderRec{};

    Map mapGenerator;
    mapGenerator.generateMap();

    Map map(20, 10);
    // Add some walls to the map
    map.setWall(5, 0, true);
    map.setWall(5, 1, true);
    map.setWall(5, 2, true);
    map.setWall(5, 3, true);
    map.setWall(5, 4, true);

    // Define start and end points
    int startX = 0, startY = 0;
    int endX = 19, endY = 9;

    // Find the path
    std::vector<std::pair<int, int>> path = Pathfinding::findPath(map, startX, startY, endX, endY);

    // Print the result
    if (path.empty()) {
        std::cout << "No path found!" << std::endl;
    } else {
        std::cout << "Path found:" << std::endl;
        printMapWithPath(map, path);
    }

    return 0;
}

    // Create player character
    PlayerChar player(mapGenerator.mapsize - 1, mapGenerator.mapsize / 2, 10);

    // Create UI system
    UISystem uiSystem(player);

    Texture2D playerTexture = LoadTexture("assets/player.png");

    // Game state
    bool gameWon = false;

    player.demonstrateSorting();

    // Main game loop
    while (!WindowShouldClose())
    {
        // Handle fullscreen toggle
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)) {
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
                SetWindowSize(Game::ScreenWidth, Game::ScreenHeight);
            } else {
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ToggleFullscreen();
            }
        }

        if (!gameWon) {
            // Handle player movement
            if (IsKeyPressed(KEY_W)) player.move(mapGenerator, 0, -1);
            if (IsKeyPressed(KEY_S)) player.move(mapGenerator, 0, 1);
            if (IsKeyPressed(KEY_A)) player.move(mapGenerator, -1, 0);
            if (IsKeyPressed(KEY_D)) player.move(mapGenerator, 1, 0);

            // Handle item pickup
            if (IsKeyPressed(KEY_E)) player.pickUpItem(mapGenerator);

            // Handle item drop (example: drop item from first inventory slot)
            if (IsKeyPressed(KEY_Q)) player.dropItem(mapGenerator, 0);

            // Handle item equip (example: equip item from first inventory slot to first equipment slot)
            if (IsKeyPressed(KEY_R)) player.equipItem(0, 0);

            // Handle UI interactions
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                uiSystem.handleClick(mousePos);
            }

            // Check win condition
            if (player.getX() == 0 && player.getY() == mapGenerator.mapsize - 1) {
                gameWon = true;
            }
        }

        BeginDrawing();
        BeginTextureMode(canvas);
        {
            ClearBackground(WHITE);
            mapGenerator.drawMap(Game::ScreenWidth, Game::ScreenHeight);

            // Draw player

            DrawTexture(playerTexture,
                        player.getX() * mapGenerator.tilesize,
                        player.getY() * mapGenerator.tilesize,
                        WHITE, playerTexture);

            // Draw UI
            uiSystem.draw();

            if (gameWon) {
                // Draw win screen
                DrawText("You Win!", GetScreenWidth() / 2 - MeasureText("You Win!", 40) / 2, GetScreenHeight() / 2 - 20, 40, GREEN);
            }
        }
        EndTextureMode();

        ClearBackground(BLACK);

        renderScale = std::min(GetScreenHeight() / (float)canvas.texture.height,
                               GetScreenWidth() / (float)canvas.texture.width);
        renderScale = floorf(renderScale);
        if (renderScale < 1) renderScale = 1;
        renderRec.width = canvas.texture.width * renderScale;
        renderRec.height = canvas.texture.height * renderScale;
        renderRec.x = (GetScreenWidth() - renderRec.width) / 2.0f;
        renderRec.y = (GetScreenHeight() - renderRec.height) / 2.0f;
        DrawTexturePro(canvas.texture,
                       Rectangle{0, 0, (float)canvas.texture.width, (float)-canvas.texture.height},
                       renderRec, {}, 0, WHITE);

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_S)) {
            DrawText(TextFormat("Render scale: %.0f", renderScale), 10, 10, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(playerTexture);
    UnloadRenderTexture(canvas);
    CloseWindow();

    return EXIT_SUCCESS;
}