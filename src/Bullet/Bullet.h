#ifndef BULLET_H
#define BULLET_H

#include "../Utils/Vector2.h"
#include "../Shooter.h"
#include "../Collidable.h"

class Bullet : public Collidable {

public:
    Bullet(
        Shooter& shooter, 
        float start_x, 
        float start_y, 
        Vector2 size, 
        float max_health, 
        bool is_from_player
    );

    void update(float dt);

    Vector2 getPosition() const;

    bool collision(Collidable& other) override;

    const float getCollisionDamage() const override;

    float getDamage() const;

    Vector2 getPosition();
    
    bool isActive();
    void deactivate();

    bool isFromPlayer() const;

    void takeDamage(float damage);

private:

    Shooter& shooter;

    float speed;

    Vector2 position;
    Vector2 size;
    
    Vector2 direction;

    bool is_active;
    bool is_from_player;

    float max_health;
    float curr_health;
};

#endif //BULLET_H