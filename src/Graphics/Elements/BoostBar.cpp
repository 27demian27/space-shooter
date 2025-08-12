#include "BoostBar.h"

BoostBar::BoostBar(sf::RenderWindow& window, Player const& player, int x, int y)
 : window(window), player(player) {

    width = 300.0f;
    height = 20.0f;

    outerRect.setSize({width, height});
    outerRect.setPosition({static_cast<float>(x), static_cast<float>(y)});
    outerRect.setFillColor(sf::Color(27, 87, 143));

    innerRect.setSize({width, height});
    innerRect.setPosition({static_cast<float>(x), static_cast<float>(y)});
    innerRect.setFillColor(sf::Color(48, 155, 255));
}

void BoostBar::draw() {
    float boost_capacity = player.getBoostCapacity();
    float remaining_boost = player.getRemainingBoost();

    innerRect.setSize({remaining_boost / boost_capacity * width, height});
    window.draw(outerRect);
    window.draw(innerRect);
}