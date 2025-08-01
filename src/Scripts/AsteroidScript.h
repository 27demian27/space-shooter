#ifndef ASTEROIDSCRIPT_H
#define ASTEROIDSCRIPT_H

#include "Script.h"

class AsteroidScript : public Script {

public:
    AsteroidScript(Vector2 begin_pos, Vector2 end_pos);

    Vector2 update(Vector2 entity_pos, float dt) override;

    void play() override;

private:
    float move_speed;
};

#endif // ASTEROIDSCRIPT_H