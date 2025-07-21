#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "GameLoop.h"

GameLoop::GameLoop(int width, int height)
 :  window(sf::VideoMode({static_cast<uint>(width), static_cast<uint>(height)}), "SFML window"),
    playArea(width, height),
    player(playArea),
    player_texture("assets/textures/space_ship.png"),
    player_sprite(player_texture)
{   
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(0, 0));
    this->width = width;
    this->height = height;
    player_start_pos = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    player.setPosition({player.getX(), player.getY() + 300.0f});

    player_sprite.setScale({0.2f, 0.2f});
    player_sprite.setOrigin(player_sprite.getLocalBounds().getCenter());
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

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
                player.shoot();

            player.setVelocity(velocity);
            player.update(dt);
            playArea.update(dt);

            render();

            drawDebug();
        }
 
        window.display();
    }
}

void GameLoop::render() {
    //sf::CircleShape player_sprite(34.0f, 3);
    player_sprite.setPosition({player_start_pos.x + player.getX(), player_start_pos.y + player.getY()});
    player_sprite.setRotation(sf::degrees(player.getRotation()));

    int count = 0;
    for (Bullet& bullet : playArea.getBullets()) {
        sf::CircleShape bullet_sprite(5.0f);
        bullet_sprite.setFillColor(sf::Color(145, 5, 0));
        bullet_sprite.setOutlineThickness(2.0f);
        bullet_sprite.setOutlineColor(sf::Color(255, 88, 82));
        bullet_sprite.setOrigin(bullet_sprite.getLocalBounds().getCenter());
        bullet_sprite.setPosition({bullet.getPosition().x + player_start_pos.x, bullet.getPosition().y + player_start_pos.y});
        window.draw(bullet_sprite);
        count++;
    }
    window.draw(player_sprite);
}

void GameLoop::drawDebug() {
    sf::Font const font("assets/fonts/public_pixel.ttf");
    sf::Text angle_text(font, "r: ", 20);
    sf::Text x_speed_text(font, "vx: ", 20);
    sf::Text y_speed_text(font, "vy: ", 20);

    angle_text.setString("r: "+ std::to_string(player.getRotation()));

    x_speed_text.setString("vx: "+ std::to_string(player.getVelocity().x));
    x_speed_text.setPosition(sf::Vector2f(0, 20));

    y_speed_text.setString("vy: "+ std::to_string(player.getVelocity().y));
    y_speed_text.setPosition(sf::Vector2f(0, 40));

    window.draw(angle_text);
    window.draw(x_speed_text);
    window.draw(y_speed_text);
}