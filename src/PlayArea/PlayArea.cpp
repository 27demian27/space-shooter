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
                entity.takeDamage(bullet.getDamage());
                bullet.deactivate();
                break;
            }
        }
        for (const auto& enemy_ptr : enemies[EnemyType::NORMAL]) {
            Entity& enemy = *enemy_ptr;
            if (bullet.collision(enemy)) {
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
                    return true;
                }
                return false;
            }),
        entities.end()
    );
    enemies[EnemyType::NORMAL].erase(
        std::remove_if(enemies[EnemyType::NORMAL].begin(), enemies[EnemyType::NORMAL].end(),
            [this, dt](std::unique_ptr<Entity> const& enemy_ptr) {
                Entity& enemy = *enemy_ptr;
                enemy.update(dt);
                if (!enemy.isAlive()) {
                    return true;
                }
                return false;
            }),
        enemies[EnemyType::NORMAL].end()
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

std::vector<std::unique_ptr<Entity>> const& PlayArea::getEnemies(EnemyType type) const { return enemies.at(type); };
void PlayArea::addEnemy(EnemyType type, std::unique_ptr<Entity> enemy) { 
    enemies[type].push_back(std::move(enemy));
}