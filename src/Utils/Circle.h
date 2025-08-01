#ifndef CIRCLE_H
#define CIRCLE_H

#include "Hitbox.h"
#include "Rect.h"
#include "Vector2.h"

/**
 * @brief Struct that represent a Circle Hitbox
 *
 * Represented by a radius and middlepoint.
 */
struct Circle : public Hitbox {

    float radius;
    Vector2 middlepoint;

    Circle()
     : Hitbox(HitboxShape::CIRCLE),
       radius(1.0f),
       middlepoint({0, 0})
    {};

    Circle(float radius, Vector2 middlepoint)
     : Hitbox(HitboxShape::CIRCLE),
       radius(radius),
       middlepoint(middlepoint)
    {};

    Vector2 size() const override { 
        return Vector2(
        {
            2.0f * radius, 
            2.0f * radius
        }); 
    }


    bool contains(float x, float y) const override;

    bool intersects(const Hitbox& other) const override;

};


#endif // CIRCLE_H