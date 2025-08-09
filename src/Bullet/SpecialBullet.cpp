
#include "SpecialBullet.h"

SpecialBullet::SpecialBullet(
    float speed,
    float damage,
    float shooter_rotation,
    float start_x, 
    float start_y, 
    Vector2 size, 
    float max_health, 
    bool is_from_player,
    Vector2 growth_rate
)
 : Bullet(
    speed, 
    damage, 
    shooter_rotation, 
    start_x, start_y, 
    size, max_health, 
    is_from_player, 
    RECT
  ),
  growth_rate(growth_rate)
{}

void SpecialBullet::update(float dt) {
    position.x = position.x + direction.x * speed * dt;
    position.y = position.y + direction.y * speed * dt;

    if (curr_health <= 0) {
        is_active = false;
    }

    size.x = size.x + growth_rate.x * dt;
    size.y = size.y + growth_rate.y * dt;

    updateHitbox(position, size);
}