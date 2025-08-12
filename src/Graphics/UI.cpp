#include "UI.h"

UI::UI(sf::RenderWindow& window, Player const& player, int width, int height) 
 : window(window),
   player(player),
   healthBar(window, player, 20, height - 20.0f - 20),
   boostBar(window, player, width / 2.0f + 80, height - 20 - 20),
   specialAttackReadyBox({40, 20}),
   font("assets/fonts/public_pixel.ttf"),
   pointsText(font, "0000000", 40),
   last_score(0)
{
    pointsText.setOrigin({pointsText.getLocalBounds().size.x, 0});
    pointsText.setPosition({static_cast<float>(width), static_cast<float>(0)});

    specialAttackReadyBox.setOrigin(specialAttackReadyBox.getLocalBounds().getCenter());
    specialAttackReadyBox.setPosition({width / 2.0f, height - 40.0f + 10.0f});
    specialAttackReadyBox.setFillColor(sf::Color::Green);
}


void UI::draw() {
    if (last_score != player.getScore()) {
        last_score = player.getScore();
        std::ostringstream ss;
        ss << std::setw(7) << std::setfill('0') << last_score;
        pointsText.setString(ss.str());
    }

    if (player.getRemainingSpecialAttackCooldown() > 0) {
        specialAttackReadyBox.setFillColor(sf::Color(128, 128, 128));
    } else {
        specialAttackReadyBox.setFillColor(sf::Color::Green);
    }
    healthBar.draw();
    boostBar.draw();
    window.draw(specialAttackReadyBox);
    window.draw(pointsText);
}