cmake_minimum_required(VERSION 3.14)
project(TestGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
find_package(raylib REQUIRED)
include_directories(include)


set(GAME_SOURCES
    src/core/Engine.cpp
    src/raylib_renderer.cpp
    include/rogue/entities/Entity.cpp
    include/rogue/entities/Monster.cpp
    src/entities/Player.cpp
    include/rogue/world/Map.cpp
    include/rogue/core/Command.cpp
    src/ParticleSystem.cpp
)


add_executable(testgame src/main.cpp ${GAME_SOURCES})
target_link_libraries(testgame raylib)

add_executable(game_tests
    tests/test_map.cpp
    tests/test_player.cpp
    tests/test_particles.cpp
    tests/test_factory.cpp
    tests/test_commands.cpp
    ${GAME_SOURCES}
)
target_link_libraries(game_tests GTest::gtest_main raylib)
target_include_directories(game_tests PRIVATE include)

include(GoogleTest)
gtest_discover_tests(game_tests)
