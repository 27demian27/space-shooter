#ifndef GAMELOOP
#define GAMELOOP

#include <SFML/Graphics.hpp>

#include "Player.h"

class GameLoop {

public:
    GameLoop(int width, int height);

    void run();
    void menu();

private:
    sf::RenderWindow window;
    Player player;
    sf::Vector2f player_start_pos;

    int width;
    int height;

    void handleInput();
    void update(float dt);
    void render();

    void drawDebug();
};

#endif //GAMELOOP