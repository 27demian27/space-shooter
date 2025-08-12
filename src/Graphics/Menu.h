#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Menu {

public:
    enum class MenuState {
        MAIN,
        GAMEOVER
    };

    Menu(sf::RenderWindow& window);

    void draw(MenuState state);

    void setFinalScore(size_t final_score);
private:

    sf::RenderWindow& window;

    size_t final_score;

    sf::Font arial_font;
    sf::Font pixel_font;

    sf::Text menu_title_text;
    sf::Text controls_desc_text;
    sf::Text esc_to_play_text;
    sf::Text game_over_text;
    sf::Text final_score_text;
    sf::Text play_again_text;

};

#endif // MENU_H