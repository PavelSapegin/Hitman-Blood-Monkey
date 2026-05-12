cmake_minimum_required(VERSION 3.14)
project(TestGame CXX)

# Включаем современный стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)

include_directories(include)

add_executable(testgame 
    src/main.cpp
    src/core/Engine.cpp
    src/ncurses_renderer.cpp
    include/rogue/entities/Entity.cpp
    include/rogue/entities/Monster.cpp
    include/rogue/entities/Player.cpp
    include/rogue/world/Map.cpp
    include/rogue/core/Command.cpp
    
)


target_link_libraries(testgame ${CURSES_LIBRARIES})
