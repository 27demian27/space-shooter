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
            [this, dt](const std::unique_ptr<Bullet>& bullet_ptr) {
                Bullet& bullet = *bullet_ptr;
                bullet.update(dt);
                if (!contains(bullet.getPosition().x, bullet.getPosition().y)) {
                    return true;
                }
                for (const auto& entity_ptr : entities) {
                    Entity& entity = *entity_ptr;
                    if (bullet.collision(entity)) {
                        entity.setCurrentHealth(entity.getCurrentHealth() - bullet.getDamage());
                        std::cout << "HIT\n";
                        return true;
                    }
                }
                return false;
            }),
        bullets.end()
    );
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [this, dt](const std::unique_ptr<Entity>& entity_ptr) {
                Entity& entity = *entity_ptr;
                entity.update(dt);
                if (!entity.isAlive()) {
                    if (entity.getCurrentHealth() <= 0) {
                        std::cout << "Entity died\n";
                    }
                    return true;
                }
                return false;
            }),
        entities.end()
    );
}


bool PlayArea::contains(float x, float y) const {
    return bounds.contains(x, y);
}

bool PlayArea::contains(const Rect& rectHitbox) {
    return bounds.intersects(rectHitbox);
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

void PlayArea::addEntity(std::unique_ptr<Entity> entity) { entities.push_back(std::move(entity)); }

std::vector<std::unique_ptr<Entity>>& PlayArea::getEntities() { return entities; }

const std::vector<std::unique_ptr<Entity>>& PlayArea::getEntities() const { return entities; }