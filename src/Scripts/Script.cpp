
#include "Script.h"

float Script::getCurrentMoveSpeed() { return current_move_speed; }

bool Script::isPlaying() { return is_playing; }

void Script::addCheckpoint(Vector2 position) {
    checkpoints.push(position);
}
void Script::setCurrentCheckpoint(Vector2 position) {
    checkpoints.front() = position;
}