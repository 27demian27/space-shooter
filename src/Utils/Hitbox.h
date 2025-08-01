#ifndef HITBOX_H
#define HITBOX_H

#include "Vector2.h"

enum HitboxShape {
    RECT,
    CIRCLE
};

/**
 * @brief Struct that represents a Hitbox.
 *
 * Has an enum depicting the type of shape.
 */
struct Hitbox {

    HitboxShape shape;

    Hitbox(HitboxShape shape) 
     : shape(shape) 
    {};

    virtual ~Hitbox() = default;

    virtual Vector2 size() const = 0;
    virtual bool contains(float x, float y) const = 0;
    virtual bool intersects(const Hitbox& other) const = 0;

};

#endif // HITBOX_H