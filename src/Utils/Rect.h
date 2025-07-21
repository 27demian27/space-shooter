#include "Vector2.h"

/**
 * @brief Struct that represents a Rectangle.
 *
 * Consists of four Vector2 corner points.
 */
struct Rect {
    Vector2 TL;
    Vector2 BL;
    Vector2 TR;
    Vector2 BR;

    Rect() {TL = {0, 0}; BL = {0, 0}; TR = {0, 0}; BR = {0, 0};};

    Rect(Vector2 TL, Vector2 BL, Vector2 TR, Vector2 BR) {
        this->TL = TL;
        this->BL = BL;
        this->TR = TR;
        this->BR = BR;
    };

};