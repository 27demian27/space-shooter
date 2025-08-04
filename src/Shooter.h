#ifndef SHOOTER_H
#define SHOOTER_H

class Shooter {

public:
    virtual ~Shooter() = default;
       
    virtual bool shoot() = 0;
    
    virtual float getRotation() = 0;
    virtual float getBulletSpeed() = 0;
    virtual float getBulletDamage() = 0;


protected:
    float attack_speed;
    float remaining_attack_cooldown;
    bool can_shoot;

    float bullet_damage;
    float bullet_speed;
    Vector2 bullet_size;

    float rotation;
};

#endif //SHOOTER_H