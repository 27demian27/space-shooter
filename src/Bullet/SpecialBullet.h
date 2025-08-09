#ifndef SPECIALBULLET_H
#define SPECIALBULLET_H

#include "Bullet.h"

class SpecialBullet : public Bullet {

public:
    SpecialBullet(
        float speed,
        float damage,
        float shooter_rotation,
        float start_x, 
        float start_y, 
        Vector2 size, 
        float max_health, 
        bool is_from_player,
        Vector2 growth_rate
    );

    void update(float dt) override;

protected:
    Vector2 growth_rate;
};

#endif // SPECIALBULLET_H