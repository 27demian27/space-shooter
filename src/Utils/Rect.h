#ifndef RECT_H
#define RECT_H

#include "Hitbox.h"
#include "Circle.h"
#include "Vector2.h"

/**
 * @brief Struct that represents a Rectangle Hitbox.
 *
 * Represented by four Vector2 corner points.
 */
struct Rect : public Hitbox {
    Vector2 TL;
    Vector2 BL;
    Vector2 TR;
    Vector2 BR;

    Rect() 
     : Hitbox(HitboxShape::RECT), 
       TL({0, 0}), 
       BL({0, 0}), 
       TR({0, 0}),
       BR({0, 0}) 
    {};

    Rect(Vector2 TL, Vector2 BL, Vector2 TR, Vector2 BR)
     : Hitbox(HitboxShape::RECT), 
       TL(TL), 
       BL(BL), 
       TR(TR),
       BR(BR) 
    {};

    Vector2 size() const override { return Vector2({TR.x - TL.x, BL.y - TL.y}); }


    bool contains(float x, float y) const override;

    bool intersects(const Hitbox& other) const override;


};

#endif //RECT