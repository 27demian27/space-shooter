#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "Player.h"

Player::Player(PlayArea& playArea) : playArea(playArea) {

    max_health = 100;
    current_health = max_health;

    size = Vector2({100, 100});
    hitbox = Rect();

    remaining_attack_cooldown = 0;
    can_shoot = true;
    attack_speed = 2.5f;

    bullet_damage = 20;
    bullet_speed = 600;
    collision_damage = 50;

    position = {0, 0};
    velocity = {0, 0};
    rotation = 0;

    move_speed = 300.0f;
    decellaration_rate = 100.0f;

    ship_type = ShipType::FROG;

}

bool Player::shoot() {
    if (!can_shoot) return false;

    float theta = rotation * (M_PI / 180.0f);

    switch (ship_type) {
        case ShipType::FROG: {
            Vector2 left_local_offset  = {-size.x / 4.0f, -size.y / 3.0f};
            Vector2 right_local_offset = { size.x / 4.0f, -size.y / 3.0f};

            Vector2 left_world_offset  = rotate(left_local_offset, theta);
            Vector2 right_world_offset = rotate(right_local_offset, theta);

            Bullet bullet1(*this, position.x + left_world_offset.x, position.y + left_world_offset.y);
            Bullet bullet2(*this, position.x + right_world_offset.x, position.y + right_world_offset.y);

            playArea.addBullet(bullet1);
            playArea.addBullet(bullet2);
            break;
        }
        default: {
            Vector2 forward = rotate({0.0f, -10.0f}, theta);
            Bullet bullet(*this, position.x + forward.x, position.y + forward.y);
            playArea.addBullet(bullet);
            break;
        }
    }

    remaining_attack_cooldown = 1.0f / attack_speed;
    return true;
}



void Player::updateHitbox() {
    float hitbox_width = size.x / 2.0f;
    float hitbox_height = size.y / 2.0f;

    hitbox.TL = {position.x - hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};
    hitbox.BL = {position.x - hitbox_width / 2.0f, position.y + hitbox_height / 2.0f};
    hitbox.TR = {position.x + hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};
    hitbox.BR = {position.x - hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};
}

void Player::update(float dt) {
    position.x = position.x + velocity.x * dt;
    position.y = position.y + velocity.y * dt;
    
    playArea.clipToArea(position.x, position.y);

    if (velocity.x > 0) {
        velocity.x = velocity.x - decellaration_rate * dt;
    } 
    else if (velocity.x < 0) {
        velocity.x = velocity.x + decellaration_rate * dt;
    }
    if (velocity.y > 0) {
        velocity.y = velocity.y - decellaration_rate * dt;
    } 
    else if(velocity.y < 0) {
        velocity.y = velocity.y + decellaration_rate * dt;
    }


    if (remaining_attack_cooldown <= 0) {
        can_shoot = true;
    } else {
        can_shoot = false;
        remaining_attack_cooldown = remaining_attack_cooldown - dt;
    }

    updateHitbox();

}

float Player::getX() { return position.x; }
float Player::getY() { return position.y; }

float Player::getMoveSpeed() { return move_speed; }
float Player::getBulletSpeed() { return bullet_speed; }

Vector2 Player::getVelocity() { return velocity; }

void Player::setVelocity(Vector2 velocity) { 
    if (velocity.x != 0.f && velocity.y != 0.f) {
        velocity.x *= 0.7071f;
        velocity.y *= 0.7071f;
    }
    velocity.x *= move_speed;
    velocity.y *= move_speed;

    this->velocity = velocity;
    
}

void Player::setPosition(Vector2 position) { this->position = position; }

float Player::getRotation() { return rotation; }

void Player::setRotation(float rotation) { this->rotation = rotation; }

/**
 * @brief Calculates rotation from a 2D mouse position vector.
 * 
 * @param mousePos 2D position vector used to calculate new angle.
 */
void Player::setRotation(Vector2 mousePos) {
    float dx = mousePos.x - position.x;
    float dy = mousePos.y - position.y;
    rotation = std::atan2(dy, dx) * (180.0f / M_PI) + 90.0f;
}

Vector2 Player::getSize() { return size; }

Rect Player::getHitbox() { return hitbox; }

float Player::getMaxHealth() { return max_health; }
float Player::getCurrentHealth() { return current_health; }