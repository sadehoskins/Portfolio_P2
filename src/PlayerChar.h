//
// Created by sadeh on 23.07.2024.
//

#ifndef RAYLIBSTARTER_PLAYERCHAR_H
#define RAYLIBSTARTER_PLAYERCHAR_H

#include "Character.h"
#include "Inventory.h"
#include "Map.h"
#include "EquipableItem.h"


class PlayerChar : public Character {
public:
    PlayerChar(int x, int y, int strength);

    bool move(Map& map, int dx, int dy) override;
    bool pickUpItem(Map& map);
    bool dropItem(Map& map, int inventorySlot);
    bool equipItem(int inventorySlot, int equipmentSlot);
    int getStrength() const;
    void updateStrength();

    ItemBase* getInventoryItem(int slot) const;
    ItemBase* getEquippedItem(int slot) const;
    bool useItem(int inventorySlot);

    void demonstrateSorting();

private:
    Inventory<ItemBase> inventory;
    int baseStrength;
    int strength;
    static const int MAX_WEIGHT_PER_STRENGTH = 2;

    bool canCarryWeight(float additionalWeight) const;
    void generateArtificialInventory();
    void printInventory(const std::string& message) const;
};


#endif //RAYLIBSTARTER_PLAYERCHAR_H
