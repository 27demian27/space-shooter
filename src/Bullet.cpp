#define _USE_MATH_DEFINES

#include "Bullet.h"

#include <cmath>

Bullet::Bullet(Shooter& shooter, float start_x, float start_y) {
    float theta = (shooter.getRotation() - 90.0f) * (M_PI / 180.0f);
    direction = Vector2({std::cos(theta), std::sin(theta)});
    position = Vector2({start_x, start_y});
    speed = shooter.getBulletSpeed();
}

void Bullet::update(float dt) {
    position.x = position.x + direction.x * speed * dt;
    position.y = position.y + direction.y * speed * dt;
}

Vector2 Bullet::getPosition() { return position; }