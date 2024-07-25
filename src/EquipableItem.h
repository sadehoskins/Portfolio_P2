//
// Created by sadeh on 23.07.2024.
//

#ifndef RAYLIBSTARTER_EQUIPABLEITEM_H
#define RAYLIBSTARTER_EQUIPABLEITEM_H

#include "ItemBase.h"

class EquipableItem : public ItemBase {
public:
    enum ItemType { Ring, Pants, Weapon };

    EquipableItem(const std::string& name, float weight, const std::string& description, int value, ItemType type, int strengthBonus);

    ItemType getItemType() const;
    int getStrengthBonus() const;

private:
    ItemType type;
    int strengthBonus;
};

#endif //RAYLIBSTARTER_EQUIPABLEITEM_H
