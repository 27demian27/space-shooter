#include "Entity.h"

Entity::Entity(
    Vector2 position, 
    Vector2 size, 
    float max_health, 
    float collision_damage, 
    std::unique_ptr<Script> script,
    HitboxShape hitboxShape
    )
 : Collidable(createHitbox(position, size, hitboxShape)),
   position(position), 
   size(size), 
   max_health(max_health), 
   script(std::move(script))
{
    this->collision_damage = collision_damage;
    current_health = max_health;
    updateHitbox(position, size);
}

bool Entity::collision(Collidable& player) {
    return player.collision(*this);
}

void Entity::update(float dt) {
    
    Vector2 direction = script->update(position, dt);

    position.x += direction.x * script->getCurrentMoveSpeed() * dt;
    position.y += direction.y * script->getCurrentMoveSpeed() * dt;

    updateHitbox(position, size);
}

const float Entity::getCollisionDamage() const { return collision_damage; }

Vector2 Entity::getSize() const { return size; }
Vector2 Entity::getPosition() const { return position; }

float Entity::getCurrentHealth() const { return current_health; }
float Entity::getMaxHealth() const { return max_health; }

void Entity::setCurrentHealth(float new_health) { current_health = new_health; }

void Entity::setScript(std::unique_ptr<Script> script) { this->script = std::move(script); }

bool Entity::isAlive() {
    return (script->isPlaying() && current_health > 0);
}