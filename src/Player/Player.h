#ifndef PLAYER_H
#define PLAYER_H

#include "../Utils/Vector2.h"
#include "../Utils/ShipType.h"
#include "../Shooter.h"
#include "../Collidable.h"
#include "../PlayArea/PlayArea.h"

class Player : public Shooter, public Collidable {

public:
    Player() = default;

    Player(PlayArea& playArea);

    bool shoot() override;

    void setIsBoosting(bool is_boosting);
    float getRemainingBoost() const;
    float getBoostCapacity() const;

    Vector2 getVelocity();
    void setVelocity(Vector2 velocity);

    float getRotation() override;

    void setRotation(Vector2 mousePos);
    void setRotation(float rotation);

    float getX();
    float getY();

    Vector2 getSize();
    void setSize(Vector2 size);

    const float getCollisionDamage() const override;

    bool collision(Collidable& entity) override;

    float lastTakenDamage() const;

    float getKnockbackDowntime();

    const Vector2& getPosition() const;
    void setPosition(Vector2 position);

    float getMoveSpeed();
    float getBulletSpeed() override;
    float getBulletDamage() override;

    void update(float dt);

    float getMaxHealth() const;
    float getCurrentHealth() const;

    size_t getScore() const;

private:

    PlayArea& playArea;

    float max_health;
    float current_health;

    size_t score;

    float knockback_downtime;

    Vector2 size;

    Vector2 position;
    Vector2 velocity;

    float move_speed;
    float decellaration_rate;

    bool is_boosting;
    float boost_speed;
    float boost_capacity;
    float boost_remaining;
    float boost_regen_rate;
    bool boost_on_cd;

    float last_taken_dmg_ago;

    ShipType ship_type;
};

#endif //PLAYER_H