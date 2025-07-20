#include "PlayArea.h"

PlayArea::PlayArea(int width, int height) {
    bounds = Rect(
        Vector2({0.0f, 0.0f}), 
        Vector2({0.0f, static_cast<float>(height)}), 
        Vector2({static_cast<float>(width), 0.0f}), 
        Vector2({static_cast<float>(width), static_cast<float>(height)})
        );
}

bool PlayArea::contains(float x, float y) {
    // Assumption: rectangle is actually a rectangle.
    if (bounds.TL.x <= x <= bounds.TR.x)
        if (bounds.TL.y <= y <= bounds.BL.y)
            return true;
    
    return false;
}