#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include <memory>
#include <iostream> 

#include "Utils/Hitbox.h"
#include "Utils/Rect.h"
#include "Utils/Circle.h"

class Collidable {

public:

    Collidable(std::unique_ptr<Hitbox> hitbox)
     : hitbox(std::move(hitbox)) 
    {};

    virtual ~Collidable() = default;

    virtual bool collision(Collidable& other) = 0;

    const Hitbox& getHitbox() {
        if (!hitbox) {
            std::cerr << "Error: hitbox is null in getHitbox()\n";
            throw std::runtime_error("Null Hitbox access");
        }
        return *hitbox;
    };

    virtual const float getCollisionDamage() const = 0;

    void updateHitbox(Vector2 position, Vector2 size) {
        if (!hitbox) {
            std::cerr << "Error: updateHitbox called but hitbox is null.\n";
            std::cerr << size.x << "\n";
            return;
        }
        float hitbox_width = size.x;
        float hitbox_height = size.y;

        switch (hitbox->shape) {
            case HitboxShape::RECT: {
                    Rect* rect = dynamic_cast<Rect*>(hitbox.get());
                    rect->TL = {position.x - hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};
                    rect->BL = {position.x - hitbox_width / 2.0f, position.y + hitbox_height / 2.0f};
                    rect->TR = {position.x + hitbox_width / 2.0f, position.y - hitbox_height / 2.0f};
                    rect->BR = {position.x + hitbox_width / 2.0f, position.y + hitbox_height / 2.0f};
                break;
            }
            case HitboxShape::CIRCLE: {
                Circle* circle = dynamic_cast<Circle*>(hitbox.get());
                if (size.x != size.y) {
                    std::cerr << "Warning: Circle entity updated with non-uniform dimensions\n";
                }
                circle->radius = hitbox_width / 2.0f;
                circle->middlepoint = {
                    position.x, 
                    position.y
                };
                break;
            }
            default:
                std::cerr << "Error: Unknown HitboxShape\n";
                break;
        }
    }

    static std::unique_ptr<Hitbox> createHitbox(Vector2 position, Vector2 size, HitboxShape shape) {
        switch (shape) {
            case HitboxShape::RECT: {
                Vector2 TL = position;
                Vector2 TR = { position.x + size.x, position.y };
                Vector2 BL = { position.x, position.y + size.y };
                Vector2 BR = { position.x + size.x, position.y + size.y };
                return std::make_unique<Rect>(TL, BL, TR, BR);
            }
            case HitboxShape::CIRCLE: {
                if (size.x != size.y) {
                    std::cerr << "Warning: Circle entity made with rectangular dimensions\n";
                }
                float radius = size.x / 2.0f;
                Vector2 center = { position.x + radius, position.y + radius };
                return std::make_unique<Circle>(radius, center);
            }
            default:
                std::cerr << "Error: Unknown HitboxShape\n";
                return std::make_unique<Circle>(1.0f, Vector2({0, 0}));
        }
    }


protected:
    float collision_damage;
    std::unique_ptr<Hitbox> hitbox;
};

#endif //COLLIDABLE_H