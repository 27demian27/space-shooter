#include "FollowPlayerScript.h"

#define MIN_PLAYER_DISTANCE 200.0f

FollowPlayerScript::FollowPlayerScript(Vector2 begin_pos, Vector2 const& player_pos)
: Script(), player_pos(player_pos) {

    checkpoints.push(player_pos);
    current_direction = (player_pos - begin_pos).normalized();
    current_move_speed = 100.0f;
    is_stationary = false;
}

Vector2 FollowPlayerScript::update(Vector2 entity_pos, float dt) {
    elapsed_time += dt;

    if (!checkpoints.empty()) {
        checkpoints.pop();
    }
    checkpoints.push(player_pos);

    if (is_stationary) {
        return Vector2({0, 0});
    }

    const float epsilon = MIN_PLAYER_DISTANCE;
    if ((entity_pos - checkpoints.front()).length() < epsilon) {
        checkpoints.pop();
    }
    if (checkpoints.empty()) {
        return Vector2({0, 0});
    }

    current_direction = (checkpoints.front() - entity_pos).normalized();

    return current_direction;
}