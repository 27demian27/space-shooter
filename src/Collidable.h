#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "Utils/Rect.h"

class Collidable {

public:
    virtual bool collision(Collidable& other) = 0;

    virtual Rect getHitbox() = 0;

protected:
    float collision_damage;
    Rect hitbox;
};

#endif //COLLIDABLE_H