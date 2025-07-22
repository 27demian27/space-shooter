#include "Entity.h"

Entity::Entity(Vector2 position, Vector2 size, float collision_damage)
 : position(position), size(size) {
    this->collision_damage = collision_damage;
    updateHitbox();
    max_health = 100.0f;
    current_health = max_health;
}

bool Entity::collision(Collidable& player) {
    return player.collision(*this);
}

void Entity::updateHitbox() {
    float hitbox_width = size.x;
    float hitbox_height = size.y;

    hitbox.TL = {position.x - hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};
    hitbox.BL = {position.x - hitbox_width / 2.0f, position.y + hitbox_height / 2.0f};
    hitbox.TR = {position.x + hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};
    hitbox.BR = {position.x - hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};

}

void Entity::update(float dt) {

    // change pos, blablabla
    updateHitbox();
}

Rect Entity::getHitbox() { return hitbox; }

Vector2 Entity::getSize() const { return size; }
Vector2 Entity::getPosition() const { return position; }

float Entity::getCurrentHealth() const { return current_health; }
float Entity::getMaxHealth() const { return max_health; }

void Entity::setCurrentHealth(float new_health) { current_health = new_health; }