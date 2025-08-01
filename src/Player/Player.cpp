#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "Player.h"

#define START_POS {0, 0}
#define SIZE {100, 100}
#define HITBOX_SIZE {50, 50}
#define MAX_HEALTH 100
#define BASE_MS 300.0f
#define BOOST_MS 500.0f

Player::Player(PlayArea& playArea)
 : Collidable(createHitbox(START_POS, HITBOX_SIZE, HitboxShape::RECT)),
   position(START_POS),
   size(SIZE),
   playArea(playArea),
   max_health(MAX_HEALTH),
   current_health(MAX_HEALTH),
   knockback_downtime(0),
   velocity({0, 0}),
   move_speed(BASE_MS),
   boost_speed(BOOST_MS),
   decellaration_rate(100.0f),
   is_boosting(false),
   boost_on_cd(false),
   boost_capacity(2.0f),
   boost_remaining(boost_capacity),
   boost_regen_rate(0.3f),
   ship_type(ShipType::FROG)
{
   rotation = 0.0f,
   remaining_attack_cooldown = 0.0f;
   can_shoot = true;
   attack_speed = 2.5f;
   bullet_damage = 10.0f;
   bullet_speed = 600.0f;
   collision_damage = 20.0f;

}

bool Player::shoot() {
    if (!can_shoot) return false;

    float theta = rotation * (M_PI / 180.0f);

    switch (ship_type) {
        case ShipType::FROG: {
            bullet_damage = 5.0f;
            bullet_size = {20.0f, 20.0f};
            
            Vector2 left_local_offset  = {-size.x / 4.0f, -size.y / 3.0f};
            Vector2 right_local_offset = { size.x / 4.0f, -size.y / 3.0f};

            Vector2 left_world_offset  = rotate(left_local_offset, theta);
            Vector2 right_world_offset = rotate(right_local_offset, theta);

            playArea.addBullet(std::make_unique<Bullet>(
                *this, 
                position.x + left_world_offset.x, 
                position.y + left_world_offset.y, 
                bullet_size
            ));
            playArea.addBullet(std::make_unique<Bullet>(
                *this, 
                position.x + right_world_offset.x, 
                position.y + right_world_offset.y, 
                bullet_size
            ));
            break;
        }

    }

    remaining_attack_cooldown = 1.0f / attack_speed;
    return true;
}

bool Player::collision(Collidable& entity) {
    if (!hitbox) {
        std::cerr << "Error: Player hitbox is null!\n";
        return false;
    }

    const Hitbox& entity_hitbox = entity.getHitbox();

    const Rect* player_hitbox = dynamic_cast<const Rect*>(hitbox.get());

    if (!player_hitbox) {
        std::cerr << "Error: Hitbox dynamic_cast failed, player not a Rect.\n";
        return false;
    }
    
    return player_hitbox->intersects(entity_hitbox);
}

void Player::update(float dt) {
    updateHitbox(position, HITBOX_SIZE);

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

    if (knockback_downtime > 0) {
        knockback_downtime -= dt;
    }

    if (is_boosting && boost_remaining > 0 && !boost_on_cd) {
        boost_remaining -= dt;
    }
    boost_remaining += boost_regen_rate * dt;
    if (boost_remaining > boost_capacity) {
        boost_remaining = boost_capacity;
    }
    if (boost_remaining <= 0) {
        boost_remaining = 0;
        boost_on_cd = true;
    } else if (boost_remaining >= 0.2f) {
        boost_on_cd = false;
    }

    if (remaining_attack_cooldown <= 0) {
        can_shoot = true;
    } else {
        can_shoot = false;
        remaining_attack_cooldown = remaining_attack_cooldown - dt;
    }

    for (const auto& entity_ptr : playArea.getEntities()) {
        Entity& entity = *entity_ptr;
        if (collision(entity)) {
            std::cout << "Hit entity\n";
            Vector2 knockback_direction(
                {
                    position.x - entity.getPosition().x, 
                    position.y - entity.getPosition().y
                });
            knockback_direction = knockback_direction.normalized();

            float knockback_speed = 400.0f;

            velocity.x = knockback_direction.x * knockback_speed;
            velocity.y = knockback_direction.y * knockback_speed;

            knockback_downtime = 0.25f;

            current_health -= entity.getCollisionDamage();
            entity.setCurrentHealth(entity.getCurrentHealth() - collision_damage);
        }
    }


}

float Player::getX() { return position.x; }
float Player::getY() { return position.y; }

float Player::getMoveSpeed() { return move_speed; }
float Player::getBulletSpeed() { return bullet_speed; }
float Player::getBulletDamage() { return bullet_damage; }

void Player::setIsBoosting(bool is_boosting) { this->is_boosting = is_boosting; }
float Player::getRemainingBoost() { return boost_remaining; }
float Player::getBoostCapacity() { return boost_capacity; }

Vector2 Player::getVelocity() { return velocity; }

void Player::setVelocity(Vector2 velocity) { 
    if (velocity.x != 0.f && velocity.y != 0.f) {
        velocity.x *= 0.7071f;
        velocity.y *= 0.7071f;
    }
    if (is_boosting && boost_remaining > 0.0f && !boost_on_cd) {
        velocity.x *= boost_speed;
        velocity.y *= boost_speed;
    } else {
        velocity.x *= move_speed;
        velocity.y *= move_speed;
    }

    this->velocity = velocity;
    
}

const Vector2 Player::getPosition() const { return position; }

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
void Player::setSize(Vector2 size) { this->size = size; }

const float Player::getCollisionDamage() const { return collision_damage; }

float Player::getKnockbackDowntime() { return knockback_downtime; }

float Player::getMaxHealth() { return max_health; }
float Player::getCurrentHealth() { return current_health; }
