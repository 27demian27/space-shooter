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

    sf::Texture const background_texture("assets/textures/space_background.jpg");
    sf::Sprite background_sprite(background_texture);
 

    sf::Font const font("assets/fonts/public_pixel.ttf");
    sf::Text menu_title_text(font, "Space Shooter", 50);
    menu_title_text.setFillColor(sf::Color(50, 0, 100));
    menu_title_text.setOutlineColor(sf::Color(255, 255, 255));
    menu_title_text.setOutlineThickness(0.8f);

    sf::FloatRect textBounds = menu_title_text.getLocalBounds();
    menu_title_text.setOrigin(textBounds.getCenter());

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenterPos(windowSize.x / 2.0f, windowSize.y / 2.0f);
    menu_title_text.setPosition(windowCenterPos);
 
    bool in_menu = true;

    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
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
            }
        }
 
        window.clear();
 
 
        if(in_menu) {
            window.draw(background_sprite);
            window.draw(menu_title_text);
        } else {

            Vector2 velocity = {
                static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)),
                static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
            };

            player.setVelocity(velocity);
            player.update(dt);

            render();

            drawDebug();
        }
 
        window.display();
    }
}

void GameLoop::render() {
    sf::CircleShape player_sprite(80.f, 3);
    player_sprite.setScale(sf::Vector2f(0.5f, 0.5f));
    player_sprite.setOrigin(player_sprite.getLocalBounds().getCenter());
    player_sprite.setPosition({player_start_pos.x + player.getX(), player_start_pos.y + player.getY()});
    player_sprite.setRotation(sf::degrees(player.getRotation()));
    window.draw(player_sprite);
}

void GameLoop::drawDebug() {
    sf::Font const font("assets/fonts/public_pixel.ttf");
    sf::Text angle_text(font, "r: ", 50);
    sf::Text x_speed_text(font, "vx: ", 50);
    sf::Text y_speed_text(font, "vy: ", 50);

    angle_text.setString("r: "+ std::to_string(player.getRotation()));

    x_speed_text.setString("vx: "+ std::to_string(player.getVelocity().x));
    x_speed_text.setPosition(sf::Vector2f(0, 50));

    y_speed_text.setString("vy: "+ std::to_string(player.getVelocity().y));
    y_speed_text.setPosition(sf::Vector2f(0, 100));

    window.draw(angle_text);
    window.draw(x_speed_text);
    window.draw(y_speed_text);
}