#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "Player/Player.h"
#include "PlayArea/PlayArea.h"
#include "EnemyTypes.h"

class GameLoop {

public:
    GameLoop(int width, int height);

    void run();
    void menu();

    bool exited() const;

private:
    bool has_exited;

    float const asteroid_cooldown;
    float curr_asteroid_cooldown;

    float const enemy_cooldown;
    float curr_enemy_cooldown;

    sf::RenderWindow window;

    Player player;
    sf::Vector2f player_start_pos;

    sf::Texture const player_texture_thrust;
    sf::Texture const player_texture_no_thrust;
    sf::Sprite player_sprite;

    std::vector<sf::Texture> asteroid_textures;
    std::vector<sf::Texture> enemy_textures;

    std::vector<sf::Sprite> background_sprites;
    std::vector<sf::Texture> background_sprite_textures;

    std::map<Vector2, float> collision_explosions;

    sf::Font const font;

    PlayArea playArea;

    int width;
    int height;

    void spawnEnemy(EnemyType type, Vector2 position);

    void loadBackgroundSpriteTextures();
    void initBackgroundSprites();
    void drawBackgroundSprites(float dt, float move_speed);

    Vector2 getRandomPosition(float width, float height);

    void spawnAsteroid(float dt);

    void updateExplosions(float dt);

    void handleInput();
    void update(float dt);
    void render();

    void drawDebug();
};

#endif //GAMELOOP_H