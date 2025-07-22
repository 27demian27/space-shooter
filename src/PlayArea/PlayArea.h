#ifndef PLAYAREA_H
#define PLAYAREA_H

#include <vector>
#include <memory>

#include "../Utils/Rect.h"
#include "../Bullet/Bullet.h"
#include "../Entity/Entity.h"

class PlayArea {

public:

    PlayArea() = default;

    PlayArea(int width, int height);

    bool contains(float x, float y);

    void clipToArea(float& x, float& y);

    void update(float dt);

    void addBullet(std::unique_ptr<Bullet> bullet);
    const std::vector<std::unique_ptr<Bullet>>& getBullets() const;

    void addEntity(Entity& entity);

    std::vector<Entity>& getEntities();

    const std::vector<Entity>& getEntities() const;

private:
    Rect bounds;

    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<Entity> entities;
};

#endif //PLAYAREA_H