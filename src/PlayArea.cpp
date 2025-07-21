#include "PlayArea.h"

#include <algorithm>
#include <iostream>

PlayArea::PlayArea(int width, int height) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    bounds = Rect(
            Vector2({-halfWidth, -halfHeight}),
            Vector2({-halfWidth, halfHeight}),
            Vector2({halfWidth, -halfHeight}),
            Vector2({-halfWidth, -halfHeight})
        );
}

void PlayArea::update(float dt) {
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [this, dt](Bullet& bullet) {
                if (!contains(bullet.getPosition().x, bullet.getPosition().y)) {
                    // std::cout << "bullet x: " << bullet.getPosition().x <<" y: " << bullet.getPosition().y << "\n";
                    // std::cout << "remove\n";
                    return true;
                }
                bullet.update(dt);
                return false;
            }),
        bullets.end()
    );
}

bool PlayArea::contains(float x, float y) {
    //std::cout << "contains x: " << x << " y: " << y << "\n";
    // Assumption: rectangle is actually a rectangle.
    return (x >= bounds.TL.x && x <= bounds.TR.x &&
            y >= bounds.TL.y && y <= bounds.BL.y);
}

void PlayArea::clipToArea(float& x, float& y) {
    if(x < bounds.TL.x) {
        x = bounds.TL.x;
    } 
    else if (x > bounds.TR.x) {
        x = bounds.TR.x;
    } 
    if (y < bounds.TL.y) {
        y = bounds.TL.y;
    }
    else if (y > bounds.BL.y) {
        y = bounds.BL.y;
    }
}

void PlayArea::addBullet(Bullet& bullet) {
    bullets.push_back(bullet);
}

std::vector<Bullet> PlayArea::getBullets() { return bullets; }