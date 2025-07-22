#ifndef ENTITY_H
#define ENTITY_H

#include "../Utils/Vector2.h"
#include "../Utils/Rect.h"
#include "../Collidable.h"

class Entity : public Collidable {

public:

    Entity(Vector2 position, Vector2 size, float collision_damage);

    bool collision(Collidable& player) override;

    Rect getHitbox() override;

    Vector2 getSize() const;
    Vector2 getPosition() const;

    void update(float dt);

    float getMaxHealth() const;

    float getCurrentHealth() const;
    void setCurrentHealth(float new_health);

private:
    void updateHitbox();
    
    float max_health;
    float current_health;

    Vector2 position;
    Vector2 size;

    Rect hitbox;

};

#endif //ENTITY_H