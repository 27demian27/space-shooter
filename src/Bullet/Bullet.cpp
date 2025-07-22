#define _USE_MATH_DEFINES

#include "Bullet.h"

#include <cmath>

Bullet::Bullet(Shooter& shooter, float start_x, float start_y)
 : shooter(shooter) {
    float theta = (shooter.getRotation() - 90.0f) * (M_PI / 180.0f);
    direction = Vector2({std::cos(theta), std::sin(theta)});
    position = Vector2({start_x, start_y});
    speed = shooter.getBulletSpeed();
    collision_damage = shooter.getBulletDamage();
}

bool Bullet::collision(Collidable& other) {
    return (other.getHitbox().contains(position.x, position.y));
}

void Bullet::updateHitbox() {
    hitbox.TL = position;
    hitbox.BL = position;
    hitbox.TR = position;
    hitbox.BR = position;
}

void Bullet::update(float dt) {
    position.x = position.x + direction.x * speed * dt;
    position.y = position.y + direction.y * speed * dt;

    updateHitbox();
}

Vector2 Bullet::getPosition() { return position; }
Vector2 Bullet::getPosition() const { return position; }

float Bullet::getDamage() const { return shooter.getBulletDamage(); }
Rect Bullet::getHitbox() { return Rect({position, position, position, position}); }