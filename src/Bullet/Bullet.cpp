#define _USE_MATH_DEFINES

#include "Bullet.h"

#include <cmath>

Bullet::Bullet(
    Shooter& shooter, 
    float start_x, 
    float start_y, 
    Vector2 size, 
    float max_health, 
    bool is_from_player
)
 : Collidable(createHitbox({start_x, start_y}, size, HitboxShape::CIRCLE)),
   position({start_x, start_y}),
   is_active(true),
   size(size),
   max_health(max_health),
   curr_health(max_health),
   is_from_player(is_from_player),
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

    if (curr_health <= 0) {
        is_active = false;
    }

    updateHitbox(position, size);
}

Vector2 Bullet::getPosition() { return position; }
Vector2 Bullet::getPosition() const { return position; }

float Bullet::getDamage() const { return shooter.getBulletDamage(); }

bool Bullet::isActive() { return is_active; }
void Bullet::deactivate() { is_active = false; }

bool Bullet::isFromPlayer() const { return is_from_player; }

void Bullet::takeDamage(float damage) { curr_health -= damage; }

const float Bullet::getCollisionDamage() const { return collision_damage; }