//
// Created by sadeh on 23.07.2024.
//

#ifndef RAYLIBSTARTER_INVENTORY_H
#define RAYLIBSTARTER_INVENTORY_H

#include <vector>
#include <algorithm>
#include <functional>
#include "ItemBase.h"

template<typename T>
class Inventory {
public:
    Inventory(int slots) : maxSlots(slots) {
        items.resize(maxSlots, nullptr);
        equippedItems.fill(nullptr);
    }

    bool addItem(T* item) {
        for (int i = 0; i < maxSlots; ++i) {
            if (items[i] == nullptr) {
                items[i] = item;
                return true;
            }
        }
        return false;
    }

    T* removeItem(int slot) {
        if (slot >= 0 && slot < maxSlots && items[slot] != nullptr) {
            T* item = items[slot];
            items[slot] = nullptr;
            return item;
        }
        return nullptr;
    }
    const std::vector<T*>& getItems() const { return items; }

    T* getItem(int slot) const {
        if (slot >= 0 && slot < maxSlots) {
            return items[slot];
        }
        return nullptr;
    }

    float getTotalWeight() const {
        float totalWeight = 0;
        for (const auto& item : items) {
            if (item != nullptr) {
                totalWeight += item->getWeight();
            }
        }
        return totalWeight;
    }

    bool equip(int inventorySlot, int equipmentSlot) {
        if (inventorySlot < 0 || inventorySlot >= maxSlots || equipmentSlot < 0 || equipmentSlot >= 3) return false;

        T* item = items[inventorySlot];
        if (!item) return false;

        if (equippedItems[equipmentSlot]) {
            items[inventorySlot] = equippedItems[equipmentSlot];
        } else {
            items[inventorySlot] = nullptr;
        }

        equippedItems[equipmentSlot] = item;
        return true;
    }

    T* getEquippedItem(int equipmentSlot) const {
        if (equipmentSlot < 0 || equipmentSlot >= 3) return nullptr;
        return equippedItems[equipmentSlot];
    }

    void sortByWeight(bool ascending = true) {
        sortBy([ascending](const T* a, const T* b) {
            return ascending ? a->getWeight() < b->getWeight() : a->getWeight() > b->getWeight();
        });
    }

    void sortByName(bool ascending = true) {
        sortBy([ascending](const T* a, const T* b) {
            return ascending ? a->getName() < b->getName() : a->getName() > b->getName();
        });
    }

    void sortByValue(bool ascending = true) {
        sortBy([ascending](const T* a, const T* b) {
            return ascending ? a->getValue() < b->getValue() : a->getValue() > b->getValue();
        });
    }

    std::vector<T*>& getItems() { return items; }

private:
    std::vector<T*> items;
    std::array<T*, 3> equippedItems;
    int maxSlots;

    void sortBy(std::function<bool(const T*, const T*)> comparator) {
        std::stable_sort(items.begin(), items.end(), [&comparator](const T* a, const T* b) {
            if (a == nullptr) return false;
            if (b == nullptr) return true;
            return comparator(a, b);
        });
    }
};


#endif //RAYLIBSTARTER_INVENTORY_H
