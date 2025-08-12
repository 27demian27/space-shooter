#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>

#include "Elements/BoostBar.h"
#include "Elements/HealthBar.h"

class UI {
public:

    UI(sf::RenderWindow& window, Player const& player, int width, int height);

    void draw();

private:

    sf::RenderWindow& window;
    Player const& player;

    size_t last_score;

    sf::Font font;

    BoostBar boostBar;
    HealthBar healthBar;

    sf::RectangleShape specialAttackReadyBox;

    sf::Text pointsText;
};

#endif // UI_H