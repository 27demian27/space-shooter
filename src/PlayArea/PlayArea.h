#ifndef PLAYAREA_H
#define PLAYAREA_H

#include <vector>

#include "../Utils/Rect.h"
#include "../Bullet/Bullet.h"

class PlayArea {

public:

    PlayArea() = default;

    PlayArea(int width, int height);

    bool contains(float x, float y);

    void clipToArea(float& x, float& y);

    void update(float dt);

    void addBullet(Bullet& bullet);
    std::vector<Bullet> getBullets();

private:
    Rect bounds;
    std::vector<Bullet> bullets;
};

#endif //PLAYAREA_H