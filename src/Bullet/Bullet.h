#ifndef BULLET_H
#define BULLET_H

#include "../Utils/Vector2.h"
#include "../Shooter.h"
#include "../Collidable.h"

class Bullet : public Collidable {

public:
    Bullet(Shooter& shooter, float start_x, float start_y, float size);

    void update(float dt);

    Vector2 getPosition() const;

    bool collision(Collidable& other) override;

    const float getCollisionDamage() const override;

    float getDamage() const;

    Vector2 getPosition();
    

private:

    Shooter& shooter;

    float speed;

    Vector2 position;
    Vector2 size;
    
    Vector2 direction;
};

#endif //BULLET_H