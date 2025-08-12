#ifndef PLAYAREA_H
#define PLAYAREA_H

#include <vector>
#include <map>
#include <memory>

#include "../Utils/Rect.h"
#include "../Bullet/Bullet.h"
#include "../Entities/Entity.h"
#include "../EnemyTypes.h"

class PlayArea {

public:

    PlayArea() = default;

    PlayArea(int width, int height);

    bool contains(float x, float y) const;
    bool contains(const Rect& rectHitbox);

    void clipToArea(float& x, float& y);

    void update(float dt);

    void addBullet(std::unique_ptr<Bullet> bullet);
    std::vector<std::unique_ptr<Bullet>> const& getBullets() const;

    void addEntity(std::unique_ptr<Entity> entity);
    std::vector<std::unique_ptr<Entity>> const& getEntities() const;

    std::vector<std::unique_ptr<Entity>> const& getEnemies(EnemyType type) const;
    void addEnemy(EnemyType type, std::unique_ptr<Entity> enemy);

    bool wasCollision() const;

    std::vector<Vector2> const getCollisionCoords() const;

    size_t getPointsToGive() const;

private:
    Rect bounds;

    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Entity>> entities;
    std::map<EnemyType, std::vector<std::unique_ptr<Entity>>> enemies;

    std::vector<Vector2> collision_coords;

    size_t points_to_give;
    bool was_collision;
};

#endif //PLAYAREA_H