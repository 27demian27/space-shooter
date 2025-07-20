#ifndef PLAYER
#define PLAYER

#include "Vector2.h"

class Player {

public:

    Player();

    bool shoot();

    Vector2 getVelocity();
    void setVelocity(Vector2 velocity);

    float getRotation();
    void setRotation(Vector2 mousePos);
    void setRotation(float rotation);

    float getX();
    float getY();

    float getMoveSpeed();

    void update(float dt);

private:
    float max_health;
    float current_health;

    float attack_speed;
    float remaining_attack_cooldown;
    bool can_shoot;

    float bullet_damage;
    float collision_damage;

    float rotation;

    Vector2 position;
    Vector2 velocity;

    float move_speed;
    float decellaration_rate;

};

#endif //PLAYER