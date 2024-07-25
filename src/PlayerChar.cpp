//
// Created by sadeh on 23.07.2024.
//

#include "PlayerChar.h"
#include "EquipableItem.h"
#include <iostream>
#include <random>

PlayerChar::PlayerChar(int x, int y, int strength)
        : Character(x, y), inventory(20), baseStrength(strength), strength(strength) {}

bool PlayerChar::move(Map &map, int dx, int dy) {
    int newX = x + dx;
    int newY = y + dy;
    if (map.isTraversable(newX, newY) && canCarryWeight(0)) {
        x = newX;
        y = newY;
        return true;
    }
    return false;
}

bool PlayerChar::pickUpItem(Map &map) {
    ItemBase *item = map.removeItem(x, y);
    if (item && canCarryWeight(item->getWeight())) {
        inventory.addItem(item);
        return true;
    }
    if (item) {
        map.placeItem(item, x, y);
    }
    return false;
}

bool PlayerChar::dropItem(Map &map, int inventorySlot) {
    ItemBase *item = inventory.removeItem(inventorySlot);
    if (item) {
        map.placeItem(item, x, y);
        return true;
    }
    return false;
}

void PlayerChar::updateStrength() {
    int bonusStrength = 0;
    for (int i = 0; i < 3; ++i) {
        ItemBase *equippedItem = inventory.getEquippedItem(i);
        if (equippedItem) {
            EquipableItem *equipableItem = dynamic_cast<EquipableItem *>(equippedItem);
            if (equipableItem) {
                bonusStrength += equipableItem->getStrengthBonus();
            }
        }
    }
    strength = baseStrength + bonusStrength;
}

bool PlayerChar::equipItem(int inventorySlot, int equipmentSlot) {
    if (inventory.equip(inventorySlot, equipmentSlot)) {
        updateStrength();
        return true;
    }
    return false;
}

int PlayerChar::getStrength() const {
    return strength;
}

bool PlayerChar::canCarryWeight(float additionalWeight) const {
    return inventory.getTotalWeight() + additionalWeight <= strength * MAX_WEIGHT_PER_STRENGTH;
}

ItemBase *PlayerChar::getInventoryItem(int slot) const {
    return inventory.getItem(slot);
}

ItemBase *PlayerChar::getEquippedItem(int slot) const {
    return inventory.getEquippedItem(slot);
}

bool PlayerChar::useItem(int inventorySlot) {
    ItemBase *item = inventory.getItem(inventorySlot);
    if (item) {
        // Implement item use logic here
        // For example, if it's a healing potion:
        // health += 50;
        // inventory.removeItem(inventorySlot);
         return true;
    }
    return false;
}

void PlayerChar::demonstrateSorting() {
    generateArtificialInventory();

    printInventory("Unsorted Inventory:");

    inventory.sortByWeight(true);
    printInventory("Sorted by Weight (Ascending):");

    inventory.sortByWeight(false);
    printInventory("Sorted by Weight (Descending):");

    inventory.sortByName(true);
    printInventory("Sorted by Name (Ascending):");

    inventory.sortByName(false);
    printInventory("Sorted by Name (Descending):");

    inventory.sortByValue(true);
    printInventory("Sorted by Value (Ascending):");

    inventory.sortByValue(false);
    printInventory("Sorted by Value (Descending):");
}

void PlayerChar::generateArtificialInventory() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> itemCount(10, 20);
    std::uniform_real_distribution<> weight(0.1, 10.0);
    std::uniform_int_distribution<> value(1, 100);

    int count = itemCount(gen);

    for (int i = 0; i < count; ++i) {
        std::string name = "Item" + std::to_string(i + 1);
        float itemWeight = static_cast<float>(std::round(weight(gen) * 10.0) / 10.0);
        int itemValue = value(gen);
        inventory.addItem(new ItemBase(name, itemWeight, "Description", itemValue));
    }
}

void PlayerChar::printInventory(const std::string &message) const {
    std::cout << message << std::endl;
    for (const auto &item: inventory.getItems()) {
        if (item) {
            std::cout << "Name: " << item->getName()
                      << ", Weight: " << item->getWeight()
                      << ", Value: " << item->getValue() << std::endl;
        }
    }
    std::cout << std::endl;
}