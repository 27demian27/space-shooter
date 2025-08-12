#include "Menu.h"

Menu::Menu(sf::RenderWindow& window)
: window(window), 
  arial_font("assets/fonts/arial.ttf"),
  pixel_font("assets/fonts/public_pixel.ttf"),
  menu_title_text(pixel_font, "Space Shooter", 50),
  controls_desc_text(
      arial_font, 
      "WASD:             Move\n" 
      "Move Mouse:   Aim\n" 
      "Hold Shift:        Boost\n" 
      "Left Click:        Shoot\n" 
      "Space:             Special Attack\n",
      20
  ),
  esc_to_play_text(arial_font,"Press ESC to play.", 20),
  game_over_text(pixel_font, "Game Over", 50),
  final_score_text(pixel_font, "0000000", 40),
  play_again_text(arial_font, "Press ESC to play again", 20)
{
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenterPos(windowSize.x / 2.0f, windowSize.y / 2.0f);

    menu_title_text.setFillColor(sf::Color(50, 0, 100));
    menu_title_text.setOutlineColor(sf::Color(255, 255, 255));
    menu_title_text.setOutlineThickness(0.8f);
    menu_title_text.setOrigin(menu_title_text.getLocalBounds().getCenter());
    menu_title_text.setPosition({windowCenterPos.x, windowCenterPos.y - 300.0f});

    controls_desc_text.setOrigin(controls_desc_text.getLocalBounds().getCenter());
    controls_desc_text.setPosition({windowCenterPos.x, windowCenterPos.y});

    esc_to_play_text.setOrigin(esc_to_play_text.getLocalBounds().getCenter());
    esc_to_play_text.setPosition({windowCenterPos.x, windowCenterPos.y + 200.0f});

    game_over_text.setFillColor(sf::Color(10, 210, 10));
    game_over_text.setOutlineColor(sf::Color(255, 255, 255));
    game_over_text.setOutlineThickness(0.8f);
    game_over_text.setOrigin(game_over_text.getLocalBounds().getCenter());
    game_over_text.setPosition({windowCenterPos.x, windowCenterPos.y - 300.0f});

    final_score_text.setOrigin(final_score_text.getLocalBounds().getCenter());
    final_score_text.setPosition({windowCenterPos.x, windowCenterPos.y});

    play_again_text.setOrigin(play_again_text.getLocalBounds().getCenter());
    play_again_text.setPosition({windowCenterPos.x, windowCenterPos.y + 200.0f});
}

void Menu::draw(MenuState state) {
    if(state == MenuState::MAIN) {
        window.draw(menu_title_text);
        window.draw(controls_desc_text);
        window.draw(esc_to_play_text);
    }
    else if (state == MenuState::GAMEOVER) {
        window.draw(game_over_text);
        window.draw(final_score_text);
        window.draw(play_again_text);
    }
}

void Menu::setFinalScore(size_t final_score) { 
    this->final_score = final_score;
    std::ostringstream ss;
    ss << std::setw(7) << std::setfill('0') << final_score;
    final_score_text.setString(ss.str());
}