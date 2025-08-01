#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "GameLoop.h"
#include "Utils/Hitbox.h"
#include "Scripts/AsteroidScript.h"

#define MAX_ENTITY_COUNT 10

GameLoop::GameLoop(int width, int height)
 :  window(sf::VideoMode({static_cast<uint>(width), static_cast<uint>(height)}), "Space Shooter"),
    playArea(width, height),
    player(playArea),
    player_texture("assets/textures/space_ship.png"),
    player_sprite(player_texture),
    health_bar(window, player, 20, height - 20.0f - 20),
    font("assets/fonts/public_pixel.ttf"),
    boost_bar(window, player, font)
{   
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(0, 0));
    this->width = width;
    this->height = height;
    player_start_pos = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    player.setPosition({player.getX(), player.getY() + 300.0f});

    player.setSize({player_sprite.getLocalBounds().size.x / 2.0f, player_sprite.getLocalBounds().size.y / 2.0f});
    
    float scale_x = 1.0f / (player_sprite.getGlobalBounds().size.x / player.getSize().x);
    float scale_y = 1.0f / (player_sprite.getGlobalBounds().size.y / player.getSize().y);
    player_sprite.setScale({scale_x, scale_y});

    player_sprite.setOrigin(player_sprite.getLocalBounds().getCenter());

    asteroid_textures.push_back(sf::Texture("assets/textures/asteroid2.png"));
    asteroid_textures[0].setSmooth(true);
    curr_asteroid_cooldown = 0.0f;
}

void GameLoop::spawnAsteroid(float dt) {
  curr_asteroid_cooldown -= dt;
    if (curr_asteroid_cooldown <= 0) {
        Vector2 player_pos = player.getPosition();

        float angle = static_cast<float>((rand() / (float)RAND_MAX) * 2.0 * M_PI);
        Vector2 direction = Vector2({std::cos(angle), std::sin(angle)});

        float D = std::max(width, height);

        Vector2 entity_start_pos = player_pos - direction * D;
        Vector2 entity_end_pos = player_pos + direction * D;

        int MAX_SIZE = 300;
        float MAX_MAX_HEALTH = 30;
        int MIN_SIZE = 10;
        float size = static_cast<float>(MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE));

        float max_health = size / MAX_SIZE * MAX_MAX_HEALTH;

        auto asteroid_script = std::make_unique<AsteroidScript>(entity_start_pos, entity_end_pos);
        playArea.addEntity(std::make_unique<Entity>(
            entity_start_pos,
            Vector2({size, size}),
            max_health,
            10,
            std::move(asteroid_script),
            HitboxShape::CIRCLE
        ));

        curr_asteroid_cooldown = asteroid_cooldown;
    }
}


void GameLoop::run() {
    window.setFramerateLimit(120);

    sf::Texture const background_texture("assets/textures/space_background.jpg");
    sf::Sprite background_sprite(background_texture);
 
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/sfx/shoot_sound.wav"))
        return;

    sf::Sound shoot_sound(buffer);
    sf::Text menu_title_text(font, "Space Shooter", 50);
    menu_title_text.setFillColor(sf::Color(50, 0, 100));
    menu_title_text.setOutlineColor(sf::Color(255, 255, 255));
    menu_title_text.setOutlineThickness(0.8f);
    menu_title_text.setOrigin(menu_title_text.getLocalBounds().getCenter());

    sf::Text game_over_text(font, "Game Over", 50);
    game_over_text.setFillColor(sf::Color(10, 210, 10));
    game_over_text.setOutlineColor(sf::Color(255, 255, 255));
    game_over_text.setOutlineThickness(0.8f);
    game_over_text.setOrigin(game_over_text.getLocalBounds().getCenter());

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenterPos(windowSize.x / 2.0f, windowSize.y / 2.0f);
    menu_title_text.setPosition({windowCenterPos.x, windowCenterPos.y - 300.0f});
    game_over_text.setPosition({windowCenterPos.x, windowCenterPos.y - 300.0f});

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
 
 
        if (in_menu) {
            window.draw(background_sprite);
            window.draw(menu_title_text);

        } else if (player.getCurrentHealth() <= 0) {
            window.draw(background_sprite);
            window.draw(game_over_text);
        } else {

            if (player.getKnockbackDowntime() <= 0) {
                Vector2 velocity = {
                    static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)),
                    static_cast<float>(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
                    player.setIsBoosting(true);
                } else {
                    player.setIsBoosting(false);
                }
                player.setVelocity(velocity);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
                if(player.shoot())
                    shoot_sound.play();

            if (playArea.getEntities().size() < MAX_ENTITY_COUNT) {
                spawnAsteroid(dt);
            }

            player.update(dt);
            playArea.update(dt);


            render();

            drawDebug();

            health_bar.draw();
            boost_bar.draw();

        }
 
        window.display();
    }
}

