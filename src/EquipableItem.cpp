//
// Created by sadeh on 23.07.2024.
//

#include "EquipableItem.h"

EquipableItem::EquipableItem(const std::string& name, float weight, const std::string& description, int value, ItemType type, int strengthBonus)
        : ItemBase(name, weight, description, value), type(type), strengthBonus(strengthBonus) {}

EquipableItem::ItemType EquipableItem::getItemType() const { return type; }
int EquipableItem::getStrengthBonus() const { return strengthBonus; }