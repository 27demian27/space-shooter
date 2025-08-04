
#include "Script.h"

#define CHECKPOINT_PROXIMITY_BUFFER 5.0f

Script::Script()
 : is_playing(true), elapsed_time(0.0f)
{}

/**
 * @brief Construct a new Script for an entity in a stationary position.
 * 
 * @param statonary_pos The position of the entity.
 */
Script::Script(Vector2 statonary_pos)
 : Script() {
    checkpoints.push(statonary_pos);
    is_stationary = true;
}

Vector2 Script::update(Vector2 entity_pos, float dt) {
    elapsed_time += dt;

    if (is_stationary) {
        return Vector2({0, 0});
    }

    const float epsilon = CHECKPOINT_PROXIMITY_BUFFER;
    if ((entity_pos - checkpoints.front()).length() < epsilon) {
        checkpoints.pop();
    }
    if (checkpoints.empty()) {
        is_playing = false;
        return Vector2({0, 0});
    }

    current_direction = (checkpoints.front() - entity_pos).normalized();

    return current_direction;
}

float Script::getCurrentMoveSpeed() { return current_move_speed; }

void Script::play() { is_playing = true; }

bool Script::isPlaying() { return is_playing; }

void Script::addCheckpoint(Vector2 position) {
    checkpoints.push(position);
}
void Script::setCurrentCheckpoint(Vector2 position) {
    checkpoints.front() = position;
}