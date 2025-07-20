#define _USE_MATH_DEFINES

#include <cmath>

#include "Player.h"

Player::Player() {
    max_health = 100;
    current_health = max_health;

    remaining_attack_cooldown = 0;
    can_shoot = true;
    attack_speed = 1.5;

    bullet_damage = 20;
    collision_damage = 50;

    position = {0, 0};
    velocity = {0, 0};
    rotation = 0;
}

bool Player::shoot() {
    if (can_shoot) {
        //shoot logic (spawn bullet)
        return true;
    }
    
    return false;
}

void Player::update(float dt) {
    position.x = position.x + velocity.x * dt;
    position.y = position.y + velocity.y * dt;

    if (remaining_attack_cooldown <= 0) {
        can_shoot = false;
        remaining_attack_cooldown = attack_speed;
    } else {
        remaining_attack_cooldown = remaining_attack_cooldown - dt;
    }


}

Vector2 Player::getVelocity()  { return velocity; }

void Player::setVelocity(Vector2 velocity) { this->velocity = velocity; }

float Player::getRotation() { return rotation; }

void Player::setRotation(float rotation) { this->rotation = rotation; }

void Player::setRotation(Vector2 mousePos) {
    float dx = mousePos.x - position.x;
    float dy = mousePos.y - position.y;
    rotation = std::atan2(dy, dx) * (180.0f / M_PI) + 90.0f;
}