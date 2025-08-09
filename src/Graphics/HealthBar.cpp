#include "HealthBar.h"

HealthBar::HealthBar(sf::RenderWindow& window, Player& player, int x, int y)
 : window(window), player(player) {

    width = 300.0f;
    height = 20.0f;

    outerRect.setSize({width, height});
    outerRect.setPosition({static_cast<float>(x), static_cast<float>(y)});
    outerRect.setFillColor(sf::Color(77, 15, 15));

    innerRect.setSize({width, height});
    innerRect.setPosition({static_cast<float>(x), static_cast<float>(y)});
    innerRect.setFillColor(sf::Color::Red);

    float padding_x = 5.0f;
    float padding_y = 5.0f;

    // for(int i=0; i<10; ++i) {
        // blocks of 10 health
    // }
}

void HealthBar::draw() {
    float max_health = player.getMaxHealth();
    float current_health = player.getCurrentHealth();

    innerRect.setSize({current_health / max_health * width, height});
    window.draw(outerRect);
    window.draw(innerRect);
}