#ifndef BOOSTBAR_H
#define BOOSTBAR_H

#include <SFML/Graphics.hpp>
#include "../Player/Player.h"

class BoostBar {

public:

    BoostBar(sf::RenderWindow& window, Player& player, int x, int y);

    void draw();

private:

    float width = 300.0f;
    float height = 20.0f;

    sf::RenderWindow& window;

    Player& player;

    sf::RectangleShape outerRect;
    sf::RectangleShape innerRect;
};

#endif //BOOSTBAR_H