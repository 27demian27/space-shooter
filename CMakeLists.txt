cmake_minimum_required(VERSION 3.28)
project(SpaceShooter LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(
    main src/main.cpp /
    
    src/GameLoop.h /
    src/GameLoop.cpp /

    src/Player/Player.h /
    src/Player/Player.cpp /

    src/PlayArea/PlayArea.h /
    src/PlayArea/PlayArea.cpp /

    src/Bullet/Bullet.h /
    src/Bullet/Bullet.cpp /

    src/Entities/Entity.h /
    src/Entities/Entity.cpp /
    src/Entities/NormalEnemy.h /
    src/Entities/NormalEnemy.cpp /

    src/Shooter.h /
    src/Collidable.h /
    src/EnemyTypes.h /

    src/Graphics/HealthBar.h /
    src/Graphics/HealthBar.cpp /
    src/Graphics/BoostBar.h /

    src/Utils/Vector2.h /
    src/Utils/Rect.h /
    src/Utils/Rect.cpp /
    src/Utils/Circle.h /
    src/Utils/Circle.cpp /
    src/Utils/Hitbox.h /
    src/Utils/ShipType.h /

    src/Scripts/Script.h /
    src/Scripts/Script.cpp /
    src/Scripts/AsteroidScript.h /
    src/Scripts/AsteroidScript.cpp
    
    )
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

