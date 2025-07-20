#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "GameLoop.h"

GameLoop::GameLoop(int width, int height)
    : window(sf::VideoMode({static_cast<uint>(width), static_cast<uint>(height)}), "SFML window") {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(0, 0));
    this->width = width;
    this->height = height;
    this->player = Player();
    this->player_start_pos = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 1.2f);

}


void GameLoop::run() {
    window.setFramerateLimit(120);

    const sf::Texture background_texture("assets/textures/space_background.jpg");
    sf::Sprite background_sprite(background_texture);
 

    const sf::Font font("assets/fonts/public_pixel.ttf");
    sf::Text menu_title_text(font, "Space Shooter", 50);
    menu_title_text.setFillColor(sf::Color(50, 0, 100));
    menu_title_text.setOutlineColor(sf::Color(255, 255, 255));
    menu_title_text.setOutlineThickness(0.8f);

    sf::Text angle_text(font, "Angle", 50);

    sf::FloatRect textBounds = menu_title_text.getLocalBounds();
    menu_title_text.setOrigin(textBounds.getCenter());

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenterPos(windowSize.x / 2.0f, windowSize.y / 2.0f);
    menu_title_text.setPosition(windowCenterPos);
 
    bool in_menu = true;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    in_menu = !in_menu;
            }
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                sf::Angle player_rotation = sf::degrees(player.getRotation());
                player.setRotation(Vector2(
                    {
                        static_cast<float>(mouseMoved->position.x) - player_start_pos.x, 
                        static_cast<float>(mouseMoved->position.y) - player_start_pos.y
                    }));
                angle_text.setString(std::to_string(player.getRotation()));
            }
        }
 
        window.clear();
 
 
        if(in_menu) {
            window.draw(background_sprite);
            window.draw(menu_title_text);
        } else {
            render();
            window.draw(angle_text);
        }
 
        window.display();
    }
}

void GameLoop::render() {
    sf::CircleShape player_sprite(80.f, 3);
    player_sprite.setOrigin(player_sprite.getLocalBounds().getCenter());
    player_sprite.setPosition({player_start_pos.x, player_start_pos.y});
    player_sprite.setRotation(sf::degrees(player.getRotation()));
    window.draw(player_sprite);
}