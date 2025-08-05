#ifndef SHOOTER_H
#define SHOOTER_H

enum ShootingMode {
    NORMAL_ATTACK,
    SPECIAL_ATTACK
};

class Shooter {

public:
    virtual ~Shooter() = default;
       
    virtual bool shoot() = 0;
    
    virtual float getRotation() = 0;
    virtual float getBulletSpeed() = 0;
    virtual float getBulletDamage() = 0;

    void setShootingMode(ShootingMode mode) { this->shootingMode = mode; }
    float getRemainingSpecialAttackCooldown() { return remaining_special_attack_cooldown; }

protected:
    float attack_speed;
    float remaining_attack_cooldown;

    float special_attack_cooldown;
    float remaining_special_attack_cooldown;

    bool can_shoot;

    float bullet_damage;
    float bullet_speed;
    Vector2 bullet_size;

    float rotation;

    ShootingMode shootingMode;
};

#endif //SHOOTER_H