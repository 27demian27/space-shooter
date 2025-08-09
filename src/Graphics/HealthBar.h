#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <vector>

#include <SFML/Graphics.hpp>
#include "../Player/Player.h"

class HealthBar {

public:
    HealthBar(sf::RenderWindow& window, Player& player, int x, int y);

    void draw();
private:

    float width;
    float height;


    sf::RenderWindow& window;
    Player& player;

    sf::RectangleShape outerRect;
    sf::RectangleShape innerRect;
    //std::vector<sf::RectangleShape> innerRects;

};

#endif //HEALTHBAR_H