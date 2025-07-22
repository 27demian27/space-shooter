#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <SFML/Graphics.hpp>

#include "Player/Player.h"
#include "PlayArea/PlayArea.h"
#include "Graphics/HealthBar.h"

class GameLoop {

public:
    GameLoop(int width, int height);

    void run();
    void menu();

private:
    sf::RenderWindow window;

    Player player;
    sf::Vector2f player_start_pos;

    sf::Texture const player_texture;
    sf::Sprite player_sprite;

    sf::Font const font;

    HealthBar health_bar;

    PlayArea playArea;

    int width;
    int height;

    void handleInput();
    void update(float dt);
    void render();

    void drawDebug();
};

#endif //GAMELOOP_H