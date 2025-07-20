#include Vector2

/* Struct that represents a Rectangle.
 * Consists of four Vector2 corner points.
 */
struct Rect {
    Vector2 TL;
    Vector2 BL;
    Vector2 TR;
    Vector2 BR;

    Rect(Vector2 TL, Vector2 BL, Vector2 TR, Vector2 BR) {
        this->TL = TL;
        this->BL = BL;
        this->TR = TR;
        this->BR = BR;
    };

};