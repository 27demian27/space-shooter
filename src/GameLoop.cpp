#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <random>

#include "GameLoop.h"
#include "Entities/NormalEnemy.h"
#include "Utils/Hitbox.h"
#include "Scripts/AsteroidScript.h"
#include "Scripts/FollowPlayerScript.h"
#include "Graphics/UI.h"
#include "Graphics/Menu.h"

#define MAX_ENTITY_COUNT 10
#define MAX_ENEMY_COUNT 1
#define ASTEROID_COOLDOWN 5.0f
#define NORMAL_ENEMY_COOLDOWN 10.0f
#define DMG_ANIMATION_DURATION 0.05f
#define EXPLOSION_ANIMATION_DURATION 0.05f

GameLoop::GameLoop(int width, int height)
 :  window(sf::VideoMode({static_cast<uint>(width), static_cast<uint>(height)}), "Space Shooter"),
    playArea(width, height),
    player(playArea),
    player_texture_thrust("assets/textures/space_ship.png"),
    player_texture_no_thrust("assets/textures/space_ship_no_flames.png"),
    player_sprite(player_texture_thrust),
    font("assets/fonts/public_pixel.ttf"),
    asteroid_cooldown(ASTEROID_COOLDOWN),
    enemy_cooldown(NORMAL_ENEMY_COOLDOWN)
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
    curr_asteroid_cooldown = 0.0f;
    curr_enemy_cooldown = 0.0f;

    loadBackgroundSpriteTextures();
    initBackgroundSprites();
}

void GameLoop::loadBackgroundSpriteTextures() {
    sf::Texture texture("assets/textures/mini_star.png");
    background_sprite_textures.push_back(texture);

}

void GameLoop::spawnEnemy(EnemyType type, Vector2 position) {
    if (type == EnemyType::NORMAL) {
        auto script = std::make_unique<FollowPlayerScript>(position, player.getPosition());
        playArea.addEnemy(EnemyType::NORMAL,
            std::make_unique<NormalEnemy>(
                position,
                Vector2({100, 100}),
                100.0f,
                10.0f,
                std::move(script),
                HitboxShape::RECT,
                player.getPosition(),
                playArea
            )
        );
    }
}

Vector2 GameLoop::getRandomPosition(float width, float height) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.0f, width);
    std::uniform_real_distribution<float> distY(0.0f, height);

    return { distX(gen), distY(gen) };
}

void GameLoop::initBackgroundSprites() {
    int max_background_sprites = 80;

    for (int i = 0; i < max_background_sprites; i++) {
        Vector2 pos = getRandomPosition(width, height);
        sf::Sprite sprite(background_sprite_textures[i % background_sprite_textures.size()]);
        sprite.setPosition({pos.x, pos.y});
        background_sprites.push_back(sprite);
    }

}

void GameLoop::drawBackgroundSprites(float dt, float move_speed) {
    int max_background_sprites = 200;

    for (auto it = background_sprites.begin(); it != background_sprites.end(); ) {
        sf::Sprite& sprite = *it;
        window.draw(sprite);

        sf::Vector2f pos = sprite.getPosition();
        sprite.setPosition({pos.x, pos.y + move_speed * dt});


        if (sprite.getPosition().y > height) {
            it = background_sprites.erase(it);
        } else {
            ++it;
        }
    }

    for (int i = background_sprites.size()-1; i < max_background_sprites; i++) {
        Vector2 pos = getRandomPosition(width, height);
        sf::Sprite sprite(background_sprite_textures[i % background_sprite_textures.size()]);
        sprite.setPosition({pos.x, pos.y - height});
        background_sprites.push_back(sprite);
    }

}

void GameLoop::updateExplosions(float dt) {
    for (auto& collision_explosion : collision_explosions) {
        collision_explosion.second -= dt;
    }
    for (auto it = collision_explosions.begin(); it != collision_explosions.end(); ) {
        if (it->second <= 0) {
            it = collision_explosions.erase(it);
        } else {
            ++it;
        }
    }
    for (Vector2 const& collision_coord : playArea.getCollisionCoords()) {
        collision_explosions.insert({collision_coord, EXPLOSION_ANIMATION_DURATION});
    }
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

        int MAX_SIZE = 200;
        float MAX_MAX_HEALTH = 30;
        int MIN_SIZE = 10;
        float size = static_cast<float>(MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE));

        float max_health = size / MAX_SIZE * MAX_MAX_HEALTH;
        size_t points = 10;
        auto asteroid_script = std::make_unique<AsteroidScript>(entity_start_pos, entity_end_pos);
        playArea.addEntity(std::make_unique<Entity>(
            entity_start_pos,
            Vector2({size, size}),
            max_health,
            points, 
            10,
            std::move(asteroid_script),
            HitboxShape::CIRCLE
        ));

        curr_asteroid_cooldown = asteroid_cooldown;
    }
}


