#include <cmath>
#include <iostream>

#include "NormalEnemy.h"

NormalEnemy::NormalEnemy(
        Vector2 position,
        Vector2 size,
        float max_health,
        float collision_damage,
        std::unique_ptr<Script> script,
        HitboxShape hitboxShape,
        Vector2 const& player_pos,
        PlayArea& playArea)
: Entity(position, size, max_health, collision_damage, std::move(script), hitboxShape),
  player_pos(player_pos), playArea(playArea)
{

    attack_speed = 3.0f;
    remaining_attack_cooldown = 0.0f;
    can_shoot = true;
    bullet_damage = 5.0f;
    bullet_speed = 400.0f;
    rotation = 0.0f;
    bullet_size = {5, 5};

};

void NormalEnemy::update(float dt) {
    Vector2 direction = script->update(position, dt);


    position.x += direction.x * script->getCurrentMoveSpeed() * dt;
    position.y += direction.y * script->getCurrentMoveSpeed() * dt;

    last_taken_dmg_ago += dt;

    aim_direction = (player_pos - position).normalized();
    //std::cout << "{" << aim_direction.x << ", " << aim_direction.y << "}";
    float angle_radians = atan2(aim_direction.y, aim_direction.x);
    rotation = angle_radians * 180.0f / M_PI + 90.0f;

    if (remaining_attack_cooldown <= 0) {
        can_shoot = true;
    } else {
        can_shoot = false;
        remaining_attack_cooldown = remaining_attack_cooldown - dt;
    }

    shoot();

    updateHitbox(position, size);
}

bool NormalEnemy::shoot() { 
    if (!can_shoot) return false;

    float bullet_health = 2.0f;

    playArea.addBullet(std::make_unique<Bullet>(
        bullet_speed,
        bullet_damage,
        rotation,
        position.x + aim_direction.x * (size.x - 15.0f),
        position.y + aim_direction.y * (size.y - 15.0f), 
        bullet_size,
        bullet_health,
        false,
        CIRCLE
    ));


    remaining_attack_cooldown = 1.0f / attack_speed;
    return true;
}

float NormalEnemy::getRotation() { return rotation; }
float NormalEnemy::getBulletSpeed() {return bullet_speed; }
float NormalEnemy::getBulletDamage() { return bullet_damage; }