//
// Created by sadeh on 23.07.2024.
//

#ifndef RAYLIBSTARTER_CHARACTER_H
#define RAYLIBSTARTER_CHARACTER_H

class Map;

class Character {
public:
    Character(int x, int y);
    virtual ~Character() = default;

    virtual bool move(Map& map, int dx, int dy) = 0;
    int getX() const;
    int getY() const;

protected:
    int x, y;
};


#endif //RAYLIBSTARTER_CHARACTER_H
