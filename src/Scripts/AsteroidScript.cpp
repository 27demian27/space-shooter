#include "AsteroidScript.h"

#include <iostream>

AsteroidScript::AsteroidScript(Vector2 begin_pos, Vector2 end_pos) {
    is_playing = true; 
    elapsed_time = 0.0f;

    checkpoints.push(end_pos);
    current_direction = (end_pos - begin_pos).normalized();
    current_move_speed = 400.0f;
}

void AsteroidScript::play() {
    is_playing = true;
}

Vector2 AsteroidScript::update(Vector2 entity_pos, float dt) {
    elapsed_time += dt;
    const float epsilon = 10.0f;
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