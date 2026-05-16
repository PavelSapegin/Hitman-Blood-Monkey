cmake_minimum_required(VERSION 3.14)
project(TestGame CXX)

# Enable digital C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

include_directories(include)

add_executable(testgame 
    src/main.cpp
    src/core/Engine.cpp
    src/raylib_renderer.cpp
    include/rogue/entities/Entity.cpp
    include/rogue/entities/Monster.cpp
    src/entities/Player.cpp
    include/rogue/world/Map.cpp
    include/rogue/core/Command.cpp
    src/ParticleSystem.cpp
    include/rogue/TextureManager.h
)

target_link_libraries(testgame raylib)
