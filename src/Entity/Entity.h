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

    bool collision(Collidable& player) override;

    const float getCollisionDamage() const override;

    Vector2 getSize() const;
    Vector2 getPosition() const;

    void update(float dt);

    float getMaxHealth() const;

    float getCurrentHealth() const;
    void setCurrentHealth(float new_health);

    void setScript(std::unique_ptr<Script> script);

    bool isAlive();

private:
    
    float max_health;
    float current_health;

    Vector2 position;
    Vector2 size;

    std::unique_ptr<Script> script;

    std::unique_ptr<Hitbox> hitbox;

};

#endif //ENTITY_H