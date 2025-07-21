#ifndef PLAYER_H
#define PLAYER_H

#include "../Utils/Vector2.h"
#include "../Utils/ShipType.h"
#include "../Shooter.h"
#include "../PlayArea/PlayArea.h"

class Player : public Shooter {

public:
    Player() = default;

    Player(PlayArea& playArea);

    bool shoot() override;

    Vector2 getVelocity();
    void setVelocity(Vector2 velocity);

    float getRotation() override;

    void setRotation(Vector2 mousePos);
    void setRotation(float rotation);

    float getX() override;
    float getY() override;

    Vector2 getSize();

    Rect getHitbox();

    void setPosition(Vector2 position);

    float getMoveSpeed();
    float getBulletSpeed() override;

    void update(float dt);

    float getMaxHealth();
    float getCurrentHealth();
    
private:
    void updateHitbox();

    PlayArea& playArea;

    float max_health;
    float current_health;

    float collision_damage;

    Vector2 size;
    Rect hitbox;

    Vector2 position;
    Vector2 velocity;

    float move_speed;
    float decellaration_rate;

    ShipType ship_type;

    enum ShootingMode {
        NORMAL,
        SPECIAL
    };
};

#endif //PLAYER_H