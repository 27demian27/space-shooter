#ifndef BULLET_H
#define BULLET_H

#include "Shooter.h"
#include "Vector2.h"

class Bullet {

public:

    Bullet(Shooter& shooter, float start_x, float start_y);

    void update(float dt);

    Vector2 getPosition();

private:

    float speed;

    Vector2 position;
    Vector2 direction;
};

#endif //BULLET_H