void GameLoop::render() {
    player_sprite.setPosition({player_start_pos.x + player.getX(), player_start_pos.y + player.getY()});
    player_sprite.setRotation(sf::degrees(player.getRotation()));


    for (const auto& entity_ptr : playArea.getEntities()) {
        const Entity& entity = *entity_ptr;

        sf::Sprite entity_sprite(asteroid_textures[0]);
        sf::FloatRect localBounds = entity_sprite.getLocalBounds();

        float scale_x = 1.0f / (entity_sprite.getGlobalBounds().size.x / entity.getSize().x);
        float scale_y = 1.0f / (entity_sprite.getGlobalBounds().size.y / entity.getSize().y);

        entity_sprite.setScale({scale_x, scale_y});
        entity_sprite.setOrigin(localBounds.getCenter());
        entity_sprite.setPosition({width / 2.0f + entity.getPosition().x, height / 2.0f + entity.getPosition().y});
        window.draw(entity_sprite);
    }
    for (const auto& bullet_ptr : playArea.getBullets()) {
        const Bullet& bullet = *bullet_ptr;

        sf::CircleShape bullet_sprite(5.0f);
        bullet_sprite.setFillColor(sf::Color(145, 5, 0));
        bullet_sprite.setOutlineThickness(2.0f);
        bullet_sprite.setOutlineColor(sf::Color(255, 88, 82));
        bullet_sprite.setOrigin(bullet_sprite.getLocalBounds().getCenter());
        bullet_sprite.setPosition({
            bullet.getPosition().x + player_start_pos.x,
            bullet.getPosition().y + player_start_pos.y
        });

        window.draw(bullet_sprite);
    }
    window.draw(player_sprite); 
}

void GameLoop::drawDebug() {
    sf::Text angle_text(font, "r: ", 20);
    sf::Text x_speed_text(font, "vx: ", 20);
    sf::Text y_speed_text(font, "vy: ", 20);
    sf::Text x_pos_text(font, "x: ", 20);
    sf::Text y_pos_text(font, "y: ", 20);

    sf::RectangleShape player_hitbox({player.getHitbox().size().x, player.getHitbox().size().y});
    player_hitbox.setFillColor(sf::Color::Transparent);
    player_hitbox.setOutlineThickness(3.0f);
    player_hitbox.setOrigin(player_hitbox.getLocalBounds().getCenter());
    player_hitbox.setPosition(
        sf::Vector2f(
            player.getX() + player_start_pos.x, 
            player.getY() + player_start_pos.y)
    );

    for (const auto& entity_ptr : playArea.getEntities()) {
        Entity& entity = *entity_ptr;

        switch (entity.getHitbox().shape) {
            case HitboxShape::RECT: {
                sf::RectangleShape entity_hitbox({entity.getHitbox().size().x, entity.getHitbox().size().y});
                entity_hitbox.setOutlineThickness(3.0f);
                entity_hitbox.setFillColor(sf::Color::Transparent);
                entity_hitbox.setOrigin(entity_hitbox.getLocalBounds().getCenter());
                entity_hitbox.setPosition({width / 2.0f + entity.getPosition().x, height / 2.0f + entity.getPosition().y});
                window.draw(entity_hitbox);
                break;
            }
            case HitboxShape::CIRCLE: {
                sf::CircleShape entity_hitbox(entity.getHitbox().size().x / 2.0f);
                entity_hitbox.setOutlineThickness(3.0f);
                entity_hitbox.setFillColor(sf::Color::Transparent);
                entity_hitbox.setOrigin(entity_hitbox.getLocalBounds().getCenter());
                entity_hitbox.setPosition({width / 2.0f + entity.getPosition().x, height / 2.0f + entity.getPosition().y});
                window.draw(entity_hitbox);
                break;
            }
            default:
                std::cerr << "Error: Unknown HitboxShape\n";
        }

        float health_text_size = 10.0f;
        sf::Text health_text(font, "100/100", health_text_size);
        health_text.setString(
            std::to_string(static_cast<int>(entity.getCurrentHealth())) + 
            "/" + 
            std::to_string(static_cast<int>(entity.getMaxHealth()))
        );
        health_text.setPosition({
            width / 2.0f + entity.getPosition().x, 
            height / 2.0f + entity.getPosition().y + entity.getHitbox().size().y / 2.0f + 5.0f
        });
        window.draw(health_text);
    }

    for (const auto& bullet_ptr : playArea.getBullets()) {
        Bullet& bullet = *bullet_ptr;
        sf::CircleShape bullet_hitbox(bullet.getHitbox().size().x / 2.0f);
        bullet_hitbox.setOutlineThickness(3.0f);
        bullet_hitbox.setFillColor(sf::Color::Transparent);
        bullet_hitbox.setOrigin(bullet_hitbox.getLocalBounds().getCenter());
        bullet_hitbox.setPosition({width / 2.0f + bullet.getPosition().x, height / 2.0f + bullet.getPosition().y});
        window.draw(bullet_hitbox);
    }

    angle_text.setString("r: "+ std::to_string(player.getRotation()));

    x_speed_text.setString("vx: "+ std::to_string(player.getVelocity().x));
    x_speed_text.setPosition(sf::Vector2f(0, 20));

    y_speed_text.setString("vy: "+ std::to_string(player.getVelocity().y));
    y_speed_text.setPosition(sf::Vector2f(0, 40));

    x_pos_text.setString("x: "+ std::to_string(player.getPosition().x));
    x_pos_text.setPosition(sf::Vector2f(0, 60));

    y_pos_text.setString("y: "+ std::to_string(player.getPosition().y));
    y_pos_text.setPosition(sf::Vector2f(0, 80));


    window.draw(angle_text);
    window.draw(x_speed_text);
    window.draw(y_speed_text);
    window.draw(x_pos_text);
    window.draw(y_pos_text);
    window.draw(player_hitbox);
}