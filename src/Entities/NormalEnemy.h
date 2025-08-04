#ifndef NORMALENEMY_H
#define NORMALENEMY_H

#include "Entity.h"
#include "../Shooter.h"
#include "../PlayArea/PlayArea.h"

class NormalEnemy : public Entity, public Shooter {
public:
    NormalEnemy(
        Vector2 position,
        Vector2 size,
        float max_health,
        float collision_damage,
        std::unique_ptr<Script> script,
        HitboxShape hitboxShape,
        Vector2 const& player_pos,
        PlayArea& playArea
    );

    void update(float dt);

    bool shoot() override;

    float getRotation() override;
    float getBulletSpeed() override;

    float getBulletDamage() override;
private:

    PlayArea& playArea;

    Vector2 const& player_pos;
};

#endif
