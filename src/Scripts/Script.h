#ifndef SCRIPT_H
#define SCRIPT_H

#include <queue>

#include "../Utils/Vector2.h"

class Script {

public:
    virtual void play() = 0;
    virtual Vector2 update(Vector2 entity_pos, float dt) = 0;

    void addCheckpoint(Vector2 position);
    void setCurrentCheckpoint(Vector2 position);

    float getCurrentMoveSpeed();

    bool isPlaying();

protected:

    bool is_playing;
    float elapsed_time;

    float current_move_speed;

    Vector2 current_direction;

    std::queue<Vector2> checkpoints;
};

#endif // SCRIPT_H