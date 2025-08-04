#include "AsteroidScript.h"

#include <iostream>

AsteroidScript::AsteroidScript(Vector2 begin_pos, Vector2 end_pos)
 : Script() {
    checkpoints.push(end_pos);
    current_direction = (end_pos - begin_pos).normalized();
    current_move_speed = 400.0f;
    is_stationary = false;
}