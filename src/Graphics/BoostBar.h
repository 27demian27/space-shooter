#ifndef BOOSTBAR_H
#define BOOSTBAR_H

#include <SFML/Graphics.hpp>
#include "../Player/Player.h"

class BoostBar {

public:

    BoostBar(sf::RenderWindow& window, Player& player, const sf::Font& font)
     : window(window),
       player(player),
       font(font)
    {}

    void draw() {
        sf::Text text(font, 
            std::to_string(player.getRemainingBoost()) + 
            "/" + 
            std::to_string(player.getBoostCapacity()),
            20
        );
        text.setPosition({
            static_cast<float>(window.getSize().x - 100), 
            static_cast<float>(window.getSize().y - 40)
        });
        window.draw(text);
    }

private:
    sf::RenderWindow& window;
    const sf::Font& font;

    Player& player;
};

#endif //BOOSTBAR_H