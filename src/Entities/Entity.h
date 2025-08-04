#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "../Scripts/Script.h"
#include "../Utils/Vector2.h"
#include "../Utils/Hitbox.h"
#include "../Collidable.h"

class Entity : public Collidable {

public:

    Entity(
        Vector2 position, 
        Vector2 size, 
        float max_health, 
        float collision_damage, 
        std::unique_ptr<Script> script,
        HitboxShape hitboxShape
        );

    virtual ~Entity() = default;

    bool collision(Collidable& player) override;

    const float getCollisionDamage() const override;

    Vector2 getSize() const;
    Vector2 getPosition() const;

    virtual void update(float dt);

    float getMaxHealth() const;

    float getCurrentHealth() const;
    void takeDamage(float damage);
    float lastTakenDamage() const;

    void setScript(std::unique_ptr<Script> script);

    bool isAlive();

protected:
    
    float max_health;
    float current_health;

    float last_taken_dmg_ago;

    Vector2 position;
    Vector2 size;

    std::unique_ptr<Script> script;

    std::unique_ptr<Hitbox> hitbox;

};

#endif //ENTITY_H