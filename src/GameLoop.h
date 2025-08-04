#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "Player/Player.h"
#include "PlayArea/PlayArea.h"
#include "Graphics/HealthBar.h"
#include "Graphics/BoostBar.h"
#include "EnemyTypes.h"

class GameLoop {

public:
    GameLoop(int width, int height);

    void run();
    void menu();


private:

    float const asteroid_cooldown = 1.4f;
    float curr_asteroid_cooldown;

    sf::RenderWindow window;

    Player player;
    sf::Vector2f player_start_pos;

    sf::Texture const player_texture_thrust;
    sf::Texture const player_texture_no_thrust;
    sf::Sprite player_sprite;

    std::vector<sf::Texture> asteroid_textures;

    std::vector<sf::Sprite> background_sprites;
    std::vector<sf::Texture> background_sprite_textures;

    sf::Font const font;

    HealthBar health_bar;
    BoostBar boost_bar;

    PlayArea playArea;

    int width;
    int height;

    void spawnEnemy(EnemyType type, Vector2 position);

    void loadBackgroundSpriteTextures();
    void initBackgroundSprites();
    void drawBackgroundSprites(float dt);

    Vector2 getRandomPosition();

    void spawnAsteroid(float dt);

    void handleInput();
    void update(float dt);
    void render();

    void drawDebug();
};

#endif //GAMELOOP_H