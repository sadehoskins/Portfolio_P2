//
// Created by sadeh on 23.07.2024.
//

#include "ItemBase.h"

ItemBase::ItemBase(const std::string& name, float weight, const std::string& description, int value)
        : name(name), weight(weight > 0 ? weight : 0.1f), description(description), value(value) {}

const std::string& ItemBase::getName() const { return name; }
float ItemBase::getWeight() const { return weight; }
const std::string& ItemBase::getDescription() const { return description; }
int ItemBase::getValue() const { return value; }