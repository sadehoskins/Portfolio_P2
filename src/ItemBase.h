//
// Created by sadeh on 23.07.2024.
//

#ifndef RAYLIBSTARTER_ITEMBASE_H
#define RAYLIBSTARTER_ITEMBASE_H


#include <string>

class ItemBase {
public:
    ItemBase(const std::string& name, float weight, const std::string& description, int value);
    virtual ~ItemBase() = default;

    const std::string& getName() const;
    float getWeight() const;
    const std::string& getDescription() const;
    int getValue() const;

private:
    std::string name;
    float weight;
    std::string description;
    int value;
};


#endif //RAYLIBSTARTER_ITEMBASE_H
