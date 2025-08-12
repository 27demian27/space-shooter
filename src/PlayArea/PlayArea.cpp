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

    was_collision = false;
    points_to_give = 0;
}

void PlayArea::update(float dt) {
    was_collision = false;
    points_to_give = 0;
    collision_coords.clear();

    for (auto& bullet_ptr : bullets) {
        Bullet& bullet = *bullet_ptr;

        if (!bullet.isActive())
            continue;

        bullet.update(dt);

        if (!contains(bullet.getPosition().x, bullet.getPosition().y)) {
            bullet.deactivate();
            continue;
        }

        for (const auto& entity_ptr : entities) {
            Entity& entity = *entity_ptr;
            if (bullet.collision(entity)) {
                was_collision = true;
                collision_coords.push_back(bullet.getPosition());
                entity.takeDamage(bullet.getDamage());
                bullet.deactivate();
                break;
            }
        }
        
        if (!bullet.isActive())
            continue;

        for (const auto& enemy_ptr : enemies[NORMAL]) {
            Entity& enemy = *enemy_ptr;
            if (bullet.collision(enemy)) {
                was_collision = true;
                collision_coords.push_back(bullet.getPosition());
                enemy.takeDamage(bullet.getDamage());
                bullet.deactivate();
                break;
            }
        }
    }

    for (size_t i = 0; i < bullets.size(); ++i) {
        Bullet& bullet_A = *bullets[i];
        if (!bullet_A.isActive()) continue;

        for (size_t j = i + 1; j < bullets.size(); ++j) {
            Bullet& bullet_B = *bullets[j];
            if (!bullet_B.isActive()) continue;

            if (bullet_A.collision(bullet_B)) {
                //was_collision = true;
                bullet_A.takeDamage(bullet_B.getDamage());
                bullet_B.takeDamage(bullet_A.getDamage());
            }
        }
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<Bullet>& bullet_ptr) {
                return !bullet_ptr->isActive();
            }),
        bullets.end()
    );
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [this, dt](std::unique_ptr<Entity> const& entity_ptr) {
                Entity& entity = *entity_ptr;
                entity.update(dt);
                if (!entity.isAlive()) {
                    if (entity.getCurrentHealth() <= 0)
                        points_to_give += entity.getPointsWorth();
                    
                    return true;
                }
                return false;
            }),
        entities.end()
    );
    enemies[NORMAL].erase(
        std::remove_if(enemies[NORMAL].begin(), enemies[NORMAL].end(),
            [this, dt](std::unique_ptr<Entity> const& enemy_ptr) {
                Entity& enemy = *enemy_ptr;
                enemy.update(dt);
                if (!enemy.isAlive()) {
                    if (enemy.getCurrentHealth() <= 0)
                        points_to_give += enemy.getPointsWorth();

                    return true;
                }
                return false;
            }),
        enemies[NORMAL].end()
    );
}


bool PlayArea::contains(float x, float y) const {
    return bounds.contains(x, y);
}

bool PlayArea::contains(Rect const& rectHitbox) {
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

std::vector<std::unique_ptr<Bullet>> const& PlayArea::getBullets() const { return bullets; }

void PlayArea::addEntity(std::unique_ptr<Entity> entity) { entities.push_back(std::move(entity)); }

std::vector<std::unique_ptr<Entity>> const& PlayArea::getEntities() const { return entities; }

std::vector<std::unique_ptr<Entity>> const& PlayArea::getEnemies(EnemyType type) const { 
    static const std::vector<std::unique_ptr<Entity>> empty;

    auto it = enemies.find(type);
    if (it != enemies.end()) {
        return it->second;
    }
    return empty;
};

void PlayArea::addEnemy(EnemyType type, std::unique_ptr<Entity> enemy) { 
    enemies[type].push_back(std::move(enemy));
}

bool PlayArea::wasCollision() const { return was_collision; }

std::vector<Vector2> const PlayArea::getCollisionCoords() const { return collision_coords; }

size_t PlayArea::getPointsToGive() const { return points_to_give; }