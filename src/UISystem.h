//
// Created by sadeh on 23.07.2024.
//

#ifndef RAYLIBSTARTER_UISYSTEM_H
#define RAYLIBSTARTER_UISYSTEM_H

#include "raylib.h"
#include "PlayerChar.h"

class UISystem {
public:
    UISystem(PlayerChar& player);
    void draw();
    void handleClick(Vector2 mousePos);

private:
    PlayerChar& player;
    Rectangle inventoryRect;
    Rectangle equipmentRect;
    static const int SLOT_SIZE = 40;
    static const int SLOT_PADDING = 5;

    void drawInventory();
    void drawEquipment();
    void drawItemSlot(Rectangle rect, ItemBase* item);
};


#endif //RAYLIBSTARTER_UISYSTEM_H
