#include "HealthBar.h"

HealthBar::HealthBar(sf::RenderWindow& window, Player& player, int x, int y)
 : window(window), player(player) {

    width = 300.0f;
    height = 20.0f;

    outerRect.setSize({width, height});
    outerRect.setPosition({static_cast<float>(x), static_cast<float>(y)});
    outerRect.setFillColor(sf::Color::Red);

    float padding_x = 5.0f;
    float padding_y = 5.0f;

    // for(int i=0; i<10; ++i) {
        
    // }
}

void HealthBar::draw() {
    float max_health = player.getMaxHealth();
    float current_health = player.getCurrentHealth();

    outerRect.setSize({current_health / max_health * width, height});
    window.draw(outerRect);
}