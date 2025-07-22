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
            [this, dt](const std::unique_ptr<Bullet>& bulletPtr) {
                Bullet& bullet = *bulletPtr;
                bullet.update(dt);
                if (!contains(bullet.getPosition().x, bullet.getPosition().y)) {
                    return true;
                }
                for (Entity& entity : entities) {
                    if (bullet.collision(entity)) {
                        entity.setCurrentHealth(entity.getCurrentHealth() - bullet.getDamage());
                        return true;
                    }
                }
                return false;
            }),
        bullets.end()
    );
}


bool PlayArea::contains(float x, float y) {
    return bounds.contains(x, y);
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

void PlayArea::addBullet(std::unique_ptr<Bullet> bullet) {
    bullets.push_back(std::move(bullet));
}

const std::vector<std::unique_ptr<Bullet>>& PlayArea::getBullets() const { return bullets; }

void PlayArea::addEntity(Entity& entity) { entities.push_back(entity); }

std::vector<Entity>& PlayArea::getEntities() { return entities; }

const std::vector<Entity>& PlayArea::getEntities() const { return entities; }