void GameLoop::run() {
    //window.setFramerateLimit(120);

    sf::Texture const background_texture("assets/textures/space_background.jpg");
    sf::Sprite background_sprite(background_texture);
 
    enemy_textures.push_back(sf::Texture("assets/textures/enemy1.png"));

    sf::SoundBuffer shoot_buffer;
    if (!shoot_buffer.loadFromFile("assets/sfx/shoot_sound.wav")) {
        std::cerr << "ERROR: FAILED TO LOAD SHOOT_SOUND\n";
        return;
    }
    sf::Sound shoot_sound(shoot_buffer);

    sf::SoundBuffer explosion_buffer;
    if (!explosion_buffer.loadFromFile("assets/sfx/explosion.wav")) {
        std::cerr << "ERROR: FAILED TO LOAD EXPLOSION_SOUND\n";
        return;
    }
    sf::Sound explosion_sound(explosion_buffer);
    explosion_sound.setVolume(15.0f);

    Menu menu(window);

    UI ui(window, player, width, height);


    bool in_menu = true;
    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                has_exited = true;
                return;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    in_menu = !in_menu;
                    if (player.getCurrentHealth() <= 0) {
                        return;
                    }
                }
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
            drawBackgroundSprites(dt, 100.0f);
            menu.draw(Menu::MenuState::MAIN);
        } else if (player.getCurrentHealth() <= 0) {
            drawBackgroundSprites(dt, 50.0f);
            menu.setFinalScore(player.getScore());
            menu.draw(Menu::MenuState::GAMEOVER);
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

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                player.setShootingMode(NORMAL_ATTACK);
                if(player.shoot())
                    shoot_sound.play(); 
            } 

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
                    player.setShootingMode(SPECIAL_ATTACK);
                    if(player.shoot())
                        shoot_sound.play();
            }

            if (playArea.getEntities().size() < MAX_ENTITY_COUNT) {
                spawnAsteroid(dt);
            }

            curr_enemy_cooldown -= dt;
            if (curr_enemy_cooldown <= 0) {
                if (playArea.getEnemies(NORMAL).size() < MAX_ENEMY_COUNT) {
                    spawnEnemy(NORMAL, getRandomPosition(width, 200.0f) - Vector2({width / 2.0f, height / 2.0f}));
                    curr_enemy_cooldown = enemy_cooldown;
                }
            }

            player.update(dt);
            playArea.update(dt);

            updateExplosions(dt);

            if (playArea.wasCollision())
                explosion_sound.play();
            if (player.lastTakenDamage() <= DMG_ANIMATION_DURATION)
                explosion_sound.play();

            drawBackgroundSprites(dt, 500.0f);

            render();

            //drawDebug();

            ui.draw();

        }
 
        window.display();
    }
}

void GameLoop::render() {
    player_sprite.setPosition({player_start_pos.x + player.getX(), player_start_pos.y + player.getY()});
    player_sprite.setRotation(sf::degrees(player.getRotation()));
    if(player.getRotation() >= 90) {
        player_sprite.setTexture(player_texture_no_thrust);
    } else {
        player_sprite.setTexture(player_texture_thrust);
    }
    if(player.lastTakenDamage() <= DMG_ANIMATION_DURATION) {
        player_sprite.setColor(sf::Color::Red);
    } else {
        player_sprite.setColor(sf::Color::White);
    }

    for (auto const& entity_ptr : playArea.getEntities()) {
        Entity const& entity = *entity_ptr;

        sf::Sprite entity_sprite(asteroid_textures[0]);
        sf::FloatRect localBounds = entity_sprite.getLocalBounds();

        float scale_x = 1.0f / (entity_sprite.getGlobalBounds().size.x / entity.getSize().x);
        float scale_y = 1.0f / (entity_sprite.getGlobalBounds().size.y / entity.getSize().y);

        entity_sprite.setScale({scale_x, scale_y});
        entity_sprite.setOrigin(localBounds.getCenter());
        entity_sprite.setPosition({width / 2.0f + entity.getPosition().x, height / 2.0f + entity.getPosition().y});
        if (entity.lastTakenDamage() <= DMG_ANIMATION_DURATION) {
            entity_sprite.setColor(sf::Color::Red);
        }
        window.draw(entity_sprite);
    }
    for (auto const& bullet_ptr : playArea.getBullets()) {
        Bullet const& bullet = *bullet_ptr;

        std::unique_ptr<sf::Shape> bullet_sprite;

        if (bullet.getSize().x == bullet.getSize().y) {
            bullet_sprite = std::make_unique<sf::CircleShape>(bullet.getHitbox().size().x / 2.0f);
        } else {
            bullet_sprite = std::make_unique<sf::RectangleShape>(sf::Vector2f{bullet.getSize().x, bullet.getSize().y});
        }

        bullet_sprite->setOutlineThickness(2.0f);
        if (bullet.isFromPlayer()) {
            bullet_sprite->setFillColor(sf::Color(3, 157, 252));
            bullet_sprite->setOutlineColor(sf::Color(47, 116, 158));
        } else {
            bullet_sprite->setFillColor(sf::Color(145, 5, 0));
            bullet_sprite->setOutlineColor(sf::Color(255, 88, 82));
        }
        bullet_sprite->setOrigin(bullet_sprite->getLocalBounds().getCenter());
        bullet_sprite->setPosition({
            bullet.getPosition().x + player_start_pos.x,
            bullet.getPosition().y + player_start_pos.y
        });
        bullet_sprite->setRotation(sf::degrees(bullet.getRotation() * 180.0f / M_PI + 90.0f));

        window.draw(*bullet_sprite);
    }

    for (auto const& enemy_ptr : playArea.getEnemies(EnemyType::NORMAL)) {
        NormalEnemy* enemy = dynamic_cast<NormalEnemy*>(enemy_ptr.get());

        sf::Sprite enemy_sprite(enemy_textures[0]);
        sf::FloatRect localBounds = enemy_sprite.getLocalBounds();

        enemy_sprite.scale({0.8f, 0.8f});

        enemy->setSize({enemy_sprite.getGlobalBounds().size.x, enemy_sprite.getGlobalBounds().size.y});
        enemy_sprite.setOrigin(localBounds.getCenter());
        enemy_sprite.setPosition({width / 2.0f + enemy->getPosition().x, height / 2.0f + enemy->getPosition().y});
        enemy_sprite.setRotation(sf::degrees(enemy->getRotation()-180.0f));
        if (enemy->lastTakenDamage() <= DMG_ANIMATION_DURATION) {
            enemy_sprite.setColor(sf::Color::Red);
        }
        window.draw(enemy_sprite);
    }

    window.draw(player_sprite);

    for (auto const& collision_explosion : collision_explosions) {
        sf::CircleShape circle(collision_explosion.second / EXPLOSION_ANIMATION_DURATION * 20.0f);
        circle.setOrigin(circle.getLocalBounds().getCenter());
        circle.setFillColor(sf::Color::Yellow);
        circle.setPosition({width / 2.0f + collision_explosion.first.x, height / 2.0f + collision_explosion.first.y});
        window.draw(circle);
    }
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
        std::unique_ptr<sf::Shape> bullet_hitbox;

        if (bullet.getSize().x == bullet.getSize().y) {
            bullet_hitbox = std::make_unique<sf::CircleShape>(bullet.getHitbox().size().x / 2.0f);
        } else {
            bullet_hitbox = std::make_unique<sf::RectangleShape>(sf::Vector2f{bullet.getSize().x, bullet.getSize().y});
        }
        bullet_hitbox->setOutlineThickness(3.0f);
        bullet_hitbox->setFillColor(sf::Color::Transparent);
        bullet_hitbox->setOrigin(bullet_hitbox->getLocalBounds().getCenter());
        bullet_hitbox->setPosition({width / 2.0f + bullet.getPosition().x, height / 2.0f + bullet.getPosition().y});
        window.draw(*bullet_hitbox);
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

bool GameLoop::exited() const { return has_exited; }