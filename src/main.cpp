#include "GameLoop.h"

#define WIDTH 800
#define HEIGTH 1000

int main()
{
    GameLoop gameLoop = GameLoop(WIDTH, HEIGTH);
    while (!gameLoop.exited()) {
        gameLoop.run();
        gameLoop.~GameLoop();
        new (&gameLoop) GameLoop(WIDTH, HEIGTH);
    }
}