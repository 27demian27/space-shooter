#ifndef FOLLOWPLAYERSCRIPT_H
#define FOLLOWPLAYERSCRIPT_H

#include "Script.h"

class FollowPlayerScript : public Script {

public:
    FollowPlayerScript(Vector2 begin_pos, Vector2 const& player_pos);

    Vector2 update(Vector2 entity_pos, float dt) override;

private:
    Vector2 const& player_pos;
};

#endif // FOLLOWPLAYERSCRIPT_H