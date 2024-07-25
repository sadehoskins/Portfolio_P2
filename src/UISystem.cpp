//
// Created by sadeh on 23.07.2024.
//

#include "UISystem.h"
#include "EquipableItem.h"

UISystem::UISystem(PlayerChar& player) : player(player) {
    inventoryRect = {10, 10, 5 * (SLOT_SIZE + SLOT_PADDING), 2 * (SLOT_SIZE + SLOT_PADDING)};
    equipmentRect = {10, inventoryRect.y + inventoryRect.height + 10, 3 * (SLOT_SIZE + SLOT_PADDING), SLOT_SIZE + SLOT_PADDING};
}

void UISystem::draw() {
    drawInventory();
    drawEquipment();
}

void UISystem::drawInventory() {
    DrawRectangleRec(inventoryRect, LIGHTGRAY);
    DrawRectangleLinesEx(inventoryRect, 2, BLACK);
    DrawText("Inventory", inventoryRect.x + 5, inventoryRect.y - 20, 20, BLACK);

    for (int i = 0; i < 10; i++) {
        int x = inventoryRect.x + (i % 5) * (SLOT_SIZE + SLOT_PADDING);
        int y = inventoryRect.y + (i / 5) * (SLOT_SIZE + SLOT_PADDING);
        Rectangle slotRect = {(float)x, (float)y, SLOT_SIZE, SLOT_SIZE};
        drawItemSlot(slotRect, player.getInventoryItem(i));
    }
}

void UISystem::drawEquipment() {
    DrawRectangleRec(equipmentRect, LIGHTGRAY);
    DrawRectangleLinesEx(equipmentRect, 2, BLACK);
    DrawText("Equipment", equipmentRect.x + 5, equipmentRect.y - 20, 20, BLACK);

    for (int i = 0; i < 3; i++) {
        int x = equipmentRect.x + i * (SLOT_SIZE + SLOT_PADDING);
        Rectangle slotRect = {(float)x, equipmentRect.y, SLOT_SIZE, SLOT_SIZE};
        drawItemSlot(slotRect, player.getEquippedItem(i));
    }
}

void UISystem::drawItemSlot(Rectangle rect, ItemBase* item) {
    DrawRectangleRec(rect, WHITE);
    DrawRectangleLinesEx(rect, 1, BLACK);
    if (item) {
        DrawText(item->getName().substr(0, 2).c_str(), rect.x + 5, rect.y + 5, 20, BLACK);
    }
}

void UISystem::handleClick(Vector2 mousePos) {
    for (int i = 0; i < 10; i++) {
        int x = inventoryRect.x + (i % 5) * (SLOT_SIZE + SLOT_PADDING);
        int y = inventoryRect.y + (i / 5) * (SLOT_SIZE + SLOT_PADDING);
        Rectangle slotRect = {(float)x, (float)y, SLOT_SIZE, SLOT_SIZE};
        if (CheckCollisionPointRec(mousePos, slotRect)) {
            ItemBase* item = player.getInventoryItem(i);
            if (item) {
                EquipableItem* equipableItem = dynamic_cast<EquipableItem*>(item);
                if (equipableItem) {
                    player.equipItem(i, equipableItem->getItemType());
                }
            }
            return;
        }
    }
}
