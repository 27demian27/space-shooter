#include <iostream>

#include "Rect.h"
#include "Circle.h"

bool Rect::contains(float x, float y) const {
    return (x >= TL.x && x <= TR.x &&
            y >= TL.y && y <= BL.y);
}

bool Rect::intersects(const Hitbox& other) const {
    if (other.shape == HitboxShape::RECT) {
        const Rect* rect = dynamic_cast<const Rect*>(&other);
        if (!rect) return false;

        return !(
            TL.x > rect->TR.x || 
            TR.x < rect->TL.x || 
            TL.y > rect->BL.y || 
            BL.y < rect->TL.y
        );
    }

    if (other.shape != HitboxShape::CIRCLE)
        return false;

    const Circle* circle = dynamic_cast<const Circle*>(&other);
    if (!circle) return false;

    return circle->intersects(*this);
}