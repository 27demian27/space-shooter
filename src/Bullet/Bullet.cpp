#define _USE_MATH_DEFINES

#include "Bullet.h"

#include <cmath>

Bullet::Bullet(Shooter& shooter, float start_x, float start_y, Vector2 size)
 : Collidable(createHitbox({start_x, start_y}, size, HitboxShape::CIRCLE)),
   position({start_x, start_y}),
   size(size),
   shooter(shooter) 
{
    float theta = (shooter.getRotation() - 90.0f) * (M_PI / 180.0f);
    direction = Vector2({std::cos(theta), std::sin(theta)});
    speed = shooter.getBulletSpeed();
    collision_damage = shooter.getBulletDamage();
}

bool Bullet::collision(Collidable& other) {
    return (other.getHitbox().intersects(getHitbox()));
}

void Bullet::update(float dt) {
    position.x = position.x + direction.x * speed * dt;
    position.y = position.y + direction.y * speed * dt;

    updateHitbox(position, size);
}

Vector2 Bullet::getPosition() { return position; }
Vector2 Bullet::getPosition() const { return position; }

float Bullet::getDamage() const { return shooter.getBulletDamage(); }

const float Bullet::getCollisionDamage() const { return collision_damage; }