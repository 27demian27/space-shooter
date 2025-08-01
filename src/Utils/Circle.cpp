#include <iostream>
#include <algorithm>

#include "Circle.h"
#include "Rect.h"

bool Circle::contains(float x, float y) const {
        return ((Vector2({x, y}) - middlepoint).length() < radius);
    }

bool Circle::intersects(const Hitbox& other) const {
    if (other.shape == HitboxShape::RECT) {
        const Rect* rect = dynamic_cast<const Rect*>(&other);
        if (!rect) return false;

        float closestX = std::clamp(middlepoint.x, rect->TL.x, rect->TR.x);
        float closestY = std::clamp(middlepoint.y, rect->TL.y, rect->BL.y);

        float dx = middlepoint.x - closestX;
        float dy = middlepoint.y - closestY;

        return (dx * dx + dy * dy) <= (radius * radius);
        return false;
    } 
    else if (other.shape == HitboxShape::CIRCLE) {
        const Circle* circle = dynamic_cast<const Circle*>(&other);
        if (!circle) return false;

        float dist = (middlepoint - circle->middlepoint).length();
        return dist <= (radius + circle->radius);
    }
    else {
        return false;
    }

}