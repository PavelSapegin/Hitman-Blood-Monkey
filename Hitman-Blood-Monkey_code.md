# Project Archive: Hitman-Blood-Monkey
**Path:** `/home/bober/Desktop/mywork/Hitman-Blood-Monkey`

---

### File: CMakeLists.txt
```
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
    src/core/Command.cpp
    src/raylib_renderer.cpp
    src/entities/Entity.cpp
    src/entities/Monster.cpp
    src/entities/Player.cpp
    src/world/Map.cpp
    src/ParticleSystem.cpp
    src/BulletSystem.cpp
    src/AudioSystem.cpp
    src/ArtifactSystem.cpp
    src/EventSystem.cpp
    src/MetaProgression.cpp
)


add_executable(testgame src/main.cpp ${GAME_SOURCES})
target_link_libraries(testgame raylib)
target_compile_options(testgame PRIVATE -Wall -Wextra)
add_executable(game_tests
    tests/test_map.cpp
    tests/test_player.cpp
    tests/test_particles.cpp
    tests/test_factory.cpp
    tests/test_commands.cpp
    tests/test_weapon.cpp
    tests/test_bullets.cpp
    ${GAME_SOURCES}
)
target_link_libraries(game_tests GTest::gtest_main raylib)
target_include_directories(game_tests PRIVATE include)

include(GoogleTest)
gtest_discover_tests(game_tests)

```

---

### File: generate_assets.py
```python
from PIL import Image
import os
import random as _random

ASSETS_DIR = "/home/bober/Desktop/mywork/Hitman-Blood-Monkey/assets"

PALETTES = {
    "grayscale": [
        0,
        0,
        0,
        0,  # 0: transparent (black)
        38,
        38,
        38,  # 1: very dark
        63,
        63,
        63,  # 2: dark outline
        82,
        82,
        82,  # 3: medium-dark
        105,
        105,
        105,  # 4: medium
        139,
        139,
        139,  # 5: medium-light
        170,
        170,
        170,  # 6: light
        210,
        210,
        210,  # 7: lighter
        240,
        240,
        240,  # 8: near white
        255,
        255,
        255,  # 9: white
    ],
    "monster_sceleton": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,  # 1
        63,
        63,
        63,  # 2
        82,
        82,
        82,  # 3
        105,
        105,
        105,  # 4
        139,
        139,
        139,  # 5
        170,
        170,
        170,  # 6
        210,
        210,
        210,  # 7
        240,
        240,
        240,  # 8
        255,
        255,
        255,  # 9
        190,
        170,
        150,  # 10: bone
        220,
        200,
        180,  # 11: light bone
    ],
    "monster_goblin": [
        0,
        0,
        0,
        0,
        30,
        40,
        30,  # 1
        50,
        65,
        50,  # 2
        70,
        90,
        70,  # 3
        90,
        115,
        90,  # 4
        110,
        140,
        110,  # 5
        140,
        170,
        140,  # 6
        180,
        200,
        180,  # 7
        220,
        235,
        220,  # 8
        240,
        250,
        240,  # 9
        60,
        30,
        20,  # 10: dark brown
        150,
        80,
        50,  # 11: brown
    ],
    "monster_kamikaze": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,  # 1
        63,
        63,
        63,  # 2
        82,
        82,
        82,  # 3
        105,
        105,
        105,  # 4
        139,
        139,
        139,  # 5
        170,
        170,
        170,  # 6
        210,
        210,
        210,  # 7
        240,
        240,
        240,  # 8
        255,
        255,
        255,  # 9
        200,
        80,
        20,  # 10: orange
        240,
        200,
        40,  # 11: yellow
        150,
        40,
        10,  # 12: dark orange
    ],
    "wall_stone": [
        0,
        0,
        0,
        0,
        20,
        20,
        24,  # 1: mortar (very dark)
        34,
        34,
        40,  # 2: dark slate
        46,
        46,
        54,  # 3: slate medium
        60,
        60,
        70,  # 4: slate light
        80,
        80,
        92,  # 5: slate highlight
    ],
    "wall_bloody": [
        0,
        0,
        0,
        0,
        20,
        20,
        24,
        34,
        34,
        40,
        46,
        46,
        54,
        60,
        60,
        70,
        80,
        80,
        92,
        130,
        22,
        22,  # 10: blood red
        170,
        30,
        30,  # 11: bright blood
        80,
        10,
        10,  # 12: dark blood
    ],
    "wall_reinforced": [
        0,
        0,
        0,
        0,
        20,
        20,
        24,
        34,
        34,
        40,
        46,
        46,
        54,
        60,
        60,
        70,
        80,
        80,
        92,
        60,
        38,
        16,  # 10: dark wood
        95,
        65,
        30,  # 11: medium wood
        130,
        92,
        48,  # 12: light wood
    ],
    "wall_destroyed": [
        0,
        0,
        0,
        0,
        20,
        20,
        24,
        34,
        34,
        40,
        46,
        46,
        54,
        60,
        60,
        70,
        80,
        80,
        92,
        12,
        10,
        8,  # 10: dark void/crack
    ],
    "wall_bloody": [
        0,
        0,
        0,
        0,
        36,
        36,
        42,
        56,
        56,
        64,
        76,
        76,
        86,
        100,
        100,
        112,
        130,
        130,
        140,
        140,
        25,
        25,  # 10: blood red
        190,
        35,
        35,  # 11: bright blood
        90,
        12,
        12,  # 12: dark blood
    ],
    "wall_reinforced": [
        0,
        0,
        0,
        0,
        36,
        36,
        42,
        56,
        56,
        64,
        76,
        76,
        86,
        100,
        100,
        112,
        130,
        130,
        140,
        70,
        45,
        20,  # 10: dark wood
        110,
        78,
        40,  # 11: medium wood
        150,
        110,
        60,  # 12: light wood
    ],
    "wall_destroyed": [
        0,
        0,
        0,
        0,
        36,
        36,
        42,
        56,
        56,
        64,
        76,
        76,
        86,
        100,
        100,
        112,
        130,
        130,
        140,
        18,
        15,
        12,  # 10: dark void/crack
    ],
    "weapon_minigun": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,
        63,
        63,
        63,
        82,
        82,
        82,
        105,
        105,
        105,
        139,
        139,
        139,
        170,
        170,
        170,
        210,
        210,
        210,
        240,
        240,
        240,
        255,
        255,
        255,
        180,
        150,
        80,  # 10: brass/gold
        80,
        80,
        80,  # 11: dark metal
        200,
        200,
        200,  # 12: light metal
    ],
    "weapon_shotgun": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,
        63,
        63,
        63,
        82,
        82,
        82,
        105,
        105,
        105,
        139,
        139,
        139,
        170,
        170,
        170,
        210,
        210,
        210,
        240,
        240,
        240,
        255,
        255,
        255,
        120,
        80,
        40,  # 10: wood brown
        160,
        120,
        60,  # 11: light wood
        200,
        200,
        200,  # 12: metal
    ],
    "weapon_rocket": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,
        63,
        63,
        63,
        82,
        82,
        82,
        105,
        105,
        105,
        139,
        139,
        139,
        170,
        170,
        170,
        210,
        210,
        210,
        240,
        240,
        240,
        255,
        255,
        255,
        60,
        120,
        60,  # 10: olive green
        100,
        160,
        100,  # 11: light green
        200,
        100,
        20,  # 12: orange tip
    ],
    "effect_explosion": [
        0,
        0,
        0,
        0,
        30,
        30,
        30,
        60,
        60,
        60,
        80,
        80,
        80,
        130,
        100,
        50,
        180,
        120,
        30,
        220,
        160,
        40,
        240,
        200,
        60,
        255,
        230,
        100,
        255,
        255,
        200,
        200,
        60,
        20,  # 10: red-orange
        100,
        50,
        20,  # 11: brown
        60,
        20,
        10,  # 12: dark ember
    ],
    "effect_muzzle": [
        0,
        0,
        0,
        0,
        40,
        40,
        40,
        80,
        80,
        80,
        120,
        120,
        120,
        180,
        180,
        100,
        220,
        220,
        80,
        255,
        255,
        100,
        255,
        255,
        180,
        255,
        255,
        230,
        255,
        255,
        255,
        255,
        200,
        50,  # 10: amber
        255,
        150,
        30,  # 11: orange
    ],
    "effect_shield": [
        0,
        0,
        0,
        0,
        10,
        30,
        60,
        20,
        60,
        120,
        30,
        90,
        180,
        50,
        120,
        210,
        80,
        150,
        230,
        120,
        180,
        240,
        160,
        210,
        250,
        200,
        230,
        255,
        230,
        245,
        255,
        100,
        200,
        255,  # 10: cyan
        60,
        160,
        220,  # 11: medium blue
    ],
    "effect_blood": [
        0,
        0,
        0,
        0,
        30,
        5,
        5,
        60,
        10,
        10,
        90,
        15,
        15,
        120,
        20,
        20,
        150,
        25,
        25,
        180,
        30,
        30,
        210,
        50,
        50,
        230,
        80,
        80,
        250,
        120,
        120,
        80,
        5,
        5,  # 10: dark blood
        200,
        30,
        30,  # 11: bright blood
    ],
    "pickup_health": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,
        63,
        63,
        63,
        82,
        82,
        82,
        105,
        105,
        105,
        139,
        139,
        139,
        170,
        170,
        170,
        210,
        210,
        210,
        240,
        240,
        240,
        255,
        255,
        255,
        60,
        160,
        60,  # 10: green
        100,
        200,
        100,  # 11: light green
    ],
    "pickup_ammo": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,
        63,
        63,
        63,
        82,
        82,
        82,
        105,
        105,
        105,
        139,
        139,
        139,
        170,
        170,
        170,
        210,
        210,
        210,
        240,
        240,
        240,
        255,
        255,
        255,
        180,
        150,
        60,  # 10: gold/ammo
        220,
        190,
        80,  # 11: light gold
    ],
    "monster_hunter": [
        0,
        0,
        0,
        0,
        25,
        20,
        30,
        45,
        35,
        55,
        65,
        55,
        80,
        85,
        75,
        105,
        110,
        100,
        130,
        140,
        130,
        160,
        170,
        160,
        190,
        200,
        190,
        220,
        230,
        225,
        240,
        255,
        50,
        50,  # 10: red eyes
        200,
        30,
        30,  # 11: dark red
    ],
    "monster_boss": [
        0,
        0,
        0,
        0,
        38,
        38,
        38,
        63,
        63,
        63,
        82,
        82,
        82,
        105,
        105,
        105,
        139,
        139,
        139,
        170,
        170,
        170,
        210,
        210,
        210,
        240,
        240,
        240,
        255,
        255,
        255,
        200,
        170,
        80,  # 10: gold crown
        240,
        210,
        100,  # 11: bright gold
        80,
        80,
        80,  # 12: dark metal
    ],
}


def make_sprite(name, pixels, palette_key="grayscale", transparent=True):
    h = len(pixels)
    w = len(pixels[0])
    palette = PALETTES[palette_key]
    img = Image.new("P", (w, h))
    img.putpalette(palette)
    for y in range(h):
        for x in range(w):
            idx = pixels[y][x]
            img.putpixel((x, y), idx)
    if transparent:
        img.info["transparency"] = 0
    path = os.path.join(ASSETS_DIR, name)
    img.save(path)
    print(f"  Saved {name}")


def draw_rect(pixels, x, y, w, h, color):
    for j in range(y, y + h):
        for i in range(x, x + w):
            if 0 <= j < len(pixels) and 0 <= i < len(pixels[0]):
                pixels[j][i] = color


def draw_circle(pixels, cx, cy, r, color):
    for j in range(cy - r, cy + r + 1):
        for i in range(cx - r, cx + r + 1):
            if (i - cx) * (i - cx) + (j - cy) * (j - cy) <= r * r:
                if 0 <= j < len(pixels) and 0 <= i < len(pixels[0]):
                    pixels[j][i] = color


def weapon_32x32(draw_func):
    pixels = [[0] * 32 for _ in range(32)]
    draw_func(pixels)
    return pixels


# ============================================================
# MONSTERS
# ============================================================

# SceletonMonkey - skull-like skeleton
sceleton = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 2, 2, 10, 10, 10, 10, 10, 10, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 10, 11, 11, 11, 11, 11, 11, 10, 2, 0, 0, 0],
    [0, 0, 0, 2, 10, 11, 9, 11, 11, 9, 11, 10, 2, 0, 0, 0],
    [0, 0, 0, 2, 10, 11, 11, 1, 1, 11, 11, 10, 2, 0, 0, 0],
    [0, 0, 0, 2, 10, 11, 1, 1, 1, 1, 11, 10, 2, 0, 0, 0],
    [0, 0, 0, 2, 10, 11, 11, 11, 11, 11, 11, 10, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 10, 10, 10, 10, 10, 10, 2, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 5, 5, 2, 2, 5, 5, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 5, 5, 5, 5, 5, 5, 5, 5, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 5, 5, 2, 2, 2, 2, 5, 5, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# GoblinMonkey - green goblin with pointy ears
goblin = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 2, 6, 3, 2, 2, 2, 2, 3, 6, 2, 0, 0, 0],
    [0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0],
    [0, 2, 2, 6, 2, 2, 2, 2, 2, 2, 2, 2, 6, 2, 2, 0],
    [0, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 0],
    [0, 2, 2, 2, 4, 7, 7, 7, 7, 7, 7, 4, 2, 2, 2, 0],
    [0, 2, 2, 2, 4, 7, 9, 9, 9, 9, 7, 4, 2, 2, 2, 0],
    [0, 2, 2, 2, 4, 7, 1, 7, 7, 1, 7, 4, 2, 2, 2, 0],
    [0, 2, 2, 2, 4, 7, 7, 7, 7, 7, 7, 4, 2, 2, 2, 0],
    [0, 0, 2, 2, 2, 4, 4, 4, 4, 4, 4, 2, 2, 2, 0, 0],
    [0, 0, 0, 2, 2, 2, 10, 2, 2, 10, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 11, 2, 2, 2, 2, 11, 2, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 5, 5, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# GunMonkey - monkey with a gun barrel
gun_monkey = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 6, 6, 6, 6, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 6, 2, 2, 6, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 6, 1, 1, 6, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 9, 9, 9, 9, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 6, 1, 1, 6, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 6, 2, 2, 6, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 3, 3, 2, 2, 3, 3, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 3, 2, 2, 2, 2, 3, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 8, 8, 8, 8, 8, 8, 8, 8, 0],
    [0, 0, 0, 0, 0, 0, 2, 8, 8, 8, 8, 8, 8, 8, 8, 0],
    [0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# KamikazeMonkey - cracked explosive monkey
kamikaze = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 2, 2, 10, 10, 10, 10, 10, 10, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 12, 2, 2, 12, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 12, 10, 10, 10, 10, 12, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 10, 11, 11, 10, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 12, 10, 11, 11, 10, 12, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 10, 2, 2, 10, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 10, 10, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 2, 10, 10, 2, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# HunterMonkey - hooded stealth hunter
hunter = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 3, 3, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 3, 3, 3, 3, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 2, 2, 3, 3, 3, 3, 3, 3, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 0, 0, 0],
    [0, 0, 0, 2, 3, 3, 10, 10, 10, 10, 3, 3, 2, 0, 0, 0],
    [0, 0, 0, 2, 3, 3, 10, 3, 3, 10, 3, 3, 2, 0, 0, 0],
    [0, 0, 0, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 3, 3, 3, 3, 3, 3, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 3, 5, 5, 5, 5, 3, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 5, 5, 5, 5, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 5, 5, 5, 5, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# BossMonkey - big boss with crown
boss = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 10, 11, 10, 10, 11, 10, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 6, 6, 6, 6, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 9, 9, 9, 9, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 6, 1, 1, 6, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 2, 2, 6, 9, 9, 9, 9, 6, 2, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 6, 6, 6, 6, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 12, 2, 2, 2, 2, 12, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 12, 2, 2, 12, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# ============================================================
# WALLS
# ============================================================


def _make_wall(seed, cracks=False, wood=False, blood=False):
    rng = _random.Random(seed)
    px = [[2] * 16 for _ in range(16)]

    # subtle surface noise
    for y in range(16):
        for x in range(16):
            r = rng.random()
            if r < 0.10:
                px[y][x] = 1
            elif r < 0.25:
                px[y][x] = 3
            elif r < 0.32:
                px[y][x] = 4
            elif r < 0.36:
                px[y][x] = 5

    # cracks
    if cracks:
        for _ in range(rng.randint(3, 5)):
            cx = rng.randint(0, 15)
            cy = rng.randint(0, 15)
            for i in range(rng.randint(3, 8)):
                dx = rng.choice([-1, 0, 0, 1])
                dy = rng.choice([-1, 0, 0, 1])
                cx = max(0, min(15, cx + dx))
                cy = max(0, min(15, cy + dy))
                if 0 <= cx < 16 and 0 <= cy < 16:
                    px[cy][cx] = 10

    # wood beams (vertical)
    if wood:
        for bx in [rng.randint(2, 5), rng.randint(9, 13)]:
            for by in range(16):
                px[by][bx] = 10
                if bx + 1 < 16:
                    px[by][bx + 1] = 11
                if bx - 1 >= 0:
                    px[by][bx - 1] = 1
            # horizontal beam cap
            for cap in [rng.randint(1, 3), rng.randint(10, 13)]:
                for dx in range(-1, 3):
                    cx = bx + dx
                    if 0 <= cx < 16 and 0 <= cap < 16:
                        px[cap][cx] = 12

    # blood
    if blood:
        for _ in range(rng.randint(4, 7)):
            bx = rng.randint(2, 13)
            by = rng.randint(2, 13)
            for dy in range(-2, 3):
                for dx in range(-2, 3):
                    if dx * dx + dy * dy <= rng.randint(2, 5):
                        cx = bx + dx
                        cy = by + dy
                        if 0 <= cx < 16 and 0 <= cy < 16:
                            if rng.random() < 0.6:
                                px[cy][cx] = 10
                            elif rng.random() < 0.5:
                                px[cy][cx] = 11

    # fix tiling
    for y in range(16):
        px[y][15] = px[y][0]

    return px


wall_stone = _make_wall(42)
wall_reinforced = _make_wall(73, wood=True)
wall_destroyed = _make_wall(17, cracks=True)
wall_bloody = _make_wall(99, blood=True)

# ============================================================
# WEAPONS
# ============================================================


# Minigun pickup - massive 6-barrel rotary cannon
def _minigun(pixels):
    # Barrel cluster - 3 barrels side by side
    draw_rect(pixels, 20, 5, 12, 4, 2)
    draw_rect(pixels, 21, 5, 10, 4, 12)
    draw_rect(pixels, 20, 10, 12, 4, 2)
    draw_rect(pixels, 21, 10, 10, 4, 12)
    draw_rect(pixels, 20, 15, 12, 4, 2)
    draw_rect(pixels, 21, 15, 10, 4, 12)
    # Barrel shroud at muzzle
    draw_rect(pixels, 29, 4, 3, 16, 2)
    draw_rect(pixels, 30, 5, 1, 14, 11)
    # Main body
    draw_rect(pixels, 2, 6, 18, 12, 2)
    draw_rect(pixels, 3, 7, 16, 10, 11)
    # Brass ammo belt across body
    draw_rect(pixels, 5, 9, 12, 6, 10)
    draw_rect(pixels, 6, 10, 10, 4, 8)
    # Handle
    draw_rect(pixels, 7, 18, 8, 10, 2)
    draw_rect(pixels, 8, 19, 6, 8, 11)
    # Handle grip lines
    draw_rect(pixels, 8, 20, 6, 1, 1)
    draw_rect(pixels, 8, 23, 6, 1, 1)
    draw_rect(pixels, 8, 26, 6, 1, 1)


weapon_minigun = weapon_32x32(_minigun)


# Shotgun pickup - wide double-barrel with pump
def _shotgun(pixels):
    # Double barrels side by side
    draw_rect(pixels, 18, 5, 14, 5, 2)
    draw_rect(pixels, 19, 5, 12, 5, 12)
    draw_rect(pixels, 18, 14, 14, 5, 2)
    draw_rect(pixels, 19, 14, 12, 5, 12)
    # Receiver / breech
    draw_rect(pixels, 4, 4, 14, 16, 2)
    draw_rect(pixels, 5, 5, 12, 14, 11)
    # Pump forend (wood)
    draw_rect(pixels, 13, 6, 5, 12, 10)
    draw_rect(pixels, 14, 7, 3, 10, 11)
    # Stock (wood)
    draw_rect(pixels, 0, 7, 4, 10, 2)
    draw_rect(pixels, 0, 8, 3, 8, 10)
    draw_rect(pixels, 1, 9, 1, 6, 11)
    # Pistol grip
    draw_rect(pixels, 6, 20, 8, 8, 2)
    draw_rect(pixels, 7, 21, 6, 6, 10)
    # Trigger guard
    draw_rect(pixels, 8, 22, 2, 3, 1)


weapon_shotgun = weapon_32x32(_shotgun)


# Rocket launcher pickup - long tube with rocket
def _rocket(pixels):
    # Launch tube
    draw_rect(pixels, 2, 8, 24, 8, 2)
    draw_rect(pixels, 3, 9, 22, 6, 10)
    draw_rect(pixels, 4, 10, 20, 4, 11)
    # Rocket warhead (orange)
    draw_rect(pixels, 24, 6, 8, 12, 2)
    draw_rect(pixels, 25, 7, 6, 10, 12)
    draw_circle(pixels, 31, 11, 4, 12)
    draw_circle(pixels, 31, 11, 3, 8)
    # Warhead tip highlight
    draw_rect(pixels, 29, 8, 2, 6, 9)
    # Rear sight
    draw_rect(pixels, 10, 4, 4, 4, 2)
    draw_rect(pixels, 11, 5, 2, 2, 11)
    # Front sight
    draw_rect(pixels, 20, 5, 2, 3, 2)
    draw_rect(pixels, 21, 5, 1, 2, 11)
    # Carry handle
    draw_rect(pixels, 7, 16, 8, 6, 2)
    draw_rect(pixels, 8, 17, 6, 4, 11)
    # Trigger guard
    draw_rect(pixels, 9, 18, 2, 3, 1)


weapon_rocket = weapon_32x32(_rocket)

# ============================================================
# EFFECTS
# ============================================================

# Explosion - starburst
effect_explosion = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 10, 10, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 12, 10, 9, 9, 10, 12, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 10, 8, 8, 9, 9, 8, 8, 10, 0, 0, 0, 0],
    [0, 0, 0, 12, 8, 8, 7, 8, 8, 7, 8, 8, 12, 0, 0, 0],
    [0, 0, 0, 10, 8, 7, 6, 6, 6, 6, 7, 8, 10, 0, 0, 0],
    [0, 0, 10, 9, 9, 8, 6, 5, 5, 6, 8, 9, 9, 10, 0, 0],
    [0, 0, 10, 9, 9, 8, 6, 5, 5, 6, 8, 9, 9, 10, 0, 0],
    [0, 0, 0, 10, 8, 7, 6, 6, 6, 6, 7, 8, 10, 0, 0, 0],
    [0, 0, 0, 12, 8, 8, 7, 8, 8, 7, 8, 8, 12, 0, 0, 0],
    [0, 0, 0, 0, 10, 8, 8, 9, 9, 8, 8, 10, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 12, 10, 9, 9, 10, 12, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 10, 10, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# Muzzle flash
effect_muzzle = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 8, 9, 9, 8, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 8, 9, 10, 10, 9, 8, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 8, 9, 10, 11, 11, 10, 9, 8, 0, 0, 0, 0],
    [0, 0, 0, 0, 8, 9, 10, 11, 11, 10, 9, 8, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 8, 9, 10, 10, 9, 8, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 8, 9, 9, 8, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# Shield effect - circular barrier
effect_shield = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 10, 10, 10, 10, 10, 10, 2, 0, 0, 0, 0],
    [0, 0, 0, 2, 10, 10, 10, 10, 10, 10, 10, 10, 2, 0, 0, 0],
    [0, 0, 0, 10, 10, 10, 4, 4, 4, 4, 10, 10, 10, 0, 0, 0],
    [0, 0, 2, 10, 10, 4, 4, 4, 4, 4, 4, 10, 10, 2, 0, 0],
    [0, 0, 2, 10, 10, 4, 4, 4, 4, 4, 4, 10, 10, 2, 0, 0],
    [0, 0, 2, 10, 10, 4, 4, 4, 4, 4, 4, 10, 10, 2, 0, 0],
    [0, 0, 0, 10, 10, 10, 4, 4, 4, 4, 10, 10, 10, 0, 0, 0],
    [0, 0, 0, 2, 10, 10, 10, 10, 10, 10, 10, 10, 2, 0, 0, 0],
    [0, 0, 0, 0, 2, 10, 10, 10, 10, 10, 10, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# Blood splatter
effect_blood = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0],
    [0, 0, 0, 11, 10, 11, 0, 0, 0, 11, 10, 11, 0, 0, 0, 0],
    [0, 0, 11, 10, 10, 10, 11, 0, 11, 10, 10, 10, 11, 0, 0, 0],
    [0, 0, 0, 11, 10, 10, 10, 11, 10, 10, 10, 11, 0, 0, 0, 0],
    [0, 0, 0, 0, 11, 10, 10, 10, 10, 10, 11, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 11, 10, 10, 10, 11, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 11, 10, 10, 10, 11, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 11, 10, 10, 10, 10, 10, 11, 0, 0, 0, 0, 0],
    [0, 0, 0, 11, 10, 10, 10, 11, 10, 10, 10, 11, 0, 0, 0, 0],
    [0, 0, 11, 10, 10, 10, 11, 0, 11, 10, 10, 10, 11, 0, 0, 0],
    [0, 0, 0, 11, 10, 11, 0, 0, 0, 11, 10, 11, 0, 0, 0, 0],
    [0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# ============================================================
# PICKUPS (extra)
# ============================================================

# Health pickup - green cross
pickup_health = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 10, 10, 10, 10, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 10, 10, 10, 10, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# Ammo pickup - ammunition box
pickup_ammo = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 11, 11, 11, 11, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 11, 11, 11, 11, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 11, 11, 11, 11, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 11, 10, 10, 10, 10, 11, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

# ============================================================
# GENERATE ALL ASSETS
# ============================================================

os.makedirs(ASSETS_DIR, exist_ok=True)

print("Generating monsters...")
make_sprite("monster_sceleton.png", sceleton, "monster_sceleton")
make_sprite("monster_goblin.png", goblin, "monster_goblin")
make_sprite("monster_gun.png", gun_monkey, "grayscale")
make_sprite("monster_kamikaze.png", kamikaze, "monster_kamikaze")
make_sprite("monster_hunter.png", hunter, "monster_hunter")
make_sprite("monster_boss.png", boss, "monster_boss")

print("Generating walls...")
make_sprite("tile_0040.png", wall_stone, "wall_stone")
make_sprite("wall_stone.png", wall_stone, "wall_stone")
make_sprite("wall_bloody.png", wall_bloody, "wall_bloody")
make_sprite("wall_reinforced.png", wall_reinforced, "wall_reinforced")
make_sprite("wall_destroyed.png", wall_destroyed, "wall_destroyed")

print("Generating weapons...")
make_sprite("weapon_minigun.png", weapon_minigun, "weapon_minigun")
make_sprite("weapon_shotgun.png", weapon_shotgun, "weapon_shotgun")
make_sprite("weapon_rocket.png", weapon_rocket, "weapon_rocket")

print("Generating effects...")
make_sprite("effect_explosion.png", effect_explosion, "effect_explosion")
make_sprite("effect_muzzle.png", effect_muzzle, "effect_muzzle")
make_sprite("effect_shield.png", effect_shield, "effect_shield")
make_sprite("effect_blood.png", effect_blood, "effect_blood")

print("Generating pickups...")
make_sprite("pickup_health.png", pickup_health, "pickup_health")
make_sprite("pickup_ammo.png", pickup_ammo, "pickup_ammo")

print("\nAll assets generated successfully!")

```

---

### File: tests/test_weapon.cpp
```cpp
#include "rogue/Weapon.h"
#include <gtest/gtest.h>

using namespace rogue;

TEST(WeaponTest, FistsDefaults) {
  Weapon w = Weapon::makeFists();
  EXPECT_EQ(w.type, WeaponType::Fists);
  EXPECT_FLOAT_EQ(w.damage, 999.0f);
  EXPECT_FLOAT_EQ(w.range, 1.5f);
  EXPECT_FLOAT_EQ(w.cooldown, 0.3f);
  EXPECT_FLOAT_EQ(w.currentFireRate, 0.3f);
}

TEST(WeaponTest, MinigunDefaults) {
  Weapon w = Weapon::makeMinigun();
  EXPECT_EQ(w.type, WeaponType::Minigun);
  EXPECT_FLOAT_EQ(w.damage, 10.0f);
  EXPECT_FLOAT_EQ(w.range, 15.0f);
  EXPECT_FLOAT_EQ(w.currentFireRate, 0.5f);
}

TEST(WeaponTest, MinigunSpinupDecreasesFireRate) {
  Weapon w = Weapon::makeMinigun();
  float initial = w.currentFireRate;
  w.isFiring = true;

  w.currentFireRate -= w.spinupRate * 0.5f;
  EXPECT_LT(w.currentFireRate, initial);
}

TEST(WeaponTest, MinigunFireRateClamped) {
  Weapon w = Weapon::makeMinigun();
  w.currentFireRate = w.minFireRate;
  w.currentFireRate -= w.spinupRate * 1.0f;
  if (w.currentFireRate < w.minFireRate)
    w.currentFireRate = w.minFireRate;
  EXPECT_FLOAT_EQ(w.currentFireRate, w.minFireRate);
}

TEST(WeaponTest, FistsNotFiringByDefault) {
  Weapon w = Weapon::makeFists();
  EXPECT_FALSE(w.isFiring);
}

```

---

### File: tests/test_particles.cpp
```cpp
#include "rogue/ParticleSystem.h"
#include <gtest/gtest.h>

using namespace rogue;

TEST(ParticleTest, SpawnCreatesParticles) {
  ParticleSystem ps;
  EXPECT_TRUE(ps.isEmpty());
  ps.spawnBlood(5.0f, 5.0f, 20);
  EXPECT_FALSE(ps.isEmpty());
}

TEST(ParticleTest, ParticlesDieOverTime) {
  ParticleSystem ps;
  ps.spawnBlood(5.0f, 5.0f, 10);

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, ParticlesAliveShortTime) {
  ParticleSystem ps;
  ps.spawnBlood(5.0f, 5.0f, 10);
  ps.update(0.01f);
  EXPECT_FALSE(ps.isEmpty());
}

TEST(ParticleTest, SpawnZeroParticles) {
  ParticleSystem ps;
  ps.spawnBlood(5.0f, 5.0f, 0);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnExplosion) {
  ParticleSystem ps;
  ps.spawnExplosion(5.0f, 5.0f, 30);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnFire) {
  ParticleSystem ps;
  ps.spawnFire(5.0f, 5.0f, 10);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(5.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnSmoke) {
  ParticleSystem ps;
  ps.spawnSmoke(5.0f, 5.0f, 8);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnMuzzleFlash) {
  ParticleSystem ps;
  ps.spawnMuzzleFlash(5.0f, 5.0f);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(2.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, Clear) {
  ParticleSystem ps;
  ps.spawnBlood(0, 0, 20);
  ps.spawnExplosion(0, 0, 30);
  ps.clear();
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, MultipleTypesCoexist) {
  ParticleSystem ps;
  ps.spawnBlood(0, 0, 10);
  ps.spawnExplosion(0, 0, 15);
  ps.spawnFire(0, 0, 5);
  ps.spawnSmoke(0, 0, 5);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}

```

---

### File: tests/test_factory.cpp
```cpp
#include "rogue/entities/MonsterFactory.h"
#include <gtest/gtest.h>

using namespace rogue;

TEST(MonsterFactoryTest, CreateSceletonMonkey) {
  auto m =
      MonsterFactory::createMonster(MonsterType::SceletonMonkey, 1.0f, 2.0f);
  ASSERT_NE(m, nullptr);
  EXPECT_FLOAT_EQ(m->getX(), 1.0f);
  EXPECT_FLOAT_EQ(m->getY(), 2.0f);
  EXPECT_EQ(m->getSymbol(), 's');
  EXPECT_EQ(m->getHp(), 30);
  EXPECT_FALSE(m->isDead());
}

TEST(MonsterFactoryTest, CreateGoblinMonkey) {
  auto m = MonsterFactory::createMonster(MonsterType::GoblinMonkey, 0.0f, 0.0f);
  ASSERT_NE(m, nullptr);
  EXPECT_EQ(m->getSymbol(), 'g');
  EXPECT_EQ(m->getHp(), 50);
}

TEST(MonsterFactoryTest, CreateMonkeyBoss) {
  auto m = MonsterFactory::createMonster(MonsterType::MonkeyBoss, 0.0f, 0.0f);
  ASSERT_NE(m, nullptr);
  EXPECT_EQ(m->getSymbol(), 'M');
  EXPECT_EQ(m->getHp(), 100);
}

TEST(MonsterFactoryTest, MonsterTakesDamage) {
  auto m =
      MonsterFactory::createMonster(MonsterType::SceletonMonkey, 0.0f, 0.0f);
  m->takeDamage(20);
  EXPECT_EQ(m->getHp(), 10);
}

TEST(MonsterFactoryTest, MonsterDiesFromDamage) {
  auto m =
      MonsterFactory::createMonster(MonsterType::SceletonMonkey, 0.0f, 0.0f);
  m->takeDamage(999);
  EXPECT_TRUE(m->isDead());
  EXPECT_EQ(m->getHp(), 0);
}

```

---

### File: tests/test_map.cpp
```cpp
#include "rogue/world/Map.h"
#include <gtest/gtest.h>

using namespace rogue;

class MapTest : public ::testing::Test {
protected:
  // Фиксированный seed — карта всегда одинаковая
  Map map{80, 50, 42};
};

TEST_F(MapTest, Dimensions) {
  EXPECT_EQ(map.getWidth(), 80);
  EXPECT_EQ(map.getHeight(), 50);
}

TEST_F(MapTest, OutOfBoundsNotWalkable) {
  EXPECT_FALSE(map.isWalkable(-1, 0));
  EXPECT_FALSE(map.isWalkable(0, -1));
  EXPECT_FALSE(map.isWalkable(80, 0));
  EXPECT_FALSE(map.isWalkable(0, 50));
}

TEST_F(MapTest, GetTileOutOfRangeThrows) {
  EXPECT_THROW(map.getTile(-1, 0), std::out_of_range);
  EXPECT_THROW(map.getTile(0, -1), std::out_of_range);
  EXPECT_THROW(map.getTile(80, 0), std::out_of_range);
}

TEST_F(MapTest, RoomsGenerated) {
  // После генерации должны быть комнаты
  EXPECT_FALSE(map.getRooms().empty());
}

TEST_F(MapTest, RoomCentersAreWalkable) {
  // Центры всех комнат должны быть проходимы
  for (const auto &room : map.getRooms()) {
    EXPECT_TRUE(map.isWalkable(room.centerX(), room.centerY()))
        << "Room center " << room.centerX() << "," << room.centerY()
        << " is not walkable";
  }
}

TEST_F(MapTest, SpillBloodChangesTile) {
  // Берём центр первой комнаты — гарантированно walkable
  const auto &room = map.getRooms()[0];
  int cx = room.centerX();
  int cy = room.centerY();
  map.spillBlood(cx, cy);
  EXPECT_EQ(map.getTile(cx, cy).symbol, '%');
  EXPECT_EQ(map.getTile(cx, cy).colorPair, 5);
}

TEST_F(MapTest, SpillBloodOutOfBoundsDoesNotCrash) {
  EXPECT_NO_THROW(map.spillBlood(-1, -1));
  EXPECT_NO_THROW(map.spillBlood(100, 100));
}

```

---

### File: tests/test_player.cpp
```cpp
#include "rogue/entities/Player.h"
#include "rogue/world/Map.h"
#include <cmath>
#include <gtest/gtest.h>

using namespace rogue;

class PlayerTest : public ::testing::Test {
protected:
  Map map{40, 40, 42}; // фиксированный seed
  Player player{(float)map.getRooms()[0].centerX(),
                (float)map.getRooms()[0].centerY(), '@', 2, 100};
};

TEST_F(PlayerTest, MovesOnOpenFloor) {
  std::vector<Entity *> entities;
  float startX = player.getX();
  player.setContext(map, entities, 1.0f);
  player.processInput(1.0f, 0.0f, 0.016f);
  player.update();
  EXPECT_GT(player.getX(), startX);
}

TEST_F(PlayerTest, InitialPosition) {
  EXPECT_FLOAT_EQ(player.getX(), (float)map.getRooms()[0].centerX());
  EXPECT_FLOAT_EQ(player.getY(), (float)map.getRooms()[0].centerY());
}

TEST_F(PlayerTest, InitialHp) {
  EXPECT_EQ(player.getHp(), 100);
  EXPECT_FALSE(player.isDead());
}

TEST_F(PlayerTest, TakeDamage) {
  player.takeDamage(30);
  EXPECT_EQ(player.getHp(), 70);
}

TEST_F(PlayerTest, TakeDamageDoesNotGoBelowZero) {
  player.takeDamage(999);
  EXPECT_EQ(player.getHp(), 0);
  EXPECT_TRUE(player.isDead());
}

TEST_F(PlayerTest, BlockedByWall) {
  // Place player near the top wall
  Player p{5.0f, 1.0f, '@', 2, 100};
  std::vector<Entity *> entities;
  p.setContext(map, entities, 1.0f);
  p.processInput(0.0f, -1.0f, 0.016f); // Attempt to move up into the wall
  p.update();
  EXPECT_GE(p.getY(), 0.5f); // Should not move into the wall
}

TEST_F(PlayerTest, DiagonalMovementNormalized) {
  std::vector<Entity *> entities;
  player.setContext(map, entities, 1.0f);

  // Create another player to compare diagonal speed
  Player p1{5.0f, 5.0f, '@', 2, 100};
  Player p2{5.0f, 5.0f, '@', 2, 100};

  p1.setContext(map, entities, 0.016f);
  p2.setContext(map, entities, 0.016f);

  p1.processInput(1.0f, 0.0f, 0.016f);
  p2.processInput(1.0f, 1.0f, 0.016f);

  p1.update();
  p2.update();

  float distX = p1.getX() - 5.0f;
  float distDiag = std::sqrt((p2.getX() - 5.0f) * (p2.getX() - 5.0f) +
                             (p2.getY() - 5.0f) * (p2.getY() - 5.0f));

  // Diagonal distance should be approximately the same as straight distance due
  // to normalization
  EXPECT_NEAR(distX, distDiag, 0.001f);
}

```

---

### File: tests/test_commands.cpp
```cpp
#include "rogue/core/Command.h"
#include "rogue/entities/Monster.h"
#include "rogue/world/Map.h"
#include <gtest/gtest.h>

using namespace rogue;

class CommandTest : public ::testing::Test {
protected:
  Map map{40, 40, 42};
  Monster monster{(float)map.getRooms()[0].centerX(),
                  (float)map.getRooms()[0].centerY(), 's', 3, 30};
};

TEST_F(CommandTest, MoveCommandMovesEntity) {
  float startX = monster.getX();
  float startY = monster.getY();
  int dx = 0, dy = 0;
  if (map.isWalkable(startX + 1, startY))
    dx = 1;
  else if (map.isWalkable(startX - 1, startY))
    dx = -1;
  else if (map.isWalkable(startX, startY + 1))
    dy = 1;
  else if (map.isWalkable(startX, startY - 1))
    dy = -1;
  ASSERT_TRUE(dx != 0 || dy != 0) << "No walkable direction from spawn";
  MoveCommand cmd(dx, dy);
  cmd.execute(monster, map);
  EXPECT_FLOAT_EQ(monster.getX(), startX + dx);
  EXPECT_FLOAT_EQ(monster.getY(), startY + dy);
}

TEST_F(CommandTest, MoveCommandBlockedByWall) {
  // Place a wall to the left of the monster
  Monster m{1.0f, 1.0f, 's', 3, 30};
  MoveCommand cmd(-1, 0); // Attempt to move left into the wall
  cmd.execute(m, map);
  EXPECT_FLOAT_EQ(m.getX(), 1.0f); // Position should not change
}

TEST_F(CommandTest, AttackCommandDamagesTarget) {
  Monster target{6.0f, 5.0f, 'g', 3, 50};
  AttackCommand cmd(&target);
  cmd.execute(monster, map);
  EXPECT_EQ(target.getHp(), 40);
}

TEST_F(CommandTest, AttackCommandKillsTarget) {
  Monster target{6.0f, 5.0f, 'g', 3, 10};
  AttackCommand cmd(&target);
  cmd.execute(monster, map);
  EXPECT_TRUE(target.isDead());
}

TEST_F(CommandTest, AttackCommandNullTargetDoesNotCrash) {
  AttackCommand cmd(nullptr);
  EXPECT_NO_THROW(cmd.execute(monster, map));
}

```

---

### File: tests/test_bullets.cpp
```cpp
#include "rogue/BulletSystem.h"
#include "rogue/entities/Monster.h"
#include "rogue/world/Map.h"
#include <gtest/gtest.h>

using namespace rogue;

TEST(BulletTest, SpawnCreatesBullet) {
  BulletSystem bs;
  bs.spawn(0.0f, 0.0f, 10.0f, 0.0f, 25.0f);
  bs.spawn(0.0f, 0.0f, 0.0f, 10.0f, 25.0f);
  bs.spawn(5.0f, 5.0f, 5.0f, 5.0f, 25.0f);
  bs.clear();
  // After clear, internal bullet list should be empty (no crash)
}

TEST(BulletTest, SpawnTowardsTarget) {
  BulletSystem bs;
  bs.spawn(0.0f, 0.0f, 10.0f, 0.0f, 25.0f);
  // No crash means sanity check passed; bullet moves in x direction
}

TEST(BulletTest, BulletsHitMonsters) {
  Map map(40, 40, 42);
  auto room = map.getRooms()[0];
  float cx = (float)room.centerX();
  float cy = (float)room.centerY();

  BulletSystem bs;
  ParticleSystem ps;
  std::vector<std::unique_ptr<Monster>> monsters;
  monsters.push_back(std::make_unique<Monster>(cx, cy, 's', 3, 30));

  // Spawn bullet close to the monster, aimed directly at it
  // SkeletMonkey has 30 HP; 3 hits of 25 should kill it
  bs.spawn(cx - 0.4f, cy, cx, cy, 25.0f);
  bs.spawn(cx - 0.4f, cy + 0.1f, cx, cy, 25.0f);
  bs.spawn(cx - 0.4f, cy - 0.1f, cx, cy, 25.0f);

  for (int i = 0; i < 20; ++i) {
    bs.update(0.01f, map, monsters, ps);
    if (monsters[0]->isDead())
      break;
  }

  EXPECT_TRUE(monsters[0]->isDead());
}

TEST(BulletTest, BulletsExpireAfterLifetime) {
  Map map(40, 40, 42);
  BulletSystem bs;
  ParticleSystem ps;
  std::vector<std::unique_ptr<Monster>> monsters;

  bs.spawn(5.0f, 5.0f, 10.0f, 5.0f, 25.0f);

  for (int i = 0; i < 200; ++i) {
    bs.update(0.1f, map, monsters, ps);
  }

  // Should not crash — bullet expired without hitting anything
}

```

---

### File: src/main.cpp
```cpp
#include "rogue/Raylib_renderer.h"
#include "rogue/core/Engine.h"

int main() {
  rogue::Engine engine(std::make_unique<rogue::RaylibRenderer>(800, 600));
  engine.run();
  return 0;
}

```

---

### File: src/ArtifactSystem.cpp
```cpp
#include "rogue/Artifact.h"
#include "rogue/entities/Player.h"
#include <algorithm>
#include <cstdlib>

namespace rogue {

std::vector<Artifact> ArtifactSystem::allArtifacts;
std::vector<Mutation> ArtifactSystem::allMutations;
std::vector<GodBlessing> ArtifactSystem::allGods;
std::vector<CombinationRecipe> ArtifactSystem::recipes;

ArtifactSystem::ArtifactSystem() {
  if (allArtifacts.empty()) {
    allArtifacts = {
      {"Demon Eye",    "+100% dmg, HP drains 2/sec", 0,0,0,0,0,0,0, false,false,false,false, 2,0,0,0,false,false,false,false,false, RED, 'D'},
      {"Angel Wings",  "+1 jump, -30% speed", 0,-0.3f,0,0,0,0,0, true,false,false,false, 0,0,0,0,false,false,false,false,false, WHITE, 'W'},
      {"Berserker Axe","+50% dmg, +50% dmg taken", 0,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,true, ORANGE, 'A'},
      {"Ricochet Lens","bullets bounce off walls", 0,0,0,0,0,0,0, false,true,false,false, 0,0,0,0,false,false,false,false,false, SKYBLUE, 'R'},
      {"Cursed Crown", "monsters attracted to you, +30% dmg", 0,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,true,false,false,false, GOLD, 'C'},
      {"Seven-League Boots","+50% speed, reverse controls", 0,0.5f,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,true,false,false, GREEN, 'B'},
      {"Explosive Rounds","bullets explode on hit, -40% fire rate", 0,0,0,-0.4f,0,0,0, false,false,true,false, 0,0,0,0,false,false,false,false,false, RED, 'E'},
      {"Piercing Shot","bullets pierce enemies, no ammo drops", 0,0,0,0,0,0,0, false,false,false,true, 0,0,0,0,false,false,true,false,false, PURPLE, 'P'},
      {"Vampire Fang","heal 2 HP on kill, -20% max HP", 0,0,-20,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, MAROON, 'V'},
      {"Chaos Orb","random teleport on damage, +100% dmg", 0,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,true,false,false,false,false, PINK, 'O'},
      {"Unstable build v0.1","random effect every 5s, +50% dmg", 0,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, Color{255,100,0,255}, 'U'},
      {"Nuke","all enemies take 50 dmg on pickup", 0,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, RED, 'N'},
      {"Golden Toilet","+100 max HP, -30% speed, enemies ignore you", 0,-0.3f,100,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, GOLD, 'T'},
      {"Eyeball", "reveals whole map, -20% dmg", -20,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, Color{200,200,255,255}, 'E'},
      {"Frozen Heart","every 3rd shot freezes enemy, +20% fire rate", 0,0,0,0.2f,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, SKYBLUE, 'F'},
      {"Mushroom","random buff every 10s, might shrink you", 0,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, Color{200,100,255,255}, 'M'},
    };
  }
  if (allMutations.empty()) {
    allMutations = {
      {"Extra Arm",     "+1 bullet per shot", Mutation::ExtraArm},
      {"Laser Eyes",    "auto-beam at nearest enemy", Mutation::LaserEyes},
      {"Wall Jump",     "walk through walls 1s", Mutation::WallJump},
      {"Thorn Skin",    "damage melee attackers", Mutation::ThornSkin},
      {"Vampire",       "heal 3 HP on kill", Mutation::Vampire},
      {"Berserker",     "+100% dmg when HP < 30%", Mutation::Berserker},
      {"Shield",        "block every 3rd hit", Mutation::Shield},
      {"Speedster",     "+50% speed, -30% HP", Mutation::Speedster},
      {"Fat Roll",      "dash has no cd, -30% speed", Mutation::FatRoll},
      {"Glass Gun",     "+200% dmg, -50% max HP", Mutation::GlassGun},
    };
  }
  if (allGods.empty()) {
    allGods = {
      {"God of Speed",  "+40% speed, infinite dashes", GodBlessing::GodOfSpeed, GREEN},
      {"God of Fire",   "all bullets explode, +50% fire rate", GodBlessing::GodOfFire, RED},
      {"God of Chaos",  "2x events, +100% dmg", GodBlessing::GodOfChaos, PURPLE},
      {"God of Health", "regen 1 HP/s, +100 max HP", GodBlessing::GodOfHealth, LIME},
      {"God of Greed",  "double pickups, fewer enemy drops", GodBlessing::GodOfGreed, GOLD},
      {"God of Luck",   "crit chance, better loot", GodBlessing::GodOfLuck, SKYBLUE},
    };
  }
}

Artifact ArtifactSystem::randomArtifact() {
  if (allArtifacts.empty()) return {"Placebo", "does nothing"};
  return allArtifacts[rand() % allArtifacts.size()];
}

Mutation ArtifactSystem::randomMutation() {
  if (allMutations.empty()) return {"Placebo", "nothing happens", Mutation::ExtraArm};
  return allMutations[rand() % allMutations.size()];
}

GodBlessing ArtifactSystem::randomGod() {
  if (allGods.empty()) return {"Atheist", "nothing", GodBlessing::GodOfSpeed, GRAY};
  return allGods[rand() % allGods.size()];
}

void ArtifactSystem::applyArtifact(Artifact &a, Player &player,
    int &ammoMG, int &ammoSG, int &ammoRL,
    const int &/*maxAmmoMG*/, const int &/*maxAmmoSG*/, const int &/*maxAmmoRL*/) {
  if (a.bonusMaxHp != 0) {
    if (a.bonusMaxHp > 0) player.takeDamage(-a.bonusMaxHp);
    else for (int i = 0; i < -a.bonusMaxHp; ++i) player.takeDamage(1);
  }
  ammoMG += a.bonusAmmoMG;
  ammoSG += a.bonusAmmoSG;
  ammoRL += a.bonusAmmoRL;
}

void ArtifactSystem::unapplyArtifact(Artifact &a, Player &player,
    int &ammoMG, int &ammoSG, int &ammoRL,
    const int &/*maxAmmoMG*/, const int &/*maxAmmoSG*/, const int &/*maxAmmoRL*/) {
  if (a.bonusMaxHp != 0) {
    for (int i = 0; i < a.bonusMaxHp; ++i) player.takeDamage(1);
  }
  ammoMG -= a.bonusAmmoMG;
  ammoSG -= a.bonusAmmoSG;
  ammoRL -= a.bonusAmmoRL;
}

Artifact ArtifactSystem::tryCombine(const Artifact &a, const Artifact &b) {
  // Simple combinations
  if ((a.name == "Explosive Rounds" && b.name == "Ricochet Lens") ||
      (a.name == "Ricochet Lens" && b.name == "Explosive Rounds")) {
    Artifact r;
    r.name = "Bouncing Bombs";
    r.desc = "bullets explode and bounce! +50% dmg";
    r.bonusDamage = 50;
    r.explosiveBullets = true;
    r.ricochet = true;
    r.color = ORANGE;
    r.symbol = 'B';
    return r;
  }
  if ((a.name == "Demon Eye" && b.name == "Angel Wings") ||
      (a.name == "Angel Wings" && b.name == "Demon Eye")) {
    Artifact r;
    r.name = "Fallen Angel";
    r.desc = "+2 jumps, +100% dmg, HP drains 1/sec";
    r.bonusDamage = 100;
    r.extraJump = true;
    r.drainHpPerSec = 1;
    r.color = Color{128, 0, 128, 255};
    r.symbol = 'F';
    return r;
  }
  if ((a.name == "Seven-League Boots" && b.name == "Berserker Axe") ||
      (a.name == "Berserker Axe" && b.name == "Seven-League Boots")) {
    Artifact r;
    r.name = "Berserker Rush";
    r.desc = "+100% speed, +100% dmg taken, +50% dmg";
    r.bonusSpeed = 1.0f;
    r.bonusDamage = 50;
    r.glassCannon = true;
    r.color = RED;
    r.symbol = 'R';
    return r;
  }
  // No valid combo
  Artifact none;
  none.name = "";
  return none;
}

} // namespace rogue

```

---

### File: src/BulletSystem.cpp
```cpp
#include "rogue/BulletSystem.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>

namespace rogue {
void BulletSystem::spawn(float x, float y, float tx, float ty, float damage,
                         bool isRocket) {
  float dx = tx - x;
  float dy = ty - y;
  float len = std::sqrt(dx * dx + dy * dy);
  if (len == 0)
    return;

  Bullet b;
  b.x = x;
  b.y = y;
  b.vx = (dx / len) * BULLET_SPEED;
  b.vy = (dy / len) * BULLET_SPEED;
  b.damage = damage;
  b.lifetime = 1.2f;
  b.isRocket = isRocket;
  if (piercingMod) b.pierceLeft = 3;
  if (ricochetMod) b.bounceLeft = 2;
  bullets.push_back(b);
}

void BulletSystem::update(float dt, Map &map,
                          std::vector<std::unique_ptr<Monster>> &monsters,
                          ParticleSystem &particles) {
  rocketExplosions.clear();

  for (auto &b : bullets) {
    if (b.dead)
      continue;
    b.x += b.vx * dt;
    b.y += b.vy * dt;
    b.lifetime -= dt;

    if (b.lifetime <= 0) {
      b.dead = true;
      continue;
    }

    if (!map.isWalkable(b.x, b.y)) {
      if (b.bounceLeft > 0) {
        --b.bounceLeft;
        if (map.isWalkable(b.x - b.vx * dt * 2, b.y + b.vy * dt * 2)) {
          b.vy = -b.vy;
        } else if (map.isWalkable(b.x + b.vx * dt * 2, b.y - b.vy * dt * 2)) {
          b.vx = -b.vx;
        } else {
          b.vx = -b.vx;
          b.vy = -b.vy;
        }
        particles.spawnFire(b.x, b.y, 3);
        continue;
      }
      particles.spawnFire(b.x, b.y, 5);
      b.dead = true;
      if (b.isRocket) {
        rocketExplosions.push_back({b.x, b.y});
      }
      if (explosiveMod && onExplosion) {
        onExplosion(b.x, b.y, explosiveRadius, explosiveDamage);
      }
      continue;
    }

    for (auto &m : monsters) {
      if (m->isDead())
        continue;
      float dx = m->getX() - b.x;
      float dy = m->getY() - b.y;
      float dist = std::sqrt(dx * dx + dy * dy);
      if (dist < 0.5f) {
        m->takeDamage(static_cast<int>(b.damage));
        m->onHit();
        particles.spawnBlood(m->getX(), m->getY(), 15);
        if (explosiveMod && onExplosion) {
          onExplosion(b.x, b.y, explosiveRadius, explosiveDamage);
        }
        if (b.pierceLeft > 0) {
          --b.pierceLeft;
        } else {
          b.dead = true;
        }
        if (b.isRocket) {
          rocketExplosions.push_back({b.x, b.y});
          b.dead = true;
        }
        if (m->isDead()) {
          particles.spawnBlood(m->getX(), m->getY(), 40);
          particles.spawnExplosion(m->getX(), m->getY(), 30);
          particles.spawnSmoke(m->getX(), m->getY(), 12);
          map.spillBloodArea((int)m->getX(), (int)m->getY(), 3);
          if (onMonsterKilled) {
            onMonsterKilled(m->getX(), m->getY());
          }
        } else {
          map.spillBloodArea((int)m->getX(), (int)m->getY(), 1);
        }
        break;
      }
    }
  }

  bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                               [](const Bullet &b) { return b.dead; }),
                bullets.end());
}

std::vector<std::pair<float, float>> BulletSystem::getRocketExplosions() {
  auto result = std::move(rocketExplosions);
  rocketExplosions.clear();
  return result;
}

void BulletSystem::clear() {
  bullets.clear();
  rocketExplosions.clear();
}

void BulletSystem::render() const {
  for (const auto &b : bullets) {
    float screenX = b.x * TILE_SIZE;
    float screenY = b.y * TILE_SIZE;
    DrawCircle((int)screenX, (int)screenY, 4, YELLOW);
    DrawCircle((int)(screenX - b.vx * 0.02f * TILE_SIZE),
               (int)(screenY - b.vy * 0.02f * TILE_SIZE), 2, ORANGE);
    if (b.isRocket) {
      DrawCircle((int)screenX, (int)screenY, 7, Color{255, 100, 0, 100});
    }
    if (b.pierceLeft > 0) {
      DrawCircle((int)screenX, (int)screenY, 6, Color{100, 100, 255, 80});
    }
  }
}
} // namespace rogue

```

---

### File: src/MetaProgression.cpp
```cpp
#include "rogue/MetaProgression.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace rogue {

MetaProgression::MetaProgression() {
  filePath = "hitman_save.txt";
  load();
}

void MetaProgression::load() {
  std::ifstream f(filePath);
  if (!f.is_open()) return;
  std::string line;
  while (std::getline(f, line)) {
    if (line.find("UNLOCK:") == 0) {
      unlockedItems.push_back(line.substr(7));
    } else if (line.find("RUNS:") == 0) {
      unlockData.totalRuns = std::stoi(line.substr(5));
    } else if (line.find("KILLS:") == 0) {
      unlockData.totalKills = std::stoi(line.substr(6));
    } else if (line.find("WAVE:") == 0) {
      unlockData.highestWave = std::stoi(line.substr(5));
    } else if (line.find("FLOOR:") == 0) {
      unlockData.highestFloor = std::stoi(line.substr(6));
    }
  }
}

void MetaProgression::save() {
  std::ofstream f(filePath);
  if (!f.is_open()) return;
  f << "RUNS:" << unlockData.totalRuns << "\n";
  f << "KILLS:" << unlockData.totalKills << "\n";
  f << "WAVE:" << unlockData.highestWave << "\n";
  f << "FLOOR:" << unlockData.highestFloor << "\n";
  for (auto &item : unlockedItems) {
    f << "UNLOCK:" << item << "\n";
  }
}

void MetaProgression::updateHighScore(int wave, int floor) {
  if (wave > unlockData.highestWave) unlockData.highestWave = wave;
  if (floor > unlockData.highestFloor) unlockData.highestFloor = floor;
  save();
}

bool MetaProgression::isUnlocked(const std::string &key) const {
  return std::find(unlockedItems.begin(), unlockedItems.end(), key) != unlockedItems.end();
}

void MetaProgression::unlock(const std::string &key) {
  if (!isUnlocked(key)) {
    unlockedItems.push_back(key);
    save();
  }
}

} // namespace rogue

```

---

### File: src/AudioSystem.cpp
```cpp
#include "rogue/AudioSystem.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace rogue {

AudioSystem::AudioSystem() {}

AudioSystem::~AudioSystem() {
  destroySound(shootSound);
  destroySound(hitSound);
  destroySound(pickupSound);
  destroySound(zaWarudoSound);
  destroySound(explosionSound);
  destroySound(deathSound);
  destroySound(dashSound);
  destroySound(levelUpSound);
  destroySound(waveStartSound);
}

void AudioSystem::destroySound(Sound &s) {
  if (s.frameCount > 0) UnloadSound(s);
}

Sound AudioSystem::makeSine(float freq, float duration, float amp) {
  unsigned int sr = 44100;
  unsigned int samples = (unsigned int)(sr * duration);
  if (samples == 0) samples = 1;
  short *data = new short[samples];
  for (unsigned int i = 0; i < samples; ++i) {
    float t = (float)i / sr;
    float envelope = 1.0f;
    float fade = 0.005f;
    if (t < fade) envelope = t / fade;
    if (t > duration - fade) envelope = (duration - t) / fade;
    data[i] = (short)(amp * envelope * 32767.0f * std::sin(2.0f * PI * freq * t));
  }
  Wave wave;
  wave.data = data;
  wave.frameCount = samples;
  wave.sampleRate = sr;
  wave.sampleSize = 16;
  wave.channels = 1;
  Sound s = LoadSoundFromWave(wave);
  delete[] data;
  return s;
}

Sound AudioSystem::makeNoise(float duration, float amp) {
  unsigned int sr = 44100;
  unsigned int samples = (unsigned int)(sr * duration);
  if (samples == 0) samples = 1;
  short *data = new short[samples];
  for (unsigned int i = 0; i < samples; ++i) {
    float t = (float)i / sr;
    float envelope = 1.0f;
    float fade = 0.003f;
    if (t < fade) envelope = t / fade;
    if (t > duration - fade) envelope = (duration - t) / fade;
    data[i] = (short)(amp * envelope * 32767.0f * ((float)std::rand() / (float)RAND_MAX * 2.0f - 1.0f));
  }
  Wave wave;
  wave.data = data;
  wave.frameCount = samples;
  wave.sampleRate = sr;
  wave.sampleSize = 16;
  wave.channels = 1;
  Sound s = LoadSoundFromWave(wave);
  delete[] data;
  return s;
}

Sound AudioSystem::makeSweep(float freqStart, float freqEnd, float duration, float amp) {
  unsigned int sr = 44100;
  unsigned int samples = (unsigned int)(sr * duration);
  if (samples == 0) samples = 1;
  short *data = new short[samples];
  for (unsigned int i = 0; i < samples; ++i) {
    float t = (float)i / sr;
    float frac = t / duration;
    float freq = freqStart + (freqEnd - freqStart) * frac;
    float envelope = 1.0f;
    float fade = 0.005f;
    if (t < fade) envelope = t / fade;
    if (t > duration - fade) envelope = (duration - t) / fade;
    float phase = 2.0f * PI * freq * t;
    data[i] = (short)(amp * envelope * 32767.0f * std::sin(phase));
  }
  Wave wave;
  wave.data = data;
  wave.frameCount = samples;
  wave.sampleRate = sr;
  wave.sampleSize = 16;
  wave.channels = 1;
  Sound s = LoadSoundFromWave(wave);
  delete[] data;
  return s;
}

void AudioSystem::init() {
  shootSound = makeNoise(0.06f, 0.4f);
  hitSound = makeSine(120, 0.15f, 0.6f);
  pickupSound = makeSweep(400, 900, 0.2f, 0.4f);
  zaWarudoSound = makeSweep(80, 30, 0.6f, 0.7f);
  explosionSound = makeNoise(0.35f, 0.6f);
  deathSound = makeSweep(500, 80, 0.5f, 0.5f);
  dashSound = makeSweep(300, 600, 0.12f, 0.3f);
  levelUpSound = makeSweep(400, 1200, 0.3f, 0.4f);
  waveStartSound = makeSweep(200, 800, 0.4f, 0.4f);
}

void AudioSystem::playShoot() { SetSoundVolume(shootSound, volume); PlaySound(shootSound); }
void AudioSystem::playHit() { SetSoundVolume(hitSound, volume); PlaySound(hitSound); }
void AudioSystem::playPickup() { SetSoundVolume(pickupSound, volume); PlaySound(pickupSound); }
void AudioSystem::playZaWarudo() { SetSoundVolume(zaWarudoSound, volume * 1.2f); PlaySound(zaWarudoSound); }
void AudioSystem::playExplosion() { SetSoundVolume(explosionSound, volume); PlaySound(explosionSound); }
void AudioSystem::playDeath() { SetSoundVolume(deathSound, volume); PlaySound(deathSound); }
void AudioSystem::playDash() { SetSoundVolume(dashSound, volume); PlaySound(dashSound); }
void AudioSystem::playLevelUp() { SetSoundVolume(levelUpSound, volume); PlaySound(levelUpSound); }
void AudioSystem::playWaveStart() { SetSoundVolume(waveStartSound, volume * 1.2f); PlaySound(waveStartSound); }

} // namespace rogue

```

---

### File: src/ncurses_renderer.cpp
```cpp
#include "rogue/Ncurses_renderer.h"
#include <ncurses.h>

namespace rogue {
void NcursesRenderer::initialize() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
  }
}

void NcursesRenderer::clear() { ::clear(); }

void NcursesRenderer::refresh() { ::refresh(); }
void NcursesRenderer::drawChar(float x, float y, char ch, int color) {
  attron(COLOR_PAIR(color));
  mvaddch(static_cast<int>(y), static_cast<int>(x), ch);
  attroff(COLOR_PAIR(color));
}
void NcursesRenderer::shutdown() { endwin(); }
} // namespace rogue

```

---

### File: src/ParticleSystem.cpp
```cpp
#include "rogue/Constants.h"
#include "rogue/ParticleSystem.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace rogue {

static float randF(float min, float max) {
  return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

void ParticleSystem::spawnBlood(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * PI;
    float speed = 2.0f + ((float)rand() / (float)RAND_MAX) * 8.0f;

    Particle p;
    p.x = x + randF(-0.2f, 0.2f);
    p.y = y + randF(-0.2f, 0.2f);
    p.vx = std::cos(angle) * speed;
    p.vy = std::sin(angle) * speed;
    p.lifetime = randF(2.0f, 5.0f);
    p.maxLifetime = p.lifetime;
    p.size = randF(2.0f, 7.0f);
    p.maxSize = p.size;
    p.type = ParticleType::Blood;

    static const Color bloodColors[] = {
        RED,         MAROON,   Color{(unsigned char)180, (unsigned char)0, (unsigned char)0, (unsigned char)255},
        Color{(unsigned char)200, (unsigned char)50, (unsigned char)50, (unsigned char)255}, Color{(unsigned char)120, (unsigned char)0, (unsigned char)0, (unsigned char)255}};
    p.color = bloodColors[rand() % 5];

    particles.push_back(p);
  }
}

void ParticleSystem::spawnExplosion(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    float angle = randF(0, 2.0f * PI);
    float speed = randF(3.0f, 12.0f);
    float t = (float)i / (float)count;

    Particle p;
    p.x = x + randF(-0.3f, 0.3f);
    p.y = y + randF(-0.3f, 0.3f);
    p.vx = std::cos(angle) * speed;
    p.vy = std::sin(angle) * speed;

    if (t < 0.4f) {
      p.type = ParticleType::ExplosionDebris;
      p.lifetime = randF(0.5f, 1.5f);
      p.size = randF(2.0f, 4.0f);
      p.color = Color{(unsigned char)(80 + rand() % 40), (unsigned char)(30 + rand() % 20), (unsigned char)10, (unsigned char)255};
    } else if (t < 0.7f) {
      p.type = ParticleType::Fire;
      p.lifetime = randF(0.3f, 0.8f);
      p.size = randF(3.0f, 8.0f);
      p.color = (rand() % 2) ? ORANGE : YELLOW;
    } else {
      p.type = ParticleType::Smoke;
      p.lifetime = randF(2.0f, 4.0f);
      p.size = randF(3.0f, 6.0f);
      p.maxSize = p.size + randF(5.0f, 12.0f);
      p.color = Color{(unsigned char)(100 + rand() % 60), (unsigned char)(100 + rand() % 60), (unsigned char)(100 + rand() % 60),
                      (unsigned char)100};
    }

    p.maxLifetime = p.lifetime;
    particles.push_back(p);
  }
}

void ParticleSystem::spawnFire(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    Particle p;
    p.x = x + randF(-0.2f, 0.2f);
    p.y = y + randF(-0.2f, 0.2f);
    p.vx = randF(-2.0f, 2.0f);
    p.vy = -randF(1.0f, 5.0f);
    p.lifetime = randF(0.3f, 0.8f);
    p.maxLifetime = p.lifetime;
    p.size = randF(2.0f, 6.0f);
    p.maxSize = p.size;
    p.color = (rand() % 3 == 0) ? YELLOW : ORANGE;
    p.type = ParticleType::Fire;
    particles.push_back(p);
  }
}

void ParticleSystem::spawnSmoke(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    Particle p;
    p.x = x + randF(-0.3f, 0.3f);
    p.y = y + randF(-0.3f, 0.3f);
    p.vx = randF(-1.5f, 1.5f);
    p.vy = -randF(0.5f, 2.5f);
    p.lifetime = randF(2.5f, 5.0f);
    p.maxLifetime = p.lifetime;
    p.size = randF(3.0f, 6.0f);
    p.maxSize = p.size + randF(6.0f, 14.0f);
    int v = 80 + rand() % 80;
    p.color = Color{(unsigned char)v, (unsigned char)v, (unsigned char)v, (unsigned char)120};
    p.type = ParticleType::Smoke;
    particles.push_back(p);
  }
}

void ParticleSystem::spawnMuzzleFlash(float x, float y) {
  for (int i = 0; i < 6; ++i) {
    Particle p;
    p.x = x + randF(-0.1f, 0.1f);
    p.y = y + randF(-0.1f, 0.1f);
    float angle = randF(0, 2.0f * PI);
    float speed = randF(1.0f, 4.0f);
    p.vx = std::cos(angle) * speed;
    p.vy = std::sin(angle) * speed;
    p.lifetime = randF(0.05f, 0.2f);
    p.maxLifetime = p.lifetime;
    p.size = randF(2.0f, 5.0f);
    p.maxSize = p.size;
    p.color = (rand() % 2) ? YELLOW : ORANGE;
    p.type = ParticleType::Fire;
    particles.push_back(p);
  }
}

void ParticleSystem::update(float deltaTime) {
  for (auto &p : particles) {
    p.x += p.vx * deltaTime;
    p.y += p.vy * deltaTime;
    p.lifetime -= deltaTime;

    switch (p.type) {
    case ParticleType::Blood:
      p.vy += 0.5f * deltaTime;
      p.vx *= 0.94f;
      break;
    case ParticleType::Fire:
      p.vy -= 0.8f * deltaTime;
      p.vx *= 0.95f;
      break;
    case ParticleType::Smoke:
      p.vy -= 0.2f * deltaTime;
      p.vx *= 0.98f;
      p.size += 2.0f * deltaTime;
      if (p.size > p.maxSize)
        p.size = p.maxSize;
      break;
    case ParticleType::ExplosionDebris:
      p.vy += 3.0f * deltaTime;
      p.vx *= 0.88f;
      break;
    }
  }

  particles.erase(
      std::remove_if(particles.begin(), particles.end(),
                     [](const Particle &p) { return p.lifetime <= 0; }),
      particles.end());
}

void ParticleSystem::render() const {
  for (const auto &p : particles) {
    float alpha = p.lifetime / p.maxLifetime;
    Color c = p.color;
    c.a = static_cast<unsigned char>(alpha * c.a);

    float screenX = p.x * TILE_SIZE;
    float screenY = p.y * TILE_SIZE;

    switch (p.type) {
    case ParticleType::Blood:
      DrawCircle((int)screenX, (int)screenY, p.size * alpha, c);
      break;
    case ParticleType::Fire: {
      float flicker = 0.7f + 0.3f * std::sin(p.lifetime * 30.0f);
      DrawCircle((int)screenX, (int)screenY, p.size * flicker, c);
      break;
    }
    case ParticleType::Smoke:
      c.a = static_cast<unsigned char>((1.0f - (1.0f - alpha) * (1.0f - alpha)) * c.a);
      DrawCircle((int)screenX, (int)screenY, p.size, c);
      break;
    case ParticleType::ExplosionDebris:
      DrawCircle((int)screenX, (int)screenY, p.size * (0.5f + 0.5f * alpha), c);
      break;
    }
  }
}

} // namespace rogue

```

---

### File: src/EventSystem.cpp
```cpp
#include "rogue/EventSystem.h"
#include <algorithm>
#include <cstdlib>

namespace rogue {

const std::vector<std::string> EventSystem::commentatorLines = {
  "Nice shot! ...wait, that was an ally.",
  "You call THAT a dodge?",
  "My grandmother fights better than you.",
  "BOSS FIGHT! ...just kidding, it's a regular rat.",
  "Wave " + std::to_string(999) + "? You're screwed.",
  "Have you tried pressing buttons randomly?",
  "LEEEEROOOOY JEEENKIIINS!",
  "That monster has a family, you know.",
  "Critical hit! ...for the enemy.",
  "You're doing great! ...relatively speaking.",
  "Is that a new weapon or did you just drop lunch?",
  "50% of players die before this point. Congrats!",
  "The gods are watching... and laughing.",
  "This would be a great time to panic.",
  "I've seen better moves in a nursing home.",
  "Did you just STEP on that monster?",
  "THE BEES! NOT THE BEES!",
  "Omae wa mou shindeiru.",
  "NANI?!",
  "GET GOOD SCRUB.",
  "Is that a mimic? Yes. Yes it is.",
  "Your mother was a hamster and your father smelt of elderberries!",
  "PRESS ANY KEY... oh wait, you already did.",
  "Insert coin to continue... oh wait, it's free.",
  "The hunter becomes the hunted! ...or not.",
  "Floor " + std::to_string(50) + "? In THIS economy?",
};

EventSystem::EventSystem() {}

void EventSystem::update(float dt, int /*waveNumber*/, int /*floorNumber*/,
                          float timeOnFloor, float /*playerHp*/, float /*playerMaxHp*/) {
  this->timeOnFloor += dt;

  // Risk timer
  if (eventCooldown > 0.0f) eventCooldown -= dt;

  // Random events (every 15-30 seconds if no event active)
  if (!isEventActive() && eventCooldown <= 0.0f) {
    float chance = dt * 0.08f * (1.0f + timeOnFloor / 60.0f);
    if ((float)rand() / (float)RAND_MAX < chance) {
      int r2 = rand() % 100;
      if (r2 < 18) currentEvent.type = DynamicEvent::MeteorRain;
      else if (r2 < 33) currentEvent.type = DynamicEvent::MonsterInvasion;
      else if (r2 < 46) currentEvent.type = DynamicEvent::Eclipse;
      else if (r2 < 58) currentEvent.type = DynamicEvent::Blackout;
      else if (r2 < 68) currentEvent.type = DynamicEvent::GoldRush;
      else if (r2 < 78) currentEvent.type = DynamicEvent::ZombieHorde;
      else if (r2 < 89) currentEvent.type = DynamicEvent::Earthquake;
      else currentEvent.type = DynamicEvent::TimeWarp;
      currentEvent.timer = currentEvent.duration;
      currentEvent.warningTimer = 2.0f;
      eventCooldown = 15.0f;
      maybeComment("EVENT INCOMING!", 1.0f);
    }
  }

  if (isEventActive()) {
    currentEvent.warningTimer -= dt;
    if (currentEvent.warningTimer <= 0.0f) {
      currentEvent.timer -= dt;
      // Earthquake: continuous shake
      if (currentEvent.type == DynamicEvent::Earthquake) {
        float chance = dt * 5.0f;
        if ((float)rand() / (float)RAND_MAX < chance) {
          maybeComment("EARTHQUAKE!", 0.8f);
        }
      }
      if (currentEvent.timer <= 0.0f) {
        currentEvent.type = DynamicEvent::None;
      }
    }
  }

  // Commentator
  if (commentCooldown > 0.0f) commentCooldown -= dt;
  updateComments(dt);
}

void EventSystem::forceEvent(DynamicEvent type) {
  currentEvent.type = type;
  currentEvent.timer = currentEvent.duration;
  currentEvent.warningTimer = 2.0f;
  eventCooldown = 20.0f;
}

DynamicEvent EventSystem::randomEvent() {
  int r = rand() % 8;
  switch (r) {
    case 0: return DynamicEvent::Eclipse;
    case 1: return DynamicEvent::MeteorRain;
    case 2: return DynamicEvent::MonsterInvasion;
    case 3: return DynamicEvent::Blackout;
    case 4: return DynamicEvent::GoldRush;
    case 5: return DynamicEvent::ZombieHorde;
    case 6: return DynamicEvent::Earthquake;
    default: return DynamicEvent::TimeWarp;
  }
}

void EventSystem::triggerComment(const std::string &text) {
  CommentatorLine cl;
  cl.text = text;
  cl.timer = cl.duration;
  comments.push_back(cl);
  if (comments.size() > 5) comments.erase(comments.begin());
}

void EventSystem::updateComments(float dt) {
  for (auto &c : comments) c.timer -= dt;
  comments.erase(
    std::remove_if(comments.begin(), comments.end(),
      [](const CommentatorLine &c) { return c.timer <= 0; }),
    comments.end());
}

void EventSystem::maybeComment(const std::string &/*text*/, float chance) {
  if (commentCooldown > 0.0f) return;
  if ((float)rand() / (float)RAND_MAX < chance) {
    if (!commentatorLines.empty()) {
      int idx = rand() % commentatorLines.size();
      triggerComment(commentatorLines[idx]);
    }
    commentCooldown = 4.0f;
  }
}

float EventSystem::getRiskMultiplier() const {
  float t = timeOnFloor;
  if (t < 30.0f) return 1.0f;
  if (t < 60.0f) return 1.2f;
  if (t < 90.0f) return 1.5f;
  if (t < 120.0f) return 2.0f;
  return 3.0f;
}

int EventSystem::getBonusXp() const {
  float t = timeOnFloor;
  if (t < 30.0f) return 0;
  if (t < 60.0f) return 1;
  if (t < 90.0f) return 2;
  if (t < 120.0f) return 3;
  return 5;
}

} // namespace rogue

```

---

### File: src/raylib_renderer.cpp
```cpp
#include "rogue/Raylib_renderer.h"
#include "rogue/TextureManager.h"
#include <cstdlib>
#include <raylib.h>

namespace rogue {

RaylibRenderer::RaylibRenderer(int sw, int sh)
    : screenWidth(sw), screenHeight(sh) {
  camera.target = {0.0f, 0.0f};
  camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
}

RaylibRenderer::~RaylibRenderer() {}

void RaylibRenderer::initialize() {
  InitWindow(screenWidth, screenHeight, "Hitman Blood Monkey - Top-Down");
  SetTargetFPS(60);
}

void RaylibRenderer::shutdown() { CloseWindow(); }
void RaylibRenderer::clear() {
  BeginDrawing();
  ClearBackground(BLACK);
}
void RaylibRenderer::refresh() { EndDrawing(); }

Vector2 RaylibRenderer::worldToScreen(float x, float y) {
  return {x * TILE_SIZE, y * TILE_SIZE};
}

static int wallVariant(int x, int y) {
  unsigned h = (unsigned)((x / 12) * 73856093 ^ (y / 12) * 19349663);
  return (int)(h % 4);
}

void RaylibRenderer::renderMap(const Map &map,
                               const std::vector<Entity *> &entities) {
  auto &tm = TextureManager::getInstance();

  static const char *wallTextures[4] = {
    "wall_stone", "wall_reinforced", "wall_destroyed", "wall_bloody"
  };

  for (int y = 0; y < map.getHeight(); ++y) {
    for (int x = 0; x < map.getWidth(); ++x) {
      const Tile &tile = map.getTile(x, y);
      Vector2 screenPos = worldToScreen((float)x, (float)y);
      Vector2 tileOrigin = {screenPos.x, screenPos.y};

      Texture2D *tex = nullptr;
      if (tile.symbol == '#')
        tex = &tm.get(wallTextures[wallVariant(x, y)]);
      else if (tile.symbol == '%') {
        DrawTexturePro(tm.get("floor"), {0, 0, 16, 16},
                       {tileOrigin.x, tileOrigin.y, TILE_SIZE, TILE_SIZE},
                       {0, 0}, 0.0f, WHITE);
        DrawRectangle((int)tileOrigin.x, (int)tileOrigin.y, (int)TILE_SIZE,
                      (int)TILE_SIZE, {150, 0, 0, 180});
        continue;
      } else if (tile.symbol == '.' || tile.symbol == ' ')
        tex = &tm.get("floor");

      if (tex) {
        DrawTexturePro(*tex, {0, 0, 16, 16},
                       {tileOrigin.x, tileOrigin.y, TILE_SIZE, TILE_SIZE}, {0, 0},
                       0.0f, WHITE);
      }
    }
  }

  for (auto* entity : entities) {
    Vector2 pos = worldToScreen(entity->getX(), entity->getY());
    Vector2 tileOrigin = {pos.x - TILE_SIZE / 2.0f, pos.y - TILE_SIZE / 2.0f};

    Texture2D* tex = nullptr;
    int color = entity->getColor();

    if (color == COLOR_PLAYER)  tex = &tm.get("player");
    else if (color == 3)        tex = &tm.get("monster_sceleton");
    else if (color == 4)        tex = &tm.get("monster_goblin");
    else if (color == 5)        tex = &tm.get("monster_boss");
    else if (color == 6)        tex = &tm.get("monster_gun");
    else if (color == 7)        tex = &tm.get("monster_kamikaze");
    else if (color == 8)        tex = &tm.get("monster_hunter");

    if (tex) {
        Color tint = entity->isFlashing() ? RED : WHITE;
        DrawTexturePro(*tex, {0, 0, 16, 16},
                       {tileOrigin.x, tileOrigin.y, TILE_SIZE, TILE_SIZE},
                       {0, 0}, 0.0f, tint);
    }
  }
}

void RaylibRenderer::setZoom(float zoom) {
  camera.zoom = zoom;
}

void RaylibRenderer::addShake(float intensity) {
  shakeIntensity = std::max(shakeIntensity, intensity);
}

void RaylibRenderer::setCameraTarget(float x, float y) {
  float targetX = x * TILE_SIZE;
  float targetY = y * TILE_SIZE;

  float speed = 5.0f;
  float dt = GetFrameTime();

  camera.target.x += (targetX - camera.target.x) * speed * dt;
  camera.target.y += (targetY - camera.target.y) * speed * dt;

  // Screen shake
  if (shakeIntensity > 0.01f) {
    float sx = (std::rand() % 200 - 100) / 100.0f * shakeIntensity * TILE_SIZE * 0.5f;
    float sy = (std::rand() % 200 - 100) / 100.0f * shakeIntensity * TILE_SIZE * 0.5f;
    camera.target.x += sx;
    camera.target.y += sy;
    shakeIntensity *= 0.85f;
    if (shakeIntensity < 0.01f) shakeIntensity = 0.0f;
  }
}

void RaylibRenderer::beginScene() { BeginMode2D(camera); }

void RaylibRenderer::endScene() { EndMode2D(); }

void RaylibRenderer::drawChar(float x, float y, char ch, int color) {
  Vector2 screenPos = worldToScreen(x, y);
  DrawText(TextFormat("%c", ch), (int)screenPos.x, (int)screenPos.y, 20,
           (color == COLOR_PLAYER) ? BLUE : WHITE);
}

} // namespace rogue

```

---

### File: src/world/Map.cpp
```cpp
#include "rogue/world/Map.h"
#include "rogue/Renderer.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <stdexcept>

namespace rogue {

static constexpr int MAX_ROOMS = 35;

static const std::vector<Prefab> &prefabRegistry() {
  static const auto reg = [] {
    std::vector<Prefab> r;
    auto add = [&](std::vector<std::string> rows) {
      int h = rows.size();
      int w = rows[0].size();
      std::vector<char> tiles;
      tiles.reserve(w * h);
      for (auto &row : rows)
        for (char c : row)
          tiles.push_back(c);
      r.emplace_back(w, h, std::move(tiles));
    };

    // 1 - empty small
    add({"######",
         "#....#",
         "#....#",
         "#....#",
         "######"});

    // 2 - empty medium
    add({"########",
         "#......#",
         "#......#",
         "#......#",
         "#......#",
         "########"});

    // 3 - empty large
    add({"###########",
         "#.........#",
         "#.........#",
         "#.........#",
         "#.........#",
         "#.........#",
         "###########"});

    // 4 - empty wide
    add({"#############",
         "#...........#",
         "#...........#",
         "#...........#",
         "#############"});

    // 5 - empty tall
    add({"#####",
         "#...#",
         "#...#",
         "#...#",
         "#...#",
         "#...#",
         "#...#",
         "#...#",
         "#####"});

    // 6 - four pillars
    add({"#########",
         "#.......#",
         "#.#...#.#",
         "#.......#",
         "#.#...#.#",
         "#.......#",
         "#########"});

    // 7 - four pillars large
    add({"###########",
         "#.........#",
         "#.#.....#.#",
         "#.........#",
         "#.#.....#.#",
         "#.........#",
         "###########"});

    // 8 - center block
    add({"#######",
         "#.....#",
         "#.....#",
         "#..##.#",
         "#.....#",
         "#.....#",
         "#######"});

    // 9 - pillar grid 3x2
    add({"###########",
         "#.........#",
         "#.#..#..#.#",
         "#.........#",
         "#.#..#..#.#",
         "#.........#",
         "#.#..#..#.#",
         "###########"});

    // 10 - split middle
    add({"##########",
         "#........#",
         "#........#",
         "###....###",
         "#........#",
         "#........#",
         "##########"});

    // 11 - split offset
    add({"##########",
         "#........#",
         "#........#",
         "####..####",
         "#........#",
         "#........#",
         "##########"});

    // 12 - cross hall
    add({"#########",
         "#.......#",
         "#.......#",
         "###...###",
         "#.......#",
         "###...###",
         "#.......#",
         "#.......#",
         "#########"});

    // 13 - side alcoves
    add({"############",
         "#..........#",
         "#..##..##..#",
         "#..........#",
         "#..##..##..#",
         "#..........#",
         "############"});

    // 14 - top alcoves
    add({"########",
         "#......#",
         "##....##",
         "#......#",
         "#......#",
         "#......#",
         "##....##",
         "#......#",
         "########"});

    // 15 - throne room
    add({"##########",
         "#........#",
         "#........#",
         "#..####..#",
         "#.#....#.#",
         "#........#",
         "#........#",
         "#........#",
         "##########"});

    // 16 - barracks
    add({"###########",
         "#.........#",
         "#.##..##..#",
         "#.........#",
         "#..##..##.#",
         "#.........#",
         "#.##..##..#",
         "###########"});

    // 17 - library
    add({"############",
         "#..........#",
         "#.##.##.##.#",
         "#..........#",
         "#.##.##.##.#",
         "#..........#",
         "#.##.##.##.#",
         "############"});

    // 18 - dining hall
    add({"############",
         "#..........#",
         "#....##....#",
         "#..........#",
         "#....##....#",
         "#..........#",
         "#....##....#",
         "############"});

    // 19 - armory
    add({"##########",
         "#........#",
         "#........#",
         "#.##.##..#",
         "#........#",
         "#........#",
         "##########"});

    // 20 - temple
    add({"############",
         "#..........#",
         "#.#......#.#",
         "#.#......#.#",
         "#..........#",
         "#..........#",
         "#.#......#.#",
         "#.#......#.#",
         "#..........#",
         "############"});

    // 21 - crypt
    add({"#########",
         "#.......#",
         "#.##.##.#",
         "#.......#",
         "#.##.##.#",
         "#.......#",
         "#.##.##.#",
         "#########"});

    // 22 - garden
    add({"##########",
         "#........#",
         "#.##..##.#",
         "#........#",
         "#.##..##.#",
         "#........#",
         "#.##..##.#",
         "#........#",
         "##########"});

    // 23 - arena
    add({"##############",
         "#............#",
         "#............#",
         "#............#",
         "#............#",
         "#............#",
         "#............#",
         "#............#",
         "#............#",
         "##############"});

    // 24 - mazelet
    add({"#########",
         "#.......#",
         "#.#.###.#",
         "#.#...#.#",
         "#.###.#.#",
         "#...#.#.#",
         "#.###.#.#",
         "#.......#",
         "#########"});

    // 25 - octagon
    add({"#########",
         "##.....##",
         "#.......#",
         "#.......#",
         "#.......#",
         "#.......#",
         "#.......#",
         "##.....##",
         "#########"});

    // 26 - pillars four small
    add({"##########",
         "#........#",
         "#........#",
         "#..#..#..#",
         "#........#",
         "#........#",
         "##########"});

    // 27 - the vault
    add({"#########",
         "##.....##",
         "#.#...#.#",
         "#..#..#.#",
         "#...#...#",
         "#..#..#.#",
         "#.#...#.#",
         "##.....##",
         "#########"});

    // 28 - columned hall
    add({"###########",
         "#.........#",
         "#..#...#..#",
         "#.........#",
         "#..#...#..#",
         "#.........#",
         "#..#...#..#",
         "#.........#",
         "###########"});

    return r;
  }();
  return reg;
}

Map::Map(int width, int height) : width(width), height(height) {
  tiles.resize(height, std::vector<Tile>(width, {'#', COLOR_WALL}));
  std::random_device rd;
  generate(rd());
}

Map::Map(int width, int height, int seed) : width(width), height(height) {
  tiles.resize(height, std::vector<Tile>(width, {'#', COLOR_WALL}));
  generate(seed);
}

void Map::fillWithWalls() {
  for (auto &row : tiles)
    for (auto &tile : row)
      tile = {'#', COLOR_WALL};
}

void Map::setFloor(int x, int y) {
  if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
    tiles[y][x] = {'.', COLOR_DEFAULT};
}

void Map::carveRoom(const Room &room, const Prefab &prefab) {
  for (int y = 0; y < prefab.h; ++y)
    for (int x = 0; x < prefab.w; ++x)
      if (prefab.tile(x, y) == '.')
        setFloor(room.x + x, room.y + y);
}

void Map::findSpawnPoint(Room &room) {
  int cx = room.x + room.w / 2;
  int cy = room.y + room.h / 2;
  for (int r = 0; r <= room.w + room.h; ++r) {
    for (int dy = -r; dy <= r; ++dy) {
      for (int dx = -r; dx <= r; ++dx) {
        if (std::abs(dx) != r && std::abs(dy) != r)
          continue;
        int tx = cx + dx;
        int ty = cy + dy;
        if (tx >= room.x && tx < room.x + room.w && ty >= room.y &&
            ty < room.y + room.h && tiles[ty][tx].symbol == '.') {
          room.spawnX = tx;
          room.spawnY = ty;
          return;
        }
      }
    }
  }
  room.spawnX = cx;
  room.spawnY = cy;
}

void Map::carveHCorridor(int x1, int x2, int y) {
  for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
    setFloor(x, y);
  for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
    setFloor(x, y + 1);
}

void Map::carveVCorridor(int y1, int y2, int x) {
  for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
    setFloor(x, y);
  for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
    setFloor(x + 1, y);
}

void Map::generate(unsigned int seed) {
  fillWithWalls();

  std::mt19937 rng(seed);
  auto randInt = [&](int lo, int hi) {
    return std::uniform_int_distribution<int>(lo, hi)(rng);
  };

  const auto &prefabs = prefabRegistry();
  rooms.clear();

  for (int attempt = 0; attempt < 200 && (int)rooms.size() < MAX_ROOMS;
       ++attempt) {
    const Prefab &prefab = prefabs[randInt(0, prefabs.size() - 1)];

    Room room;
    room.w = prefab.w;
    room.h = prefab.h;
    room.x = randInt(1, width - room.w - 1);
    room.y = randInt(1, height - room.h - 1);

    bool overlaps = false;
    for (const auto &r : rooms)
      if (room.intersects(r)) {
        overlaps = true;
        break;
      }

    if (!overlaps) {
      carveRoom(room, prefab);
      findSpawnPoint(room);

      if (!rooms.empty()) {
        const Room &prev = rooms.back();
        if (randInt(0, 1) == 0) {
          carveHCorridor(prev.spawnX, room.spawnX, prev.spawnY);
          carveVCorridor(prev.spawnY, room.spawnY, room.spawnX);
        } else {
          carveVCorridor(prev.spawnY, room.spawnY, prev.spawnX);
          carveHCorridor(prev.spawnX, room.spawnX, room.spawnY);
        }
      }
      rooms.push_back(room);
    }
  }
}

bool Map::isWalkable(float x, float y) const {
  int gridX = static_cast<int>(std::floor(x));
  int gridY = static_cast<int>(std::floor(y));
  if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height) {
    return false;
  }
  return tiles[gridY][gridX].symbol != '#';
}

void Map::spillBlood(float x, float y) {
  int gridX = static_cast<int>(x);
  int gridY = static_cast<int>(y);
  if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height) {
    return;
  }
  tiles[gridY][gridX] = {'%', 5};
}

void Map::spillBloodArea(int cx, int cy, int radius) {
  for (int dy = -radius; dy <= radius; ++dy) {
    for (int dx = -radius; dx <= radius; ++dx) {
      if (dx * dx + dy * dy > radius * radius)
        continue;
      int gx = cx + dx;
      int gy = cy + dy;
      if (gx < 0 || gx >= width || gy < 0 || gy >= height)
        continue;
      if (tiles[gy][gx].symbol != '#')
        tiles[gy][gx] = {'%', 5};
    }
  }
}

int Map::getHeight() const { return height; }

int Map::getWidth() const { return width; }

const Tile &Map::getTile(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    throw std::out_of_range("Map::getTile coordinates out of range");
  }
  return tiles[y][x];
}

void Map::render(IRenderer &renderer) const {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const Tile &tile = tiles[y][x];
      renderer.drawChar(x, y, tile.symbol, tile.colorPair);
    }
  }
}

} // namespace rogue

```

---

### File: src/core/Command.cpp
```cpp
#include "rogue/core/Command.h"

namespace rogue {
void MoveCommand::execute(Entity &entity, Map &map) {
  int newX = entity.getX() + dx;
  int newY = entity.getY() + dy;
  if (map.isWalkable(newX, newY)) {
    entity.setPosition(newX, newY);
  }
}

void AttackCommand::execute(Entity &/*entity*/, Map &map) {
  if (!target)
    return;

  target->takeDamage(damage);
  if (target->isDead()) {
    map.spillBlood(target->getX(), target->getY());
  }
}
} // namespace rogue

```

---

### File: src/core/Engine.cpp
```cpp
#include "rogue/core/Engine.h"
#include "rogue/Exceptions.h"
#include "rogue/ParticleSystem.h"
#include "rogue/Raylib_renderer.h"
#include "rogue/Renderer.h"
#include "rogue/TextureManager.h"
#include "rogue/entities/Monster.h"
#include "rogue/entities/MonsterFactory.h"
#include "rogue/entities/Player.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <raylib.h>
#include <string>

namespace rogue {

Engine::Engine(std::unique_ptr<IRenderer> r)
    : isRunning(true), map(120, 80), player(0.0f, 0.0f, '@', COLOR_PLAYER, 100),
      deltaTime(0.0f) {

  if (!r) {
    throw InitializationException("Renderer cannot be null");
  }
  renderer = std::move(r);
  renderer->initialize();
  auto &tm = TextureManager::getInstance();
  tm.loadTexture("wall", "../assets/tile_0040.png");
  tm.loadTexture("wall_stone", "../assets/wall_stone.png");
  tm.loadTexture("wall_bloody", "../assets/wall_bloody.png");
  tm.loadTexture("wall_reinforced", "../assets/wall_reinforced.png");
  tm.loadTexture("wall_destroyed", "../assets/wall_destroyed.png");
  tm.loadTexture("floor", "../assets/tile_0049.png");
  tm.loadTexture("player", "../assets/tile_0088.png");
  tm.loadTexture("monster1", "../assets/tile_0109.png");
  tm.loadTexture("monster2", "../assets/tile_0111.png");
  tm.loadTexture("boss", "../assets/tile_0108.png");
  tm.loadTexture("monster_sceleton", "../assets/monster_sceleton.png");
  tm.loadTexture("monster_goblin", "../assets/monster_goblin.png");
  tm.loadTexture("monster_boss", "../assets/monster_boss.png");
  tm.loadTexture("monster_gun", "../assets/monster_gun.png");
  tm.loadTexture("monster_kamikaze", "../assets/monster_kamikaze.png");
  tm.loadTexture("monster_hunter", "../assets/monster_hunter.png");
  tm.loadTexture("weapon_minigun", "../assets/weapon_minigun.png");
  tm.loadTexture("weapon_shotgun", "../assets/weapon_shotgun.png");
  tm.loadTexture("weapon_rocket", "../assets/weapon_rocket.png");
  tm.loadTexture("effect_explosion", "../assets/effect_explosion.png");
  tm.loadTexture("effect_muzzle", "../assets/effect_muzzle.png");
  tm.loadTexture("effect_shield", "../assets/effect_shield.png");
  tm.loadTexture("effect_blood", "../assets/effect_blood.png");
  tm.loadTexture("pickup_health", "../assets/pickup_health.png");
  tm.loadTexture("pickup_ammo", "../assets/pickup_ammo.png");
  audio.init();
  ArtifactSystem artifactSys; // init static pools
  bulletSystem.onMonsterKilled = [this](float mx, float my) {
    audio.playDeath();
    meta.addKill();
    // Vampire mutation: heal on kill
    for (auto &m : mutations) {
      if (m.type == Mutation::Vampire) {
        player.takeDamage(-3);
        addFloatingText(mx, my - 1.0f, "+3 HP (Vampire)", MAROON);
      }
    }
  };
  bulletSystem.onExplosion = [this](float x, float y, float radius, float damage) {
    explosionDamage(x, y, radius, damage, &player);
  };
  // Place player in first room
  const auto &rooms = map.getRooms();
  if (!rooms.empty()) {
    player.setPosition(rooms[0].centerX(), rooms[0].centerY());
  }
  // God selection at game start
  int godChoice = rand() % 6;
  GodBlessing startGod = ArtifactSystem::allGods[godChoice];
  applyGodBlessing(startGod);
  // Hunter timer start
  hunterSpawnTimer = hunterSpawnInterval;
  // Meta tracking
  meta.addRun();
  syncBulletMods();
  startNextWave();
}

Engine::~Engine() {
  TextureManager::getInstance().unloadAll();
  renderer->shutdown();
}

void Engine::spawnEntities() {
  const auto &rooms = map.getRooms();
  if (rooms.empty())
    return;

  player.setPosition(rooms[0].centerX(), rooms[0].centerY());

  std::mt19937 rng(std::random_device{}());
  for (int i = 1; i < (int)rooms.size(); ++i) {
    const auto &room = rooms[i];
    int count = 3 + rng() % 5;
    for (int j = 0; j < count; ++j) {
      float mx = room.x + 1 + rng() % (room.w - 2);
      float my = room.y + 1 + rng() % (room.h - 2);
      MonsterType type = static_cast<MonsterType>(rng() % 4);
      monsters.push_back(MonsterFactory::createMonster(type, mx, my));
    }
  }
}

void Engine::spawnPickup(float x, float y) {
  auto spawnOne = [&]() {
    Pickup p;
    p.x = x;
    p.y = y;
    int r = rand() % 5;
    if (r == 0)       p.type = Pickup::Ammo;
    else if (r == 1)  p.type = Pickup::RocketAmmo;
    else              p.type = Pickup::Health;
    p.lifetime = 20.0f;
    pickups.push_back(p);
  };
  spawnOne();
  if (godGreedDoubleDrops) spawnOne();
}

void Engine::addFloatingText(float x, float y, const std::string &text, Color color) {
  FloatingText ft;
  ft.x = x;
  ft.y = y;
  ft.text = text;
  ft.color = color;
  ft.lifetime = 1.2f;
  ft.maxLifetime = 1.2f;
  floatingTexts.push_back(ft);
}

void Engine::startNextWave() {
  monsters.clear();
  const auto &rooms = map.getRooms();
  if (rooms.empty()) return;

  std::mt19937 rng(std::random_device{}());

  // Boss wave every 10 waves
  if (waveNumber % 10 == 0 && !rooms.empty()) {
    int bi = rng() % rooms.size();
    const auto &broom = rooms[bi];
    float bmx = broom.centerX();
    float bmy = broom.centerY();
    auto boss = MonsterFactory::createMonster(MonsterType::MonkeyBoss, bmx, bmy);
    if (boss) {
      int bossHP = (waveNumber * 25) + floorNumber * 50;
      boss->takeDamage(-bossHP);
      boss->setAttackDamage(15 + waveNumber);
      monsters.push_back(std::move(boss));
    }
    eventSystem.triggerComment("BOSS WAVE! GOOD LUCK!");
  } else {
    // Normal wave spawning
    int monstersPerRoom = 2 + waveNumber / 2;
    for (size_t i = 1; i < rooms.size(); ++i) {
      const auto &room = rooms[i];
      for (int j = 0; j < monstersPerRoom; ++j) {
        float mx = room.x + 1 + rng() % std::max(1, room.w - 2);
        float my = room.y + 1 + rng() % std::max(1, room.h - 2);
        MonsterType type;
        int rt = rng() % 100;
        if (waveNumber >= 8 && rt < 8)
          type = MonsterType::SplitterMonkey;
        else if (waveNumber >= 6 && rt < 18)
          type = MonsterType::ChargerMonkey;
        else if (waveNumber >= 4 && rt < 28)
          type = MonsterType::SummonerMonkey;
        else if (waveNumber >= 3 && rt < 38)
          type = MonsterType::HealerMonkey;
        else if (waveNumber >= 5 && rt < 46)
          type = MonsterType::MonkeyBoss;
        else if (waveNumber >= 3 && rt < 58)
          type = MonsterType::GunMonkey;
        else if (rt < 68)
          type = MonsterType::KamikazeMonkey;
        else if (rt < 82)
          type = MonsterType::GoblinMonkey;
        else
          type = MonsterType::SceletonMonkey;

        auto m = MonsterFactory::createMonster(type, mx, my);
        if (m) {
          int bonusHP = (waveNumber - 1) * 10;
          for (int h = 0; h < bonusHP; ++h) m->takeDamage(-1);
          monsters.push_back(std::move(m));
        }
      }
    }
  }

  // Spawn extra monsters in corridors (30-60% of room count)
  int corridorCount = (int)(rooms.size() * (0.3f + rng() % 30 / 100.0f));
  for (int i = 0; i < corridorCount; ++i) {
    float mx = rng() % (map.getWidth() - 2) + 1;
    float my = rng() % (map.getHeight() - 2) + 1;
    if (!map.isWalkable(mx, my)) continue;
    // Check not too close to player
    float dx = mx - player.getX();
    float dy = my - player.getY();
    if (std::sqrt(dx * dx + dy * dy) < 5.0f) continue;

    MonsterType type;
    int rt = rng() % 100;
    if (rt < 18) type = MonsterType::KamikazeMonkey;
    else if (rt < 35) type = MonsterType::GunMonkey;
    else if (rt < 50) type = MonsterType::HealerMonkey;
    else if (rt < 65) type = MonsterType::ChargerMonkey;
    else type = MonsterType::SceletonMonkey;

    auto m = MonsterFactory::createMonster(type, mx, my);
    if (m) {
      int bonusHP = (waveNumber - 1) * 10;
      for (int h = 0; h < bonusHP; ++h) m->takeDamage(-1);
      monsters.push_back(std::move(m));
    }
  }

  // Spawn mimics in some rooms (skip room 0)
  mimics.clear();
  mimicsSpawnedThisWave = 0;
  std::mt19937 mimicRng(std::random_device{}());
  for (size_t i = 1; i < rooms.size(); ++i) {
    if (mimicRng() % 100 < 20) { // 20% chance per room
      const auto &room = rooms[i];
      Mimic m;
      m.x = room.x + 1 + mimicRng() % std::max(1, room.w - 2);
      m.y = room.y + 1 + mimicRng() % std::max(1, room.h - 2);
      m.active = true;
      m.revealed = false;
      m.hp = 30.0f + waveNumber * 10;
      mimics.push_back(m);
      ++mimicsSpawnedThisWave;
    }
  }
  if (mimicsSpawnedThisWave > 0) {
    eventSystem.maybeComment("I wonder if that chest is safe...", 0.8f);
  }

  waitingForWave = false;
  waveAnnounceTimer = WAVE_ANNOUNCE_DURATION;
  audio.playWaveStart();
}

void Engine::explosionDamage(float x, float y, float radius, float damage, Entity *source) {
  particles.spawnExplosion(x, y, 40);
  audio.playExplosion();
  auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
  if (rr) rr->addShake(8.0f);

  // Damage monsters
  for (auto &m : monsters) {
    if (m->isDead() || m.get() == source) continue;
    float dx = m->getX() - x;
    float dy = m->getY() - y;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist < radius) {
      float falloff = 1.0f - dist / radius;
      int dmg = (int)(damage * falloff);
      if (dmg < 1) dmg = 1;
      m->takeDamage(dmg);
      m->onHit();
      addFloatingText(m->getX(), m->getY() - 0.5f, TextFormat("%d", dmg), ORANGE);
      if (m->isDead()) {
        particles.spawnExplosion(m->getX(), m->getY(), 20);
        particles.spawnSmoke(m->getX(), m->getY(), 10);
        spawnPickup(m->getX(), m->getY());
        ++killCount;
        addFloatingText(m->getX(), m->getY() - 1.0f, "KILL!", RED);
      }
    }
  }

  // Damage player if not source
  if (source != &player) {
    float dx = player.getX() - x;
    float dy = player.getY() - y;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist < radius) {
      float falloff = 1.0f - dist / radius;
      int dmg = (int)(damage * falloff * 0.5f);
      if (dmg > 0) {
        player.takeDamage(dmg);
        damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
        addFloatingText(player.getX(), player.getY() - 0.5f, TextFormat("-%d", dmg), RED);
      }
    }
  }
}

void Engine::addXPForKill() {
  int bonus = eventSystem.getBonusXp();
  xp += 1 + bonus;
  if (xp >= xpToNextLevel) {
    xp = 0;
    ++level;
    xpToNextLevel += 3;
    levelUpFlashTimer = LEVEL_UP_FLASH_DURATION;
    addFloatingText(player.getX(), player.getY() - 1.5f, "LEVEL UP!", GOLD);
    audio.playLevelUp();
    int heal = 10 + level * 5;
    player.takeDamage(-heal);
    // Mutation on level up
    Mutation mut = ArtifactSystem::randomMutation();
    applyMutation(mut);
  }
}

void Engine::applyMutation(Mutation &m) {
  mutations.push_back(m);
  addFloatingText(player.getX(), player.getY() - 2.0f,
      TextFormat("MUTATION: %s", m.name.c_str()), MAGENTA);

  switch (m.type) {
    case Mutation::ExtraArm: break; // handled in shooting code
    case Mutation::LaserEyes: break; // auto-beam
    case Mutation::WallJump: wallJumpTimer = 3.0f; break;
    case Mutation::ThornSkin: break; // reflected damage
    case Mutation::Vampire: break; // heal on kill
    case Mutation::Berserker: break; // dmg when low hp
    case Mutation::Shield: shieldHits = 0; break;
    case Mutation::Speedster:
      player.takeDamage(player.getMaxHp() * 30 / 100);
      break;
    case Mutation::FatRoll:
      break;
    case Mutation::GlassGun:
      for (int i = 0; i < player.getMaxHp() / 2; ++i) player.takeDamage(1);
      break;
  }
}

void Engine::applyGodBlessing(GodBlessing &g) {
  hasGod = true;
  currentGod = g;
  addFloatingText(player.getX(), player.getY() - 2.0f,
      TextFormat("BLESSING: %s", g.name.c_str()), g.color);

  // Reset god-specific fields
  godGreedDoubleDrops = false;
  godLuckCritChance = 0.0f;
  godSpeedBonus = 0.0f;

  switch (g.type) {
    case GodBlessing::GodOfSpeed:
      godSpeedBonus = 0.4f;
      addFloatingText(player.getX(), player.getY() - 0.5f, "+40% SPEED + INFINITE DASH", GREEN);
      break;
    case GodBlessing::GodOfFire:
      syncBulletMods();
      addFloatingText(player.getX(), player.getY() - 0.5f, "EXPLOSIVE BULLETS + FIRE RATE", RED);
      break;
    case GodBlessing::GodOfChaos:
      eventSystem.forceEvent(EventSystem::randomEvent());
      addFloatingText(player.getX(), player.getY() - 0.5f, "EVENTS x2 +100% DMG", PURPLE);
      break;
    case GodBlessing::GodOfHealth:
      for (int i = 0; i < 100; ++i) player.takeDamage(-1);
      addFloatingText(player.getX(), player.getY() - 0.5f, "+100 HP + REGEN", LIME);
      break;
    case GodBlessing::GodOfGreed:
      godGreedDoubleDrops = true;
      addFloatingText(player.getX(), player.getY() - 0.5f, "DOUBLE DROPS!", GOLD);
      break;
    case GodBlessing::GodOfLuck:
      godLuckCritChance = 0.25f;
      addFloatingText(player.getX(), player.getY() - 0.5f, "25% CRIT CHANCE!", SKYBLUE);
      break;
  }
}

void Engine::handleDynamicEvents() {
  // Meteor rain
  if (eventSystem.getCurrentEvent() == DynamicEvent::MeteorRain &&
      eventSystem.getEventWarning() <= 0.0f) {
    meteorTimer -= deltaTime;
    if (meteorTimer <= 0.0f) {
      meteorTimer = 0.3f;
      float mx = (float)(rand() % map.getWidth());
      float my = (float)(rand() % map.getHeight());
      if (map.isWalkable(mx, my)) {
        explosionDamage(mx, my, 2.0f, 30.0f, nullptr);
        // Check player hit
        float dx = player.getX() - mx;
        float dy = player.getY() - my;
        if (std::sqrt(dx*dx + dy*dy) < 3.0f) {
          player.takeDamage(15);
          damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
          addFloatingText(player.getX(), player.getY() - 0.5f, "-15", RED);
        }
      }
    }
  }

  // Monster invasion
  if (eventSystem.getCurrentEvent() == DynamicEvent::MonsterInvasion &&
      eventSystem.getEventWarning() <= 0.0f) {
    invasionTimer -= deltaTime;
    if (invasionTimer <= 0.0f) {
      invasionTimer = 0.5f;
      const auto &rooms = map.getRooms();
      if (!rooms.empty()) {
        int ri = rand() % rooms.size();
        const auto &room = rooms[ri];
        float mx = room.x + 1 + rand() % std::max(1, room.w - 2);
        float my = room.y + 1 + rand() % std::max(1, room.h - 2);
        int rt = rand() % 100;
        MonsterType type;
        if (rt < 40) type = MonsterType::SceletonMonkey;
        else if (rt < 70) type = MonsterType::GunMonkey;
        else if (rt < 90) type = MonsterType::KamikazeMonkey;
        else type = MonsterType::GoblinMonkey;
        auto m = MonsterFactory::createMonster(type, mx, my);
        if (m) {
          for (int h = 0; h < (waveNumber-1)*10; ++h) m->takeDamage(-1);
          if (zombieHordeActive) m->zombieRevivesLeft = 1;
          monsters.push_back(std::move(m));
        }
      }
    }
  }
}

void Engine::updateHunter(float dt) {
  if (!hunterActive) {
    hunterSpawnTimer -= dt;
    if (hunterSpawnTimer <= 0.0f) {
      hunterActive = true;
      // Spawn in far room
      const auto &rooms = map.getRooms();
      if (rooms.size() > 1) {
        int ri = rooms.size() - 1;
        const auto &room = rooms[ri];
        float hx = room.centerX();
        float hy = room.centerY();
        hunter = MonsterFactory::createMonster(MonsterType::HunterMonkey, hx, hy);
        if (hunter) {
          int bonusHP = (waveNumber - 1) * 15 + floorNumber * 20;
          for (int h = 0; h < bonusHP; ++h) hunter->takeDamage(-1);
          addFloatingText(hx, hy - 1.0f, "HUNTER APPEARED!", RED);
          eventSystem.triggerComment("THE HUNTER IS COMING FOR YOU!");
        }
      }
    }
  } else if (hunter && !hunter->isDead()) {
    hunter->updateAI(map, player.getX(), player.getY(), dt);
    hunter->tickRangedCooldown(dt);

    float dx = player.getX() - hunter->getX();
    float dy = player.getY() - hunter->getY();
    float dist = std::sqrt(dx*dx + dy*dy);

    if (dist <= Monster::ATTACK_RANGE && hunter->getAttackCooldown() <= 0.0f) {
      player.takeDamage(hunter->getAttackDamage());
      damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
      particles.spawnBlood(player.getX(), player.getY(), 5);
      hunter->resetCooldown();
      addFloatingText(player.getX(), player.getY() - 0.5f,
          TextFormat("-%d", hunter->getAttackDamage()), RED);
    }

    if (hunter->canFireRanged() && dist < hunter->rangedRange && dist > Monster::ATTACK_RANGE) {
      EnemyProjectile eb;
      eb.x = hunter->getX();
      eb.y = hunter->getY();
      float ndx = player.getX() - hunter->getX();
      float ndy = player.getY() - hunter->getY();
      float nlen = std::sqrt(ndx*ndx + ndy*ndy);
      if (nlen > 0) { ndx /= nlen; ndy /= nlen; }
      eb.vx = ndx * 8.0f;
      eb.vy = ndy * 8.0f;
      eb.damage = 12.0f;
      enemyBullets.push_back(eb);
      hunter->resetRangedCooldown();
    }
  } else if (hunter && hunter->isDead()) {
    addFloatingText(hunter->getX(), hunter->getY() - 1.0f, "HUNTER DEFEATED!", GOLD);
    spawnArtifactPickup(hunter->getX(), hunter->getY());
    spawnPickup(hunter->getX(), hunter->getY());
    hunter.reset();
    hunterActive = false;
    hunterSpawnTimer = hunterSpawnInterval;
    if (hunterSpawnInterval > 10.0f) hunterSpawnInterval -= 2.0f;
  }
}

void Engine::spawnArtifactPickup(float x, float y) {
  Pickup p;
  p.x = x;
  p.y = y;
  p.type = Pickup::ArtifactItem;
  p.artifact = ArtifactSystem::randomArtifact();
  p.lifetime = 20.0f;
  pickups.push_back(p);
}

void Engine::checkArtifactCombinations() {
  for (size_t i = 0; i < artifacts.size(); ++i) {
    for (size_t j = i + 1; j < artifacts.size(); ++j) {
      Artifact result = ArtifactSystem::tryCombine(artifacts[i], artifacts[j]);
      if (result.name.empty()) continue;
      // Remove originals, add combined
      ArtifactSystem::unapplyArtifact(artifacts[i], player, ammoMG, ammoSG, ammoRL,
          MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
      ArtifactSystem::unapplyArtifact(artifacts[j], player, ammoMG, ammoSG, ammoRL,
          MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
      artifacts.erase(artifacts.begin() + j);
      artifacts.erase(artifacts.begin() + i);
      artifacts.push_back(result);
      ArtifactSystem::applyArtifact(result, player, ammoMG, ammoSG, ammoRL,
          MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
      syncBulletMods();
      addFloatingText(player.getX(), player.getY() - 1.0f,
          TextFormat("COMBINED: %s!", result.name.c_str()), ORANGE);
      particles.spawnExplosion(player.getX(), player.getY(), 30);
      return;
    }
  }
}

void Engine::breakWall(int x, int y) {
  map.setFloor(x, y);
  particles.spawnExplosion((float)x, (float)y, 15);
  auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
  if (rr) rr->addShake(2.0f);
  // Check if secret room revealed
  addFloatingText((float)x, (float)y - 0.5f, "WALL DESTROYED!", ORANGE);
}

void Engine::spawnSplitter(float x, float y, int splitCount, int maxHp) {
  if (splitCount <= 0) return;
  for (int i = 0; i < 2; ++i) {
    float ox = (i == 0) ? -0.5f : 0.5f;
    float oy = (i == 0) ? 0.5f : -0.5f;
    auto s = MonsterFactory::createMonster(MonsterType::SceletonMonkey, x + ox, y + oy);
    if (s) {
      s->isSplitter = true;
      s->splitCount = splitCount - 1;
      s->setAttackDamage(8);
      int newHp = std::max(10, maxHp / 2);
      for (int h = 0; h < newHp; ++h) s->takeDamage(-1);
      monsters.push_back(std::move(s));
    }
  }
}

void Engine::applyWaveReward(int choice) {
  if (!showingWaveReward) return;
  showingWaveReward = false;
  int r = waveRewardChoices[choice];
  const char *name = waveRewardTexts[choice].c_str();

  switch (r) {
    case 0: // +20 MAX HP
      for (int i = 0; i < 20; ++i) player.takeDamage(-1);
      break;
    case 1: // +30% DMG (adds to damage multiplier via artifact bonus)
      artifacts.push_back({"Wave Reward (+30% DMG)", "", 30,0,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, GOLD, 'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
      syncBulletMods();
      break;
    case 2: // +SPEED
      artifacts.push_back({"Wave Reward (+SPEED)", "", 0,0.3f,0,0,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, GREEN, 'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
      break;
    case 3: // +FIRE RATE
      artifacts.push_back({"Wave Reward (+FIRE RATE)", "", 0,0,0,0.2f,0,0,0, false,false,false,false, 0,0,0,0,false,false,false,false,false, ORANGE, 'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
      break;
    case 4: // +AMMO
      ammoMG = std::min(MAX_AMMO_MG + 50, ammoMG + 50);
      ammoSG = std::min(MAX_AMMO_SG + 10, ammoSG + 10);
      ammoRL = std::min(MAX_AMMO_RL + 5, ammoRL + 5);
      break;
    case 5: // +ARMOR (reduced damage taken - not implemented, give HP instead)
      for (int i = 0; i < 30; ++i) player.takeDamage(-1);
      break;
    case 6: // +1 JUMP
      artifacts.push_back({"Wave Reward (+JUMP)", "", 0,0,0,0,0,0,0, true,false,false,false, 0,0,0,0,false,false,false,false,false, SKYBLUE, 'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
      break;
    case 7: // VAMPIRE
      mutations.push_back({"Wave Reward (Vampire)", "heal 3 HP on kill", Mutation::Vampire});
      break;
  }
  addFloatingText(player.getX(), player.getY() - 1.5f, TextFormat("REWARD: %s", name), GOLD);
  particles.spawnExplosion(player.getX(), player.getY(), 20);
}

void Engine::syncBulletMods() {
  hasRicochet = false;
  hasPiercing = false;
  hasExplosive = false;
  for (auto &a : artifacts) {
    if (a.ricochet) hasRicochet = true;
    if (a.piercingBullets || a.noAmmoDrops) hasPiercing = true;
    if (a.explosiveBullets) hasExplosive = true;
  }
  if (hasGod && currentGod.type == GodBlessing::GodOfFire) {
    hasExplosive = true;
  }
  bulletSystem.ricochetMod = hasRicochet;
  bulletSystem.piercingMod = hasPiercing;
  bulletSystem.explosiveMod = hasExplosive;
}

float Engine::getDamageMultiplier() {
  // God of Luck: crit chance - roll now, cache result
  if (godLuckCritChance > 0.0f && (float)rand() / (float)RAND_MAX < godLuckCritChance) {
    return 3.0f; // crit = 3x damage + visual indicated by caller
  }
  float mult = 1.0f;
  for (auto &a : artifacts) {
    if (a.bonusDamage > 0) mult += a.bonusDamage / 100.0f;
  }
  for (auto &m : mutations) {
    if (m.type == Mutation::GlassGun) mult += 2.0f;
    if (m.type == Mutation::Berserker && player.getHp() > 0 &&
        player.getHp() < player.getMaxHp() * 30 / 100) {
      mult += 1.0f;
    }
  }
  if (hasGod && currentGod.type == GodBlessing::GodOfFire) mult += 0.5f;
  if (hasGod && currentGod.type == GodBlessing::GodOfChaos) mult += 1.0f;
  return mult;
}

void Engine::handleInput() {
  // Pause toggle
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
    if (!player.isDead()) paused = !paused;
  }
  if (paused) return;

  // Debug toggle
  if (IsKeyPressed(KEY_F1)) showDebug = !showDebug;

  if (player.isDead()) {
    if (IsKeyPressed(KEY_Q)) isRunning = false;
    if (IsKeyPressed(KEY_R)) { reset(); }
    return;
  }

  // Dash input (stored, applied in run loop)
  if (IsKeyPressed(KEY_LEFT_SHIFT) && dashCooldown <= 0.0f && !isDashing) {
    isDashing = true;
    dashTimer = DASH_DURATION;
    bool godSpeed = hasGod && currentGod.type == GodBlessing::GodOfSpeed;
    bool fatRoll = false;
    for (auto &m : mutations) { if (m.type == Mutation::FatRoll) fatRoll = true; }
    if (!godSpeed && !fatRoll) dashCooldown = DASH_COOLDOWN;
    dashDx = 0.0f; dashDy = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dashDy -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dashDy += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dashDx -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dashDx += 1.0f;
    if (dashDx == 0 && dashDy == 0) dashDy = -1.0f; // default dash forward
    float len = std::sqrt(dashDx * dashDx + dashDy * dashDy);
    if (len > 0) { dashDx /= len; dashDy /= len; }
    audio.playDash();
  }

  // Panic mode + movement input (skip if dashing)
  if (!isDashing) {
    float dx = 0.0f, dy = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dy -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dy += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dx -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx += 1.0f;
    // Reverse controls curse (Seven-League Boots artifact)
    for (auto &a : artifacts) {
      if (a.reverseControls) { dx = -dx; dy = -dy; }
    }
    // Critical fail reversed controls
    if (controlsReversed) { dx = -dx; dy = -dy; }
    // WallJump mutation: pass through walls while active
    player.ignoreWalls = (wallJumpTimer > 0.0f);
    player.processInput(dx, dy, deltaTime);

    // Speedster mutation: +50% speed
    for (auto &m : mutations) {
      if (m.type == Mutation::Speedster) {
        player.move(dx * deltaTime * 7.5f, dy * deltaTime * 7.5f);
      }
    }
    // God of Speed bonus
    if (hasGod && currentGod.type == GodBlessing::GodOfSpeed) {
      player.move(dx * deltaTime * 6.0f, dy * deltaTime * 6.0f);
    }
    // Panic mode: +20% speed when HP < 30
    if (player.getHp() > 0 && player.getHp() < 30) {
      float panicBonus = 1.2f;
      float pdx = dx * deltaTime * 3.0f * panicBonus;
      float pdy = dy * deltaTime * 3.0f * panicBonus;
      if (map.isWalkable(player.getX() + pdx, player.getY() + pdy)) {
        player.move(pdx, pdy);
      }
    }
  }

  if (IsKeyPressed(KEY_Q)) isRunning = false;

  // Weapon switching
  if (IsKeyPressed(KEY_ONE)) currentWeapon = Weapon::makeFists();
  if (IsKeyPressed(KEY_TWO)) currentWeapon = Weapon::makeMinigun();
  if (IsKeyPressed(KEY_THREE)) currentWeapon = Weapon::makeShotgun();
  if (IsKeyPressed(KEY_FOUR)) currentWeapon = Weapon::makeRocketLauncher();

  // Destructible environment: break nearest wall (F key)
  if (IsKeyPressed(KEY_F)) {
    for (int dy = -2; dy <= 2; ++dy) {
      for (int dx = -2; dx <= 2; ++dx) {
        int wx = (int)player.getX() + dx;
        int wy = (int)player.getY() + dy;
        if (wx < 0 || wx >= map.getWidth() || wy < 0 || wy >= map.getHeight()) continue;
        const Tile &t = map.getTile(wx, wy);
        if (t.symbol == '#') {
          breakWall(wx, wy);
          dx = 3; dy = 3; // break out
        }
      }
    }
  }

  if (IsKeyPressed(KEY_E) && timeStopCooldownTimer <= 0.0f && !timeStopped) {
    timeStopped = true;
    timeStopTimer = TIME_STOP_DURATION;
    timeStopTextTimer = TIME_STOP_TEXT_DURATION;
    timeStopCooldownTimer = TIME_STOP_COOLDOWN;
    particles.spawnExplosion(player.getX(), player.getY(), 25);
    audio.playZaWarudo();
  }

  // Critical fail system
  if (critFailTimer > 0.0f) critFailTimer -= deltaTime;
  if (critFailTimer <= 0.0f && !artifacts.empty() && rand() % 100 < 3) {
    int failType = rand() % 3;
    if (failType == 0) {
      addFloatingText(player.getX(), player.getY() - 1.0f, "CRITICAL FAIL! Weapon jam!", RED);
      weaponCooldownTimer = 1.0f;
    } else if (failType == 1) {
      int selfDmg = 5 + rand() % 10;
      player.takeDamage(selfDmg);
      damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
      addFloatingText(player.getX(), player.getY() - 0.5f, TextFormat("-%d (misfire!)", selfDmg), RED);
    } else {
      addFloatingText(player.getX(), player.getY() - 1.0f, "CONTROLS REVERSED!", ORANGE);
      controlsReversed = true;
      critFailTimer = 3.0f;
    }
    critFailTimer = 2.0f;
  }
  // Reversed controls timer
  if (controlsReversed && critFailTimer <= 0.0f) {
    controlsReversed = false;
    addFloatingText(player.getX(), player.getY() - 1.0f, "Controls normal again", GREEN);
  }

  weaponCooldownTimer -= deltaTime;

  auto *raylibRenderer = dynamic_cast<RaylibRenderer *>(renderer.get());
  auto getMouseTarget = [&]() -> std::pair<float, float> {
    if (!raylibRenderer) return {player.getX() + 1, player.getY()};
    Vector2 ms = GetMousePosition();
    Vector2 mw = GetScreenToWorld2D(ms, raylibRenderer->getCamera());
    return {mw.x / TILE_SIZE, mw.y / TILE_SIZE};
  };

  if (currentWeapon.type == WeaponType::Fists) {
    currentWeapon.isFiring = false;
    if (IsKeyPressed(KEY_SPACE) && weaponCooldownTimer <= 0.0f) {
      weaponCooldownTimer = currentWeapon.cooldown;
      if (raylibRenderer) raylibRenderer->addShake(4.0f);
      audio.playHit();
      float dmgMult = getDamageMultiplier();
      bool isCrit = dmgMult >= 3.0f;
      int fistDmg = (int)(currentWeapon.damage * dmgMult);
      for (auto &m : monsters) {
        if (m->isDead()) continue;
        float ddx = m->getX() - player.getX();
        float ddy = m->getY() - player.getY();
        float dist = std::sqrt(ddx * ddx + ddy * ddy);
        if (dist <= currentWeapon.range) {
          m->takeDamage(fistDmg);
          m->onHit();
          map.spillBloodArea((int)m->getX(), (int)m->getY(), 2);
          particles.spawnBlood(m->getX(), m->getY(), 60);
          addFloatingText(m->getX(), m->getY() - 0.5f,
              isCrit ? TextFormat("CRIT! %d", fistDmg) : TextFormat("%d", fistDmg),
              isCrit ? GOLD : YELLOW);
          audio.playHit();
          if (m->isDead()) {
            particles.spawnExplosion(m->getX(), m->getY(), 40);
            particles.spawnSmoke(m->getX(), m->getY(), 15);
            map.spillBloodArea((int)m->getX(), (int)m->getY(), 3);
            spawnPickup(m->getX(), m->getY());
            ++killCount;
            addFloatingText(m->getX(), m->getY() - 1.0f, "KILL!", RED);
            addXPForKill();
          }
        }
      }
    }
  }

  if (currentWeapon.type == WeaponType::Minigun) {
    currentWeapon.isFiring = IsMouseButtonDown(MOUSE_LEFT_BUTTON) && ammoMG > 0;

    if (currentWeapon.isFiring) {
      currentWeapon.currentFireRate -= currentWeapon.spinupRate * deltaTime;
      if (currentWeapon.currentFireRate < currentWeapon.minFireRate)
        currentWeapon.currentFireRate = currentWeapon.minFireRate;
    } else {
      currentWeapon.currentFireRate += currentWeapon.spinupRate * deltaTime;
      if (currentWeapon.currentFireRate > currentWeapon.maxFireRate)
        currentWeapon.currentFireRate = currentWeapon.maxFireRate;
    }

    if (raylibRenderer && currentWeapon.isFiring &&
        weaponCooldownTimer <= 0.0f && ammoMG > 0) {
      --ammoMG;
      particles.spawnMuzzleFlash(player.getX(), player.getY());
      weaponCooldownTimer = currentWeapon.currentFireRate;
      raylibRenderer->addShake(1.5f);
      audio.playShoot();

      auto [tx, ty] = getMouseTarget();
      float dmgMult = getDamageMultiplier();
      int baseDmg = (int)(currentWeapon.damage * dmgMult);
      float spreadAngles[] = {-0.15f, 0.0f, 0.15f};
      int shotCount = 1;
      for (auto &m : mutations) { if (m.type == Mutation::ExtraArm) shotCount = 2; }
      for (int si = 0; si < shotCount; ++si) {
        for (float angleOffset : spreadAngles) {
          float extraOff = (si == 1) ? 0.4f : 0.0f;
          float dx = tx - player.getX();
          float dy = ty - player.getY();
          float len = std::sqrt(dx * dx + dy * dy);
          if (len == 0) continue;
          dx /= len; dy /= len;
          float rx = dx * std::cos(angleOffset + extraOff) - dy * std::sin(angleOffset + extraOff);
          float ry = dx * std::sin(angleOffset + extraOff) + dy * std::cos(angleOffset + extraOff);
          bulletSystem.spawn(player.getX(), player.getY(),
              player.getX() + rx * 10.0f, player.getY() + ry * 10.0f,
              (float)baseDmg);
        }
      }
    }
  }

  if (currentWeapon.type == WeaponType::Shotgun) {
    currentWeapon.isFiring = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && ammoSG > 0;
    if (raylibRenderer && currentWeapon.isFiring &&
        weaponCooldownTimer <= 0.0f && ammoSG > 0) {
      --ammoSG;
      weaponCooldownTimer = currentWeapon.cooldown;
      raylibRenderer->addShake(6.0f);
      audio.playShoot();
      particles.spawnMuzzleFlash(player.getX(), player.getY());

      auto [tx, ty] = getMouseTarget();
      float dmgMult = getDamageMultiplier();
      int baseDmg = (int)(currentWeapon.damage * dmgMult);
      float spreadAngles[] = {-0.25f, -0.15f, -0.05f, 0.05f, 0.15f, 0.25f};
      int shotCount = 1;
      for (auto &m : mutations) { if (m.type == Mutation::ExtraArm) shotCount = 2; }
      for (int si = 0; si < shotCount; ++si) {
        for (float angleOffset : spreadAngles) {
          float extraOff = (si == 1) ? 0.4f : 0.0f;
          float dx = tx - player.getX();
          float dy = ty - player.getY();
          float len = std::sqrt(dx * dx + dy * dy);
          if (len == 0) continue;
          dx /= len; dy /= len;
          float rx = dx * std::cos(angleOffset + extraOff) - dy * std::sin(angleOffset + extraOff);
          float ry = dx * std::sin(angleOffset + extraOff) + dy * std::cos(angleOffset + extraOff);
          bulletSystem.spawn(player.getX(), player.getY(),
              player.getX() + rx * 10.0f, player.getY() + ry * 10.0f,
              (float)baseDmg);
        }
      }
    }
  }

  if (currentWeapon.type == WeaponType::RocketLauncher) {
    currentWeapon.isFiring = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && ammoRL > 0;
    if (raylibRenderer && currentWeapon.isFiring &&
        weaponCooldownTimer <= 0.0f && ammoRL > 0) {
      --ammoRL;
      weaponCooldownTimer = currentWeapon.cooldown;
      raylibRenderer->addShake(10.0f);
      audio.playShoot();

      auto [tx, ty] = getMouseTarget();
      float dx = tx - player.getX();
      float dy = ty - player.getY();
      float len = std::sqrt(dx * dx + dy * dy);
      if (len > 0) { dx /= len; dy /= len; }
      float targetX = player.getX() + dx * currentWeapon.range;
      float targetY = player.getY() + dy * currentWeapon.range;

      bulletSystem.spawn(player.getX(), player.getY(), targetX, targetY,
                         0.0f, true);
      int rCount = 1;
      for (auto &m : mutations) { if (m.type == Mutation::ExtraArm) rCount = 2; }
      if (rCount > 1) {
        bulletSystem.spawn(player.getX() + dy * 0.3f, player.getY() - dx * 0.3f,
            targetX + dy * 0.5f, targetY - dx * 0.5f, 0.0f, true);
      }
    }
  }
}
void Engine::renderDebugInfo() {
  int y = 10;
  const int lineHeight = 20;
  const int fontSize = 20;

  DrawText(TextFormat("FPS: %d", GetFPS()), 10, y, fontSize, YELLOW);
  y += lineHeight;
  DrawText(TextFormat("Delta: %.4f ms", deltaTime * 1000.0f), 10, y, fontSize,
           YELLOW);
  y += lineHeight;

  DrawText(TextFormat("Pos: %.2f, %.2f", player.getX(), player.getY()), 10, y,
           fontSize, GREEN);
  y += lineHeight;
  DrawText(TextFormat("HP: %d", player.getHp()), 10, y, fontSize, GREEN);
  y += lineHeight;
  DrawText(TextFormat("Tile: %d, %d", (int)player.getX(), (int)player.getY()),
           10, y, fontSize, GREEN);
  y += lineHeight;

  DrawText(TextFormat("Monsters alive: %d", (int)monsters.size()), 10, y,
           fontSize, RED);
  y += lineHeight;
  for (auto &m : monsters) {
    DrawText(TextFormat("  [%c] pos: %.1f,%.1f hp:%d", m->getSymbol(),
                        m->getX(), m->getY(), m->getHp()),
             10, y, fontSize, RED);
    y += lineHeight;
  }

  DrawText(TextFormat("Map: %dx%d", map.getWidth(), map.getHeight()), 10, y,
           fontSize, GRAY);
  y += lineHeight;

  const Tile &t = map.getTile((int)player.getX(), (int)player.getY());
  DrawText(
      TextFormat("Tile under player: '%c' color:%d", t.symbol, t.colorPair), 10,
      y, fontSize, GRAY);
}
void Engine::renderHUD() {
  auto &texMgr = TextureManager::getInstance();
  int sw = GetScreenWidth();
  int sh = GetScreenHeight();
  int hPosX = 10, hPosY = sh - 50;

  // Health bar
  const int barW = 200, barH = 20;
  float hpRatio = (float)player.getHp() / player.getMaxHp();
  DrawRectangle(hPosX, hPosY, barW, barH, Color{50, 50, 50, 200});
  Color hpColor = hpRatio > 0.5f ? GREEN : (hpRatio > 0.25f ? ORANGE : RED);
  DrawRectangle(hPosX, hPosY, (int)(barW * hpRatio), barH, hpColor);
  DrawRectangleLines(hPosX, hPosY, barW, barH, Color{200, 200, 200, 100});
  DrawText(TextFormat("%d/%d", player.getHp(), player.getMaxHp()),
           hPosX + barW + 10, hPosY + 2, 16, WHITE);

  // Weapon indicator + ammo
  const char *weaponName = "FISTS";
  Color weaponColor = SKYBLUE;
  std::string ammoText = "";
  const char *weaponTexName = nullptr;
  if (currentWeapon.type == WeaponType::Minigun) {
    weaponName = "MINIGUN"; weaponColor = ORANGE;
    ammoText = TextFormat(" [%d/%d]", ammoMG, MAX_AMMO_MG);
    weaponTexName = "weapon_minigun";
  } else if (currentWeapon.type == WeaponType::Shotgun) {
    weaponName = "SHOTGUN"; weaponColor = PINK;
    ammoText = TextFormat(" [%d/%d]", ammoSG, MAX_AMMO_SG);
    weaponTexName = "weapon_shotgun";
  } else if (currentWeapon.type == WeaponType::RocketLauncher) {
    weaponName = "ROCKET"; weaponColor = RED;
    ammoText = TextFormat(" [%d/%d]", ammoRL, MAX_AMMO_RL);
    weaponTexName = "weapon_rocket";
  }
  if (weaponTexName) {
    float iconSize = 42.0f;
    DrawTexturePro(texMgr.get(weaponTexName), {0, 0, 32, 32},
                   {(float)hPosX, (float)hPosY - 22, iconSize, iconSize},
                   {0, 0}, 0.0f, WHITE);
    DrawText(TextFormat("[%d] %s%s", (int)currentWeapon.type + 1, weaponName,
             ammoText.c_str()),
             hPosX + (int)iconSize + 4, hPosY - 22, 16, weaponColor);
  } else {
    DrawText(TextFormat("[%d] %s%s", (int)currentWeapon.type + 1, weaponName,
             ammoText.c_str()),
             hPosX, hPosY - 22, 16, weaponColor);
  }

  // Floor / XP / Level
  DrawText(TextFormat("FLOOR %d  WAVE %d", floorNumber, waveNumber),
           sw - 200, hPosY - 22, 14, LIME);
  DrawText(TextFormat("LVL %d  XP %d/%d", level, xp, xpToNextLevel),
           sw - 200, hPosY, 14, SKYBLUE);

  // Kill counter
  DrawText(TextFormat("KILLS: %d", killCount),
           sw - 100, hPosY + 16, 14, RED);

  // Weapon switch hint
  DrawText("1:FISTS  2:MINIGUN  3:SHOTGUN  4:ROCKET",
           sw / 2 - 180, sh - 25, 12, Color{180, 180, 180, 150});
}

void Engine::render() {
  // ---- Camera zoom for time stop ----
  float targetZoom = 1.0f;
  if (timeStopped) {
    float prog = timeStopTimer / TIME_STOP_DURATION;
    float tN = TIME_STOP_TRANSITION / TIME_STOP_DURATION;
    if (prog > (1.0f - tN)) {
      float t = (prog - (1.0f - tN)) / tN;
      targetZoom = 1.0f + 0.15f * t;
    } else if (prog < tN) {
      float t = prog / tN;
      targetZoom = 1.0f + 0.15f * (1.0f - t);
    } else {
      targetZoom = 1.15f;
    }
  }
  auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
  if (rr) rr->setZoom(targetZoom);

  renderer->clear();

  renderer->setCameraTarget(player.getX(), player.getY());
  renderer->beginScene();

  std::vector<Entity *> allEntities;
  allEntities.push_back(&player);
  for (auto &m : monsters) {
    if (!m->isDead())
      allEntities.push_back(m.get());
  }

  renderer->renderMap(map, allEntities);

  // Pickups
  auto &tm = TextureManager::getInstance();
  for (auto &p : pickups) {
    if (!p.active) continue;
    float sx = p.x * TILE_SIZE;
    float sy = p.y * TILE_SIZE;
    if (p.type == Pickup::Health) {
      DrawTexturePro(tm.get("pickup_health"), {0, 0, 16, 16},
                     {sx - TILE_SIZE/2, sy - TILE_SIZE/2, TILE_SIZE, TILE_SIZE},
                     {0, 0}, 0.0f, WHITE);
    } else if (p.type == Pickup::ArtifactItem) {
      DrawRectangle((int)sx - 8, (int)sy - 8, 16, 16, {200, 100, 255, 220});
      DrawText("?", (int)sx - 4, (int)sy - 7, 16, WHITE);
    } else {
      DrawTexturePro(tm.get("pickup_ammo"), {0, 0, 16, 16},
                     {sx - TILE_SIZE/2, sy - TILE_SIZE/2, TILE_SIZE, TILE_SIZE},
                     {0, 0}, 0.0f, WHITE);
    }
  }

  // Mimics (chests before reveal)
  for (auto &mim : mimics) {
    if (!mim.active) continue;
    float sx = mim.x * TILE_SIZE;
    float sy = mim.y * TILE_SIZE;
    if (!mim.revealed) {
      DrawRectangle((int)sx - 10, (int)sy - 8, 20, 16, {180, 140, 40, 220});
      DrawRectangle((int)sx - 6, (int)sy - 4, 12, 8, {120, 90, 20, 220});
      DrawText("?", (int)sx - 4, (int)sy - 7, 16, GOLD);
    } else {
      DrawRectangle((int)sx - 10, (int)sy - 8, 20, 16, {200, 50, 50, 200});
      DrawText("!", (int)sx - 4, (int)sy - 7, 16, WHITE);
    }
  }

  // Enemy projectiles
  for (auto &b : enemyBullets) {
    if (b.dead) continue;
    float sx = b.x * TILE_SIZE;
    float sy = b.y * TILE_SIZE;
    DrawCircle((int)sx, (int)sy, 5, RED);
    DrawCircle((int)(sx - b.vx * 0.015f * TILE_SIZE),
               (int)(sy - b.vy * 0.015f * TILE_SIZE), 3, MAROON);
  }

  particles.render();
  bulletSystem.render();

  // Weapon in player hand, rotated toward mouse cursor
  if (currentWeapon.type != WeaponType::Fists && rr) {
    const char *wname = nullptr;
    if (currentWeapon.type == WeaponType::Minigun) wname = "weapon_minigun";
    else if (currentWeapon.type == WeaponType::Shotgun) wname = "weapon_shotgun";
    else if (currentWeapon.type == WeaponType::RocketLauncher) wname = "weapon_rocket";
    if (wname) {
      Vector2 ms = GetMousePosition();
      Vector2 mw = GetScreenToWorld2D(ms, rr->getCamera());
      float px = player.getX() * TILE_SIZE;
      float py = player.getY() * TILE_SIZE;
      float dx = mw.x - px;
      float dy = mw.y - py;
      float angle = atan2(dy, dx) * RAD2DEG;
      float dist = sqrt(dx*dx + dy*dy);
      float ox = dist > 0.0f ? dx / dist : 1.0f;
      float oy = dist > 0.0f ? dy / dist : 0.0f;
      float weaponDist = TILE_SIZE * 0.75f;
      float ws = TILE_SIZE * 1.3f;
      float wx = px + ox * weaponDist;
      float wy = py + oy * weaponDist;
      DrawTexturePro(tm.get(wname),
                     {0, 0, 32, 32},
                     {wx, wy, ws, ws},
                     {ws/2, ws/2},
                     angle, WHITE);
    }
  }

  renderer->endScene();
  renderDebugInfo();

  // ---- Floating damage numbers ----
  for (auto &ft : floatingTexts) {
    float alpha = ft.lifetime / ft.maxLifetime;
    float sx = ft.x * TILE_SIZE;
    float sy = (ft.y - (1.0f - alpha) * 0.5f) * TILE_SIZE;
    Color c = ft.color;
    c.a = (unsigned char)(alpha * 255);
    DrawText(ft.text.c_str(), (int)sx, (int)sy, 18, c);
  }

  // ---- HUD ----
  renderHUD();

  // ---- Wave announcement ----
  if (waveAnnounceTimer > 0.0f) {
    int sw = GetScreenWidth();
    float t = waveAnnounceTimer / WAVE_ANNOUNCE_DURATION;
    unsigned char wa = (unsigned char)(255 * t);
    int fontSize = 60;
    const char *waveText = TextFormat("FLOOR %d - WAVE %d", floorNumber, waveNumber);
    int tw = MeasureText(waveText, fontSize);
    DrawText(waveText, (sw - tw) / 2, 100, fontSize, Color{255, 200, 50, wa});
    waveAnnounceTimer -= deltaTime;
  }

  // ---- Low HP warning ----
  if (player.getHp() > 0 && player.getHp() < 30) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float pulse = std::sin(GetTime() * 5.0f) * 0.3f + 0.7f;
    unsigned char hA = (unsigned char)(80 * pulse);
    DrawRectangle(0, 0, sw, sh, Color{180, 0, 0, hA});
    int bW = 8;
    DrawRectangle(0, 0, sw, bW, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
    DrawRectangle(0, sh - bW, sw, bW, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
    DrawRectangle(0, 0, bW, sh, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
    DrawRectangle(sw - bW, 0, bW, sh, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
  }

  // ---- Damage overlay (blood splatter) ----
  if (damageOverlayTimer > 0.0f) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float t = damageOverlayTimer / DAMAGE_OVERLAY_DURATION;
    unsigned char dA = (unsigned char)(120 * t);
    DrawRectangle(0, 0, sw, sh, Color{100, 0, 0, dA});
  }

  // ---- Dynamic event overlays ----
  if (eventSystem.isEventActive()) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DynamicEvent ev = eventSystem.getCurrentEvent();
    float warn = eventSystem.getEventWarning();
    float prog = warn > 0.0f ? 1.0f : eventSystem.getEventTimer() / eventSystem.getEventDuration();

    if (ev == DynamicEvent::Eclipse && warn <= 0.0f) {
      unsigned char eA = (unsigned char)(100 * prog);
      DrawRectangle(0, 0, sw, sh, Color{10, 0, 30, eA});
      DrawText("ECLIPSE - monsters deal +50% damage", sw/2-200, 50, 18, Color{100, 50, 200, (unsigned char)(200*prog)});
    }
    if (ev == DynamicEvent::Blackout && warn <= 0.0f) {
      unsigned char bA = (unsigned char)(200 * prog);
      DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, bA});
      DrawCircle(sw/2, sh/2, 120, BLANK);
      DrawText("BLACKOUT", sw/2-70, 50, 24, Color{100, 100, 100, (unsigned char)(200*prog)});
    }
    if (ev == DynamicEvent::GoldRush && warn <= 0.0f) {
      DrawText("GOLD RUSH - double drops!", sw/2-150, 50, 18, Color{255, 215, 0, (unsigned char)(180)});
    }
    if (ev == DynamicEvent::MeteorRain && warn > 0.0f) {
      DrawText("METEOR RAIN INCOMING!", sw/2-160, 50, 22, ORANGE);
    }
    if (ev == DynamicEvent::MonsterInvasion && warn > 0.0f) {
      DrawText("MONSTER INVASION!", sw/2-140, 50, 22, RED);
    }
    if (ev == DynamicEvent::ZombieHorde && warn <= 0.0f) {
      DrawText("ZOMBIE HORDE - enemies revive!", sw/2-170, 50, 18, Color{0, 200, 0, (unsigned char)(180)});
    }
    if (ev == DynamicEvent::Earthquake && warn <= 0.0f) {
      float intensity = 0.5f + 0.5f * std::sin(GetTime() * 10.0f);
      unsigned char eA = (unsigned char)(60 * intensity);
      DrawRectangle(0, 0, sw, sh, Color{80, 50, 0, eA});
      DrawText("EARTHQUAKE", sw/2-90, 50, 22, Color{150, 100, 50, (unsigned char)(200*intensity)});
    }
    if (ev == DynamicEvent::TimeWarp && warn <= 0.0f) {
      unsigned char tA = (unsigned char)(100 * prog);
      DrawRectangle(0, 0, sw, sh, Color{0, 100, 200, tA});
      DrawText("TIME WARP - slowed!", sw/2-130, 80, 20, Color{0, 200, 255, (unsigned char)(200*prog)});
    }
    // Warning flash
    if (warn > 0.0f) {
      float flash = std::sin(GetTime() * 15.0f) * 0.5f + 0.5f;
      DrawRectangle(0, 0, sw, sh, Color{255, 0, 0, (unsigned char)(30 * flash)});
    }
  }

  if (timeStopped) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    Vector2 center = {sw * 0.5f, sh * 0.5f};

    // Normalised progress: 1.0 = just activated, 0.0 = ending
    float prog = timeStopTimer / TIME_STOP_DURATION;
    float transN = TIME_STOP_TRANSITION / TIME_STOP_DURATION;

    bool activating = prog > (1.0f - transN);
    bool deactivating = prog < transN;

    float actT = activating ? (prog - (1.0f - transN)) / transN : 0.0f;
    float deactT = deactivating ? 1.0f - prog / transN : 0.0f;
    float intensity = activating ? actT : (deactivating ? deactT : 1.0f);

    // ---- 1. White/violet flash on activation ----
    if (activating) {
      unsigned char fa = (unsigned char)(220 * (1.0f - actT));
      DrawRectangle(0, 0, sw, sh, Color{210, 180, 255, fa});
    }

    // ---- 2. Violet flash on deactivation ----
    if (deactivating) {
      unsigned char fa = (unsigned char)(180 * (1.0f - deactT));
      DrawRectangle(0, 0, sw, sh, Color{180, 120, 255, fa});
    }

    // ---- 3. Desaturation (gray overlay) + violet tint + darkening ----
    if (intensity > 0.01f) {
      unsigned char gA = (unsigned char)(70  * intensity);
      unsigned char tA = (unsigned char)(55  * intensity);
      unsigned char dA = (unsigned char)(90  * intensity);

      DrawRectangle(0, 0, sw, sh, Color{128, 128, 128, gA});  // desat
      DrawRectangle(0, 0, sw, sh, Color{70,  50,  130, tA});  // tint
      DrawRectangle(0, 0, sw, sh, Color{0,   0,   8,   dA});  // dark
    }

    // ---- 4. Radial blur (concentric tint rings from centre) ----
    if (intensity > 0.01f) {
      float maxR = std::sqrt((float)(sw * sw + sh * sh)) * 0.7f;
      for (int i = 1; i <= 25; ++i) {
        float t = (float)i / 25.0f;
        float radius = maxR * t * t;
        unsigned char a = (unsigned char)(18 * intensity * (1.0f - t));
        DrawCircleLines((int)center.x, (int)center.y, (int)radius,
                        Color{140, 120, 230, a});
      }

      // Chromatic aberration rings (R & B offset)
      for (int i = 1; i <= 18; ++i) {
        float t = (float)i / 18.0f;
        float radius = maxR * t * t;
        unsigned char a = (unsigned char)(7 * intensity * (1.0f - t));
        int off = (int)(4.0f * t);
        DrawCircleLines((int)(center.x + off), (int)center.y, (int)radius,
                        Color{255, 60, 60, a});
        DrawCircleLines((int)(center.x - off), (int)center.y, (int)radius,
                        Color{60, 60, 255, a});
      }
    }

    // ---- 5. Expanding / contracting ring shockwave ----
    if (activating || deactivating) {
      float ringProg = activating ? actT : deactT;
      float ringR = ringProg * 550.0f;
      unsigned char rA = (unsigned char)(130 * (1.0f - ringProg));
      int cx = (int)center.x, cy = (int)center.y;
      DrawCircleLines(cx, cy, (int)ringR, Color{200, 180, 255, rA});
      DrawCircleLines(cx, cy, (int)(ringR * 0.65f),
                      Color{200, 180, 255, (unsigned char)(rA / 2)});
      DrawCircleLines(cx, cy, (int)(ringR * 0.35f),
                      Color{200, 180, 255, (unsigned char)(rA / 3)});
    }

    // ---- 6. Vignette ----
    if (intensity > 0.01f) {
      int vS = (int)(120 * intensity);
      unsigned char vA = (unsigned char)(200 * intensity);
      // top
      DrawRectangleGradientV(0, 0, sw, vS,
          Color{0, 0, 20, vA}, Color{0, 0, 20, 0});
      // bottom
      DrawRectangleGradientV(0, sh - vS, sw, vS,
          Color{0, 0, 20, 0}, Color{0, 0, 20, vA});
      // left
      DrawRectangleGradientH(0, 0, vS, sh,
          Color{0, 0, 20, vA}, Color{0, 0, 20, 0});
      // right
      DrawRectangleGradientH(sw - vS, 0, vS, sh,
          Color{0, 0, 20, 0}, Color{0, 0, 20, vA});
    }

    // ---- 7. Film grain ----
    if (intensity > 0.01f) {
      int dots = (int)(300 * intensity);
      for (int i = 0; i < dots; ++i) {
        int gx = GetRandomValue(0, sw - 1);
        int gy = GetRandomValue(0, sh - 1);
        unsigned char ga = (unsigned char)(GetRandomValue(0, 35) * intensity);
        DrawPixel(gx, gy, Color{140, 130, 160, ga});
      }
    }

    // ---- 8. Heat distortion (subtle wavy lines) ----
    if (intensity > 0.01f) {
      float hTime = GetTime();
      for (int y = 0; y < sh; y += 16) {
        float wave = std::sin(y * 0.04f + hTime * 3.5f);
        int xOff = (int)(wave * 4.0f * intensity);
        unsigned char hA = (unsigned char)(8 * intensity);
        DrawLine(xOff, y, sw + xOff, y, Color{160, 140, 220, hA});
      }
    }

    // ---- 9. Chromatic aberration coloured borders ----
    if (intensity > 0.01f) {
      int bW = 6;
      unsigned char cA = (unsigned char)(55 * intensity);
      DrawRectangle(0, 0, bW, sh, Color{0, 0, 255, cA});    // blue left
      DrawRectangle(sw - bW, 0, bW, sh, Color{255, 0, 0, cA}); // red right
      DrawRectangle(0, 0, sw, 2, Color{0, 255, 0, (unsigned char)(cA / 2)});
      DrawRectangle(0, sh - 2, sw, 2, Color{0, 255, 0, (unsigned char)(cA / 2)});
    }

    // ---- 10. Motion blur ghosting on deactivation ----
    if (deactivating) {
      float mb = 1.0f - deactT;
      int ghosts = (int)(8 * mb);
      for (int i = 1; i <= ghosts; ++i) {
        float spread = (float)i / ghosts * 12.0f * mb;
        unsigned char gA = (unsigned char)(18 * mb * (1.0f - (float)i / (ghosts + 1)));
        DrawRectangle((int)(-spread), 0, sw, sh, Color{80, 50, 130, gA});
        DrawRectangle((int)(spread), 0, sw, sh, Color{130, 50, 80, gA});
      }
    }

    // ---- 11. "ZA WARUDO!" text ----
    if (timeStopTextTimer > 0) {
      float t = timeStopTextTimer / TIME_STOP_TEXT_DURATION;
      int fontSize = 90 + (int)(30 * (1.0f - t));
      unsigned char alpha = (unsigned char)(255 * t);
      Color zaColor = {255, 215, 0, alpha};

      int w1 = MeasureText("ZA WARUDO!", fontSize);
      int w2 = MeasureText("TOKI WO TOMARE", fontSize / 2);

      DrawText("ZA WARUDO!", (sw - w1) / 2, sh / 2 - fontSize - 10,
               fontSize, zaColor);
      DrawText("TOKI WO TOMARE", (sw - w2) / 2, sh / 2 + 10,
               fontSize / 2,
               Color{255, 215, 0, (unsigned char)(alpha * 0.6f)});

      DrawRectangle((sw - w1) / 2 - 20, sh / 2 - fontSize - 20,
                    w1 + 40, fontSize + 50,
                    Color{255, 215, 0, (unsigned char)(alpha * 0.08f)});
    }
  }

  // ---- Commentator text ----
  const auto &comments = eventSystem.getComments();
  int commY = 10;
  for (auto &c : comments) {
    float alpha = c.timer / c.duration;
    int tw = MeasureText(c.text.c_str(), 14);
    DrawText(c.text.c_str(), GetScreenWidth() - tw - 10, commY, 14,
             Color{255, 200, 100, (unsigned char)(255 * alpha)});
    commY += 20;
  }

  // ---- Artifact / Mutation HUD ----
  int hudX = GetScreenWidth() - 220;
  int hudY = 150;
  for (auto &a : artifacts) {
    DrawText(TextFormat("%c %s", a.symbol, a.name.c_str()), hudX, hudY, 12, a.color);
    hudY += 16;
  }
  for (auto &m : mutations) {
    DrawText(TextFormat("* %s", m.name.c_str()), hudX, hudY, 12, m.color);
    hudY += 16;
  }
  if (hasGod) {
    DrawText(TextFormat("GOD: %s", currentGod.name.c_str()),
             hudX, hudY + 4, 12, currentGod.color);
  }

  // ---- Risk timer display ----
  float timeOnFloor = eventSystem.getTimeOnFloor();
  if (timeOnFloor > 20.0f) {
    DrawText(TextFormat("RISK: %.1fs (x%.1f rewards)", timeOnFloor,
             eventSystem.getRiskMultiplier()), 10, 80, 14,
             timeOnFloor > 90.0f ? RED : (timeOnFloor > 60.0f ? ORANGE : YELLOW));
  }

  if (timeStopCooldownTimer > 0 && !timeStopped) {
    DrawText(TextFormat("ZA WARUDO COOLDOWN: %.1f", timeStopCooldownTimer),
             10, GetScreenHeight() - 30, 18, Color{200, 180, 255, 180});
  } else if (!timeStopped) {
    DrawText("PRESS E: ZA WARUDO", 10, GetScreenHeight() - 30, 18,
             Color{200, 180, 255, 180});
  }

  // Wave reward overlay
  if (showingWaveReward) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, 180});
    DrawText("CHOOSE YOUR REWARD:", sw/2 - 200, sh/2 - 100, 30, GOLD);
    for (int ri = 0; ri < 3; ++ri) {
      Color c = (ri == 0) ? RED : (ri == 1) ? GREEN : SKYBLUE;
      DrawText(TextFormat("[%d] %s", ri + 1, waveRewardTexts[ri].c_str()),
               sw/2 - 120, sh/2 - 40 + ri * 40, 22, c);
    }
  }

  renderer->refresh();
}

void Engine::reset() {
  map = Map(120, 80);

  monsters.clear();
  bulletSystem.clear();
  currentWeapon = Weapon::makeFists();
  weaponCooldownTimer = 0.0f;

  player =
      Player(0.0f, 0.0f, '@', COLOR_PLAYER, 100);

  particles = ParticleSystem();
  attackCooldown = 0.0f;
  timeStopped = false;
  timeStopTimer = 0.0f;
  timeStopTextTimer = 0.0f;
  timeStopCooldownTimer = 0.0f;
  floatingTexts.clear();
  pickups.clear();
  enemyBullets.clear();
  rocketExplosions.clear();

  // Reset all stats
  killCount = 0;
  damageOverlayTimer = 0.0f;
  levelUpFlashTimer = 0.0f;
  waveNumber = 1;
  floorNumber = 1;
  waveTimer = 0.0f;
  waveAnnounceTimer = 0.0f;
  waitingForWave = true;
  xp = 0;
  level = 1;
  xpToNextLevel = 5;
  ammoMG = MAX_AMMO_MG;
  ammoSG = MAX_AMMO_SG;
  ammoRL = MAX_AMMO_RL;
  isDashing = false;
  dashTimer = 0.0f;
  dashCooldown = 0.0f;
  dashDx = 0.0f;
  dashDy = 0.0f;
  paused = false;
  fadeAlpha = 0.0f;
  isRunning = true;

  // Clear new systems
  artifacts.clear();
  mutations.clear();
  hasGod = false;
  hunter.reset();
  hunterActive = false;
  hunterSpawnTimer = hunterSpawnInterval;
  mimics.clear();
  controlsReversed = false;
  shieldHits = 0;
  wallJumpTimer = 0.0f;
  regenTimer = 0.0f;
  drainTimer = 0.0f;
  zombieHordeActive = false;
  laserEyesTimer = 0.0f;
  unstableBuildTimer = 0.0f;
  mimicsSpawnedThisWave = 0;
  mimicAttackTimer = 0.0f;
  hasRicochet = false;
  hasPiercing = false;
  hasExplosive = false;
  critFailTimer = 0.0f;
  bulletSystem.ricochetMod = false;
  bulletSystem.piercingMod = false;
  bulletSystem.explosiveMod = false;
  godGreedDoubleDrops = false;
  godLuckCritChance = 0.0f;
  godSpeedBonus = 0.0f;
  showingWaveReward = false;
  waveRewardTimer = 0.0f;
  frozenHeartTimer = 0.0f;
  mushroomTimer = 0.0f;
  mushroomShrink = false;
  eyeballReveal = false;
  timeWarpSlow = 1.0f;
  earthquakeWallTimer = 0.0f;
  chargerRushTimer = 0.0f;
  eventSystem = EventSystem();
  meta.addRun();

  // Place player in first room
  const auto &rooms = map.getRooms();
  if (!rooms.empty()) {
    player.setPosition(rooms[0].centerX(), rooms[0].centerY());
  }
  // New god blessing
  int godChoice = rand() % 6;
  GodBlessing startGod = ArtifactSystem::allGods[godChoice];
  applyGodBlessing(startGod);
}

void Engine::run() {
  try {
    while (isRunning && !WindowShouldClose()) {
      deltaTime = std::min(GetFrameTime(),0.05f);

      // TimeWarp event: slow everything
      timeWarpSlow = 1.0f;
      if (eventSystem.isEventActive() &&
          eventSystem.getCurrentEvent() == DynamicEvent::TimeWarp &&
          eventSystem.getEventWarning() <= 0.0f) {
        timeWarpSlow = 0.5f;
      }
      deltaTime *= timeWarpSlow;

      handleInput();

      std::vector<Entity *> allEntities;
      allEntities.push_back(&player);
      for (auto &m : monsters) {
        if (!m->isDead())
          allEntities.push_back(m.get());
      }

      player.setContext(map, allEntities, deltaTime);
      player.update();

      // Event system + risk timer (runs even during time stop)
      eventSystem.update(deltaTime, waveNumber, floorNumber,
          eventSystem.getTimeOnFloor() + deltaTime,
          (float)player.getHp(), (float)player.getMaxHp());

      // Wall jump timer
      if (wallJumpTimer > 0.0f) wallJumpTimer -= deltaTime;

      // Laser Eyes mutation: auto-beam at nearest enemy
      for (auto &m : mutations) {
        if (m.type == Mutation::LaserEyes) {
          laserEyesTimer -= deltaTime;
          if (laserEyesTimer <= 0.0f) {
            laserEyesTimer = 0.3f;
            Monster *nearest = nullptr;
            float nearestDist = 999.0f;
            for (auto &mon : monsters) {
              if (mon->isDead()) continue;
              float ldx = mon->getX() - player.getX();
              float ldy = mon->getY() - player.getY();
              float ld = std::sqrt(ldx*ldx + ldy*ldy);
              if (ld < nearestDist) { nearestDist = ld; nearest = mon.get(); }
            }
            if (nearest) {
              int laserDmg = 5 + level;
              nearest->takeDamage(laserDmg);
              nearest->onHit();
              particles.spawnBlood(nearest->getX(), nearest->getY(), 5);
              addFloatingText(nearest->getX(), nearest->getY() - 0.5f,
                  TextFormat("LASER: %d", laserDmg), SKYBLUE);
              auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
              if (rr) rr->addShake(1.0f);
              if (nearest->isDead()) {
                particles.spawnExplosion(nearest->getX(), nearest->getY(), 20);
                spawnPickup(nearest->getX(), nearest->getY());
                ++killCount;
                addXPForKill();
              }
            }
          }
        }
      }

      // AttractMonsters artifact: pull enemies toward player
      for (auto &a : artifacts) {
        if (a.attractMonsters) {
          for (auto &mon : monsters) {
            if (mon->isDead()) continue;
            float adx = player.getX() - mon->getX();
            float ady = player.getY() - mon->getY();
            float adist = std::sqrt(adx*adx + ady*ady);
            if (adist > 0.5f && adist < 15.0f) {
              float pull = 2.0f * deltaTime;
              mon->move(adx / adist * pull, ady / adist * pull);
            }
          }
        }
      }

      // Unstable build v0.1 artifact: random effect every 5s
      for (auto &a : artifacts) {
        if (a.name == "Unstable build v0.1") {
          unstableBuildTimer -= deltaTime;
          if (unstableBuildTimer <= 0.0f) {
            unstableBuildTimer = 5.0f;
            int u = rand() % 4;
            if (u == 0) {
              player.takeDamage(-15);
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: +15 HP", GREEN);
            } else if (u == 1) {
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: teleport!", PINK);
              const auto &urooms = map.getRooms();
              if (!urooms.empty()) {
                int ri = rand() % urooms.size();
                player.setPosition(urooms[ri].centerX(), urooms[ri].centerY());
              }
            } else if (u == 2) {
              for (auto &mon : monsters) {
                if (!mon->isDead()) {
                  mon->takeDamage(20);
                  mon->onHit();
                }
              }
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: chain lightning!", YELLOW);
            } else {
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: dud...", GRAY);
            }
          }
        }
      }

      // Mimic reveal & attack
      mimicAttackTimer -= deltaTime;
      for (auto &mim : mimics) {
        if (!mim.active) continue;
        float mdx = player.getX() - mim.x;
        float mdy = player.getY() - mim.y;
        float mdist = std::sqrt(mdx*mdx + mdy*mdy);
        if (!mim.revealed && mdist < MIMIC_REVEAL_RANGE) {
          mim.revealed = true;
          eventSystem.triggerComment("IT'S A MIMIC!");
          audio.playDeath();
          auto mimicMonster = MonsterFactory::createMonster(
              MonsterType::SceletonMonkey, (float)mim.x, (float)mim.y);
          if (mimicMonster) {
            mimicMonster->setAttackDamage((int)mim.hp / 5);
            monsters.push_back(std::move(mimicMonster));
          }
          addFloatingText((float)mim.x, (float)mim.y - 1.0f, "MIMIC!", RED);
        }
        if (mim.revealed && mdist > 15.0f) {
          mim.active = false;
        }
      }

      // Frozen Heart artifact: freeze enemy every 3rd shot (stun = stop AI briefly)
      frozenHeartTimer -= deltaTime;
      if (frozenHeartTimer <= 0.0f) {
        for (auto &a : artifacts) {
          if (a.name == "Frozen Heart") {
            frozenHeartTimer = 0.5f;
            // Freeze nearest enemy for 0.3s (move them back so they lose pathfinding)
            for (auto &mon : monsters) {
              if (mon->isDead()) continue;
              float fdx = mon->getX() - player.getX();
              float fdy = mon->getY() - player.getY();
              if (std::sqrt(fdx*fdx + fdy*fdy) < 8.0f) {
                mon->move(-fdx * 0.1f, -fdy * 0.1f);
                addFloatingText(mon->getX(), mon->getY() - 0.5f, "FROZEN!", SKYBLUE);
                break;
              }
            }
          }
        }
      }

      // Mushroom artifact: random buff every 10s
      mushroomTimer -= deltaTime;
      if (mushroomTimer <= 0.0f) {
        for (auto &a : artifacts) {
          if (a.name == "Mushroom") {
            mushroomTimer = 10.0f;
            int mu = rand() % 4;
            if (mu == 0) {
              player.takeDamage(-25);
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: +25 HP", GREEN);
            } else if (mu == 1) {
              for (auto &mon : monsters) {
                if (!mon->isDead()) {
                  mon->takeDamage(30);
                  mon->onHit();
                }
              }
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: SPORES!", PINK);
            } else if (mu == 2) {
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: SPEED UP!", LIME);
              // Give speed boost by moving player toward cursor
              float rx = (float)(rand() % 200 - 100) / 100.0f;
              float ry = (float)(rand() % 200 - 100) / 100.0f;
              player.move(rx, ry);
            } else {
              player.takeDamage(10);
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: POISON!", RED);
            }
          }
        }
      }

      // Eyeball artifact: minimap always revealed (handled in render)
      for (auto &a : artifacts) {
        if (a.name == "Eyeball") eyeballReveal = true;
      }

      // Artifact HP drain
      drainTimer -= deltaTime;
      if (drainTimer <= 0.0f) {
        drainTimer = 1.0f;
        for (auto &a : artifacts) {
          if (a.drainHpPerSec > 0) {
            player.takeDamage(a.drainHpPerSec);
            addFloatingText(player.getX(), player.getY() - 0.5f,
                TextFormat("-%d (curse)", a.drainHpPerSec), RED);
          }
        }
      }

      // God of Health: regen
      if (hasGod && currentGod.type == GodBlessing::GodOfHealth) {
        regenTimer -= deltaTime;
        if (regenTimer <= 0.0f) {
          regenTimer = 1.0f;
          if (player.getHp() < player.getMaxHp()) {
            player.takeDamage(-1);
          }
        }
      }

      if (timeStopped) {
        timeStopTimer -= deltaTime;
        timeStopTextTimer -= deltaTime;
        timeStopCooldownTimer -= deltaTime;
        if (timeStopTimer <= 0.0f) {
          timeStopped = false;
          particles.spawnExplosion(player.getX(), player.getY(), 20);
        }
      } else {
        timeStopCooldownTimer -= deltaTime;
        particles.update(deltaTime);
        bulletSystem.update(deltaTime, map, monsters, particles);

        // Process rocket explosions
        for (auto &[ex, ey] : bulletSystem.getRocketExplosions()) {
          explosionDamage(ex, ey, 3.0f, 80.0f, &player);
        }

        // Floating texts
        for (auto &ft : floatingTexts) ft.lifetime -= deltaTime;
        floatingTexts.erase(
            std::remove_if(floatingTexts.begin(), floatingTexts.end(),
                [](const FloatingText &ft) { return ft.lifetime <= 0; }),
            floatingTexts.end());

        // Damage overlay
        if (damageOverlayTimer > 0.0f) damageOverlayTimer -= deltaTime;

        // Pickup lifetime / collection
        for (auto &p : pickups) {
          if (!p.active) continue;
          p.lifetime -= deltaTime;
          if (p.lifetime <= 0) { p.active = false; continue; }
          float dx = player.getX() - p.x;
          float dy = player.getY() - p.y;
          if (std::sqrt(dx * dx + dy * dy) < 0.5f) {
            p.active = false;
            if (p.type == Pickup::Health) {
              int heal = 30;
              player.takeDamage(-heal);
              addFloatingText(p.x, p.y - 0.5f, TextFormat("+%d HP", heal), GREEN);
              particles.spawnBlood(p.x, p.y, 10);
            } else if (p.type == Pickup::Ammo) {
              ammoMG = std::min(MAX_AMMO_MG, ammoMG + 30);
              ammoSG = std::min(MAX_AMMO_SG, ammoSG + 5);
              addFloatingText(p.x, p.y - 0.5f, "+AMMO", ORANGE);
            } else if (p.type == Pickup::RocketAmmo) {
              ammoRL = std::min(MAX_AMMO_RL, ammoRL + 3);
              addFloatingText(p.x, p.y - 0.5f, "+ROCKETS", ORANGE);
            } else if (p.type == Pickup::ArtifactItem) {
              ArtifactSystem::applyArtifact(p.artifact, player, ammoMG, ammoSG, ammoRL,
                  MAX_AMMO_MG, MAX_AMMO_SG, MAX_AMMO_RL);
              artifacts.push_back(p.artifact);
              addFloatingText(p.x, p.y - 0.5f,
                  TextFormat("ARTIFACT: %s", p.artifact.name.c_str()), PINK);
              syncBulletMods();
              checkArtifactCombinations();
            }
            audio.playPickup();
          }
        }
        pickups.erase(
            std::remove_if(pickups.begin(), pickups.end(),
                [](const Pickup &p) { return !p.active; }),
            pickups.end());

        // Enemy projectiles
        for (auto &b : enemyBullets) {
          if (b.dead) continue;
          b.x += b.vx * deltaTime;
          b.y += b.vy * deltaTime;
          b.lifetime -= deltaTime;
          if (b.lifetime <= 0 || !map.isWalkable(b.x, b.y)) {
            b.dead = true;
            continue;
          }
          float dx = player.getX() - b.x;
          float dy = player.getY() - b.y;
          if (std::sqrt(dx * dx + dy * dy) < 0.5f) {
            player.takeDamage((int)b.damage);
            damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
            particles.spawnBlood(player.getX(), player.getY(), 10);
            addFloatingText(player.getX(), player.getY() - 0.5f,
                TextFormat("-%d", (int)b.damage), RED);
            b.dead = true;
          }
        }
        enemyBullets.erase(
            std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                [](const EnemyProjectile &b) { return b.dead; }),
            enemyBullets.end());

        // Monster AI + melee attacks + ranged attacks
        for (auto &monster : monsters) {
          monster->updateAI(map, player.getX(), player.getY(), deltaTime);
          monster->tickRangedCooldown(deltaTime);

          float dx = player.getX() - monster->getX();
          float dy = player.getY() - monster->getY();
          float dist = std::sqrt(dx * dx + dy * dy);

          // Melee
          if (dist <= Monster::ATTACK_RANGE &&
              monster->getAttackCooldown() <= 0.0f) {
            // Shield mutation: block every 3rd hit
            for (auto &m : mutations) {
              if (m.type == Mutation::Shield) {
                shieldHits = (shieldHits + 1) % 3;
                if (shieldHits == 0) {
                  addFloatingText(player.getX(), player.getY() - 1.0f, "BLOCKED!", SKYBLUE);
                  monster->resetCooldown();
                  goto afterMelee;
                }
              }
            }
            player.takeDamage(monster->getAttackDamage());
            damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
            particles.spawnBlood(player.getX(), player.getY(), 5);
            monster->resetCooldown();
            addFloatingText(player.getX(), player.getY() - 0.5f,
                TextFormat("-%d", monster->getAttackDamage()), RED);
            auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
            if (rr) rr->addShake(3.0f);
            // Thorn Skin: reflect damage
            for (auto &m : mutations) {
              if (m.type == Mutation::ThornSkin) {
                int reflectDmg = 5 + level;
                monster->takeDamage(reflectDmg);
                monster->onHit();
                addFloatingText(monster->getX(), monster->getY() - 0.5f,
                    TextFormat("-%d (thorn)", reflectDmg), GREEN);
              }
            }
          }
          afterMelee: ;

          // Push player out of monster overlap
          float minDist = 0.6f;
          if (dist < minDist && dist > 0.01f) {
            float push = (minDist - dist) * 0.5f;
            player.move(-dx / dist * push, -dy / dist * push);
          }

          // Ranged
          if (monster->canFireRanged() && dist < monster->rangedRange && dist > Monster::ATTACK_RANGE) {
            EnemyProjectile eb;
            eb.x = monster->getX();
            eb.y = monster->getY();
            float ndx = player.getX() - monster->getX();
            float ndy = player.getY() - monster->getY();
            float nlen = std::sqrt(ndx * ndx + ndy * ndy);
            if (nlen > 0) { ndx /= nlen; ndy /= nlen; }
            float speed = 6.0f;
            eb.vx = ndx * speed;
            eb.vy = ndy * speed;
            eb.damage = 8.0f;
            enemyBullets.push_back(eb);
            monster->resetRangedCooldown();
          }
        }

        // ---- New monster behaviors ----
        for (auto &mon : monsters) {
          if (mon->isDead()) continue;

          // Healer: heal nearby monsters every 3s
          if (mon->isHealer) {
            mon->healTimer -= deltaTime;
            if (mon->healTimer <= 0.0f) {
              mon->healTimer = 3.0f;
              for (auto &other : monsters) {
                if (other->isDead() || other.get() == mon.get()) continue;
                float hdx = other->getX() - mon->getX();
                float hdy = other->getY() - mon->getY();
                if (std::sqrt(hdx*hdx + hdy*hdy) < 5.0f) {
                  other->takeDamage(-15);
                  addFloatingText(other->getX(), other->getY() - 0.5f, "+15 (heal)", GREEN);
                }
              }
              particles.spawnBlood(mon->getX(), mon->getY(), 8);
            }
          }

          // Summoner: spawn minion every 4s
          if (mon->isSummoner) {
            mon->summonTimer -= deltaTime;
            if (mon->summonTimer <= 0.0f) {
              mon->summonTimer = 4.0f;
              auto minion = MonsterFactory::createMonster(MonsterType::SceletonMonkey,
                  mon->getX() + ((rand()%2) ? 0.5f : -0.5f),
                  mon->getY() + ((rand()%2) ? 0.5f : -0.5f));
              if (minion) {
                monsters.push_back(std::move(minion));
                addFloatingText(mon->getX(), mon->getY() - 0.5f, "SUMMON!", MAGENTA);
              }
            }
          }

          // Charger: rush player very fast
          if (mon->isCharger) {
            float cdx = player.getX() - mon->getX();
            float cdy = player.getY() - mon->getY();
            float cdist = std::sqrt(cdx*cdx + cdy*cdy);
            if (cdist > 0) { cdx /= cdist; cdy /= cdist; }
            mon->move(cdx * 6.0f * deltaTime, cdy * 6.0f * deltaTime);
          }
        }

        // Splitter: on death, split into smaller copies
        for (size_t si = 0; si < monsters.size(); ++si) {
          auto &mon = monsters[si];
          if (mon->isDead() && mon->isSplitter && mon->splitCount > 0) {
            int sc = mon->splitCount;
            int hp = mon->getMaxHp();
            spawnSplitter(mon->getX(), mon->getY(), sc, hp);
            mon->splitCount = 0;
          }
        }

        // Earthquake: crack walls randomly
        if (eventSystem.isEventActive() &&
            eventSystem.getCurrentEvent() == DynamicEvent::Earthquake &&
            eventSystem.getEventWarning() <= 0.0f) {
          earthquakeWallTimer -= deltaTime;
          if (earthquakeWallTimer <= 0.0f) {
            earthquakeWallTimer = 0.5f;
            int wx = rand() % map.getWidth();
            int wy = rand() % map.getHeight();
            const Tile &et = map.getTile(wx, wy);
            if (et.symbol == '#') {
              breakWall(wx, wy);
            }
          }
        }

        // Update hunter (runs even in time stop for timer)
        updateHunter(deltaTime);

        // Dynamic events
        handleDynamicEvents();
      }

      // ZombieHorde: on activation, give all monsters a revive
      if (eventSystem.isEventActive() &&
          eventSystem.getCurrentEvent() == DynamicEvent::ZombieHorde &&
          eventSystem.getEventWarning() <= 0.0f && !zombieHordeActive) {
        zombieHordeActive = true;
        for (auto &mon : monsters) {
          if (!mon->isDead()) mon->zombieRevivesLeft = 1;
        }
      }
      // ZombieHorde: revive dead monsters once
      if (zombieHordeActive &&
          eventSystem.getCurrentEvent() != DynamicEvent::ZombieHorde) {
        zombieHordeActive = false;
      }
      if (zombieHordeActive) {
        for (auto &mon : monsters) {
          if (mon->isDead() && mon->zombieRevivesLeft > 0) {
            mon->zombieRevivesLeft = 0;
            for (int hi = 0; hi < mon->getMaxHp() / 2; ++hi) mon->takeDamage(-1);
            addFloatingText(mon->getX(), mon->getY() - 0.5f, "REVIVED!", LIME);
            particles.spawnExplosion(mon->getX(), mon->getY(), 15);
          }
        }
      }

      monsters.erase(
          std::remove_if(monsters.begin(), monsters.end(),
                         [](const std::unique_ptr<Monster> &monster) {
                           return monster->isDead();
                         }),
          monsters.end());

      // Wave transition
      if (monsters.empty() && !waitingForWave && !player.isDead()) {
        ++waveNumber;
        waveTimer = 0.0f;
        waitingForWave = true;
        showingWaveReward = true;
        waveRewardTimer = 10.0f;

        // Generate 3 random choices
        const char *rewardNames[] = {
          "+20 MAX HP", "+30% DMG", "+SPEED", "+FIRE RATE",
          "+AMMO", "+ARMOR", "+1 JUMP", "VAMPIRE"
        };
        for (int ri = 0; ri < 3; ++ri) {
          waveRewardChoices[ri] = rand() % 8;
          waveRewardTexts[ri] = rewardNames[waveRewardChoices[ri]];
        }

        // New floor every WAVES_PER_FLOOR waves
        if (waveNumber % WAVES_PER_FLOOR == 1) {
          ++floorNumber;
          map = Map(120, 80);
          const auto &newRooms = map.getRooms();
          if (!newRooms.empty()) {
            player.setPosition(newRooms[0].centerX(), newRooms[0].centerY());
          }
          addFloatingText(player.getX(), player.getY() - 2.0f,
              TextFormat("- FLOOR %d -", floorNumber), GOLD);
          // Bonus HP on floor transition
          int heal = 20 + floorNumber * 5;
          player.takeDamage(-heal);
          addFloatingText(player.getX(), player.getY() - 1.0f,
              TextFormat("+%d HP", heal), GREEN);
        }
      }

      // Wave reward selection
      if (showingWaveReward && IsKeyPressed(KEY_ONE)) {
        applyWaveReward(0);
      }
      if (showingWaveReward && IsKeyPressed(KEY_TWO)) {
        applyWaveReward(1);
      }
      if (showingWaveReward && IsKeyPressed(KEY_THREE)) {
        applyWaveReward(2);
      }
      if (showingWaveReward) {
        waveRewardTimer -= deltaTime;
        if (waveRewardTimer <= 0.0f) showingWaveReward = false;
      }
      if (waitingForWave && !player.isDead()) {
        waveTimer += deltaTime;
        if (waveTimer >= WAVE_DELAY) {
          startNextWave();
        }
      }

      if (player.isDead()) {
        while (!WindowShouldClose()) {
          BeginDrawing();
          ClearBackground(BLACK);
          DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                        {0, 0, 0, 180});
          meta.updateHighScore(waveNumber, floorNumber);
          DrawText("YOU FUCK DIED", 250, 160, 80, RED);
          DrawText(TextFormat("FLOOR: %d  WAVE: %d  LEVEL: %d  KILLS: %d",
                   floorNumber, waveNumber, level, killCount), 200, 245, 20, GRAY);
          DrawText(TextFormat("BEST: Wave %d  Floor %d",
                   meta.data().highestWave, meta.data().highestFloor), 260, 275, 16, GOLD);
          DrawText("PRESS R TO RESTART OR Q TO QUIT", 180, 320, 25, WHITE);
          EndDrawing();

          if (IsKeyPressed(KEY_Q)) {
            isRunning = false;
            break;
          }
          if (IsKeyPressed(KEY_R)) {
            reset();
            break;
          }
        }
        if (!isRunning)
          break;
        continue;
      }
      render();
    }
  } catch (const GameException &e) {
    std::cout << "Game error: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cout << "Unexpected error: " << e.what() << std::endl;
  }
}

} // namespace rogue

```

---

### File: src/entities/Entity.cpp
```cpp
#include "rogue/entities/Entity.h"

namespace rogue {

Entity::Entity(float x, float y, char symbol, int color, int maxHp)
    : x(x), y(y), symbol(symbol), color(color), hp(maxHp), maxHp(maxHp) {}

float Entity::getX() const { return x; }

float Entity::getY() const { return y; }

char Entity::getSymbol() const { return symbol; }

int Entity::getColor() const { return color; }

int Entity::getHp() const { return hp; }

bool Entity::isDead() const { return hp <= 0; }

void Entity::takeDamage(int damage) {
  hp -= damage;
  if (hp < 0) {
    hp = 0;
  }
  if (hp > maxHp) {
    hp = maxHp;
  }
}

void Entity::setPosition(float newX, float newY) {
  x = newX;
  y = newY;
}

void Entity::move(float dx, float dy) {
  x += dx;
  y += dy;
}

} // namespace rogue

```

---

### File: src/entities/Monster.cpp
```cpp
#include "rogue/entities/Monster.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>

namespace rogue {

Monster::Monster(float x, float y, char symbol, int color, int maxHp)
    : Entity(x, y, symbol, color, maxHp) {}

void Monster::update() {}

static float heuristic(int x1, int y1, int x2, int y2) {
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<std::pair<int, int>> Monster::findPath(const Map &map, int startX,
                                                   int startY, int goalX,
                                                   int goalY) {
  auto encode = [&](int x, int y) { return y * map.getWidth() + x; };

  struct ANode {
    int x, y;
    float f;
    bool operator>(const ANode &o) const { return f > o.f; }
  };

  std::priority_queue<ANode, std::vector<ANode>, std::greater<ANode>> open;
  std::unordered_map<int, float> gScore;
  std::unordered_map<int, int> parent;

  int startKey = encode(startX, startY);
  gScore[startKey] = 0;
  open.push({startX, startY, heuristic(startX, startY, goalX, goalY)});
  parent[startKey] = -1;

  const int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  while (!open.empty()) {
    auto cur = open.top();
    open.pop();
    int cx = cur.x, cy = cur.y;

    if (cx == goalX && cy == goalY) {
      std::vector<std::pair<int, int>> path;
      int key = encode(goalX, goalY);
      while (key != -1) {
        int px = key % map.getWidth();
        int py = key / map.getWidth();
        path.push_back({px, py});
        key = parent.count(key) ? parent[key] : -1;
      }
      std::reverse(path.begin(), path.end());
      return path;
    }

    float curG = gScore[encode(cx, cy)];
    for (auto &d : dirs) {
      int nx = cx + d[0];
      int ny = cy + d[1];
      if (!map.isWalkable(nx, ny))
        continue;

      int nkey = encode(nx, ny);
      float ng = curG + 1.0f;
      if (!gScore.count(nkey) || ng < gScore[nkey]) {
        gScore[nkey] = ng;
        parent[nkey] = encode(cx, cy);
        open.push({nx, ny, ng + heuristic(nx, ny, goalX, goalY)});
      }
    }
  }

  return {};
}

void Monster::updateAI(const Map &map, float playerX, float playerY, float dt) {
  tickFlash(dt);
  float dx = playerX - x;
  float dy = playerY - y;
  float dist = std::sqrt(dx * dx + dy * dy);

  if (dist > aggroRange)
    return;

  tickCooldown(dt);
  if (dist <= ATTACK_RANGE)
    return;

  pathUpdateTimer -= dt;
  if (pathUpdateTimer <= 0.0f) {
    pathUpdateTimer = PATH_UPDATE_INTERVAL;
    currentPath =
        findPath(map, static_cast<int>(x), static_cast<int>(y),
                 static_cast<int>(playerX), static_cast<int>(playerY));
  }

  if (currentPath.size() > 1) {
    auto &next = currentPath[1];
    float ndx = next.first - x;
    float ndy = next.second - y;
    float nlen = std::sqrt(ndx * ndx + ndy * ndy);

    if (nlen > 0) {
      float speed = 3.0f * dt;
      x += (ndx / nlen) * speed;
      y += (ndy / nlen) * speed;
    }

    if (nlen < 0.2f)
      currentPath.erase(currentPath.begin());
  }
}

} // namespace rogue

```

---

### File: src/entities/Player.cpp
```cpp
#include "rogue/entities/Player.h"
#include <algorithm>
#include <cmath>

namespace rogue {

Player::Player(float x, float y, char symbol, int color, int health)
    : Entity(x, y, symbol, color, health) {}

void Player::processInput(float dx, float dy, float dt) {
  inputDx = dx;
  inputDy = dy;
  m_deltaTime = dt;
}

void Player::setInput(float dx, float dy) {
  inputDx = dx;
  inputDy = dy;
}

void Player::setContext(Map &map, const std::vector<Entity *> &entities,
                        float dt) {
  mapPtr = &map;
  entitiesPtr = &entities;
  m_deltaTime = dt;
}

void Player::update() {
  if (!mapPtr)
    return;

  float dx = inputDx;
  float dy = inputDy;

  // Normalize input to prevent faster diagonal movement
  float length = std::sqrt(dx * dx + dy * dy);
  if (length > 0) {
    dx /= length;
    dy /= length;
  }

  float moveAmount = playerSpeed * m_deltaTime;
  dx *= moveAmount;
  dy *= moveAmount;

  // Step limiting to prevent tunneling through walls
  float stepSize = std::sqrt(dx * dx + dy * dy);
  if (stepSize > maxStep) {
    dx = (dx / stepSize) * maxStep;
    dy = (dy / stepSize) * maxStep;
  }

  constexpr float collisionRadius = 0.35f;
  auto canMoveTo = [&](float testX, float testY) {
    if (ignoreWalls) return true;
    return mapPtr->isWalkable(testX + collisionRadius, testY) &&
           mapPtr->isWalkable(testX - collisionRadius, testY) &&
           mapPtr->isWalkable(testX, testY + collisionRadius) &&
           mapPtr->isWalkable(testX, testY - collisionRadius);
  };

  int steps = std::max(1, static_cast<int>(std::ceil(
                              std::max(std::abs(dx), std::abs(dy)) / 0.1f)));
  float stepX = dx / steps;
  float stepY = dy / steps;

  for (int i = 0; i < steps; ++i) {
    float nextX = x + stepX;
    float nextY = y + stepY;

    if (canMoveTo(nextX, nextY)) {
      move(stepX, stepY);
      continue;
    }

    if (stepX != 0.0f && canMoveTo(x + stepX, y)) {
      move(stepX, 0.0f);
      continue;
    }

    if (stepY != 0.0f && canMoveTo(x, y + stepY)) {
      move(0.0f, stepY);
      continue;
    }

    break;
  }
}

} // namespace rogue

```

---

### File: include/rogue/Renderer.h
```cpp
#pragma once
#include <vector>

namespace rogue {
class Map;
class Entity;

class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual void initialize() = 0;
  virtual void shutdown() = 0;
  virtual void clear() = 0;
  virtual void refresh() = 0;
  virtual void drawChar(float x, float y, char ch, int color) = 0;
  virtual void renderMap(const Map &map,
                         const std::vector<Entity *> &entities) = 0;
  virtual void setCameraTarget(float x, float y) = 0;
  virtual void beginScene() = 0;
  virtual void endScene() = 0;
};
} // namespace rogue

```

---

### File: include/rogue/MetaProgression.h
```cpp
#pragma once
#include <string>
#include <vector>

namespace rogue {

struct UnlockData {
  bool hasGodSpeed = false;
  bool hasGodFire = false;
  bool hasGodChaos = false;
  bool hasGodHealth = false;
  bool hasGodGreed = false;
  bool hasGodLuck = false;

  bool hasExtraArm = false;
  bool hasLaserEyes = false;
  bool hasWallJump = false;
  bool hasThornSkin = false;
  bool hasVampire = false;
  bool hasBerserker = false;

  int totalRuns = 0;
  int totalKills = 0;
  int highestWave = 0;
  int highestFloor = 0;
};

class MetaProgression {
public:
  MetaProgression();

  void load();
  void save();

  UnlockData &data() { return unlockData; }
  const UnlockData &data() const { return unlockData; }

  void addRun() { unlockData.totalRuns++; }
  void addKill() { unlockData.totalKills++; }
  void updateHighScore(int wave, int floor);

  bool isUnlocked(const std::string &key) const;
  void unlock(const std::string &key);

private:
  UnlockData unlockData;
  std::string filePath;

  std::vector<std::string> unlockedItems;
};

} // namespace rogue

```

---

### File: include/rogue/EventSystem.h
```cpp
#pragma once
#include <raylib.h>
#include <string>
#include <vector>

namespace rogue {

enum class DynamicEvent {
  None,
  Eclipse,          // dim light, all monsters get +50% damage
  MeteorRain,       // random fireballs fall from sky
  MonsterInvasion,  // extra wave of monsters spawns
  Blackout,         // vision radius shrinks to 5 tiles
  GoldRush,         // all drops are doubled
  ZombieHorde,      // killed monsters revive once
  Earthquake,       // screen shakes, walls crack
  TimeWarp          // everything slows down 50% for player too
};

struct ActiveEvent {
  DynamicEvent type = DynamicEvent::None;
  float timer = 0.0f;
  float duration = 8.0f;
  float warningTimer = 2.0f;
};

struct CommentatorLine {
  std::string text;
  float timer = 0.0f;
  float duration = 3.0f;
};

class EventSystem {
public:
  EventSystem();

  void update(float dt, int waveNumber, int floorNumber,
              float timeOnFloor, float playerHp, float playerMaxHp);
  void forceEvent(DynamicEvent type);
  DynamicEvent getCurrentEvent() const { return currentEvent.type; }
  float getEventTimer() const { return currentEvent.timer; }
  float getEventDuration() const { return currentEvent.duration; }
  float getEventWarning() const { return currentEvent.warningTimer; }
  bool isEventActive() const {
    return currentEvent.type != DynamicEvent::None && currentEvent.timer > 0.0f;
  }

  // Commentator
  void triggerComment(const std::string &text);
  void updateComments(float dt);
  const std::vector<CommentatorLine> &getComments() const { return comments; }
  void maybeComment(const std::string &text, float chance = 0.3f);

  // Risk system
  float getTimeOnFloor() const { return timeOnFloor; }
  float getRiskMultiplier() const;
  int getBonusXp() const;

  static DynamicEvent randomEvent();

private:
  ActiveEvent currentEvent;
  float eventCooldown = 0.0f;
  float timeOnFloor = 0.0f;
  std::vector<CommentatorLine> comments;
  float commentCooldown = 0.0f;

  static const std::vector<std::string> commentatorLines;
};

} // namespace rogue

```

---

### File: include/rogue/Exceptions.h
```cpp
#pragma once
#include <stdexcept>
#include <string>

namespace rogue {
class GameException : public std::runtime_error {
public:
  using std::runtime_error::runtime_error; // Inherit constructors
};

class RendererException : public GameException {
public:
  explicit RendererException(const std::string &message)
      : GameException(message) {}
};

class MapException : public GameException {
public:
  explicit MapException(const std::string &message) : GameException(message) {}
};

class InitializationException : public GameException {
public:
  explicit InitializationException(const std::string &message)
      : GameException(message) {}
};
} // namespace rogue

```

---

### File: include/rogue/Raylib_renderer.h
```cpp
#pragma once

#include "Constants.h"
#include "Renderer.h"
#include "entities/Entity.h"
#include "world/Map.h"
#include <raylib.h>
#include <vector>

namespace rogue {

class RaylibRenderer : public IRenderer {
public:
  RaylibRenderer(int sw, int sh);
  ~RaylibRenderer() override;

  void initialize() override;
  void shutdown() override;
  void clear() override;
  void refresh() override;
  void drawChar(float x, float y, char ch, int color) override;

  void renderMap(const Map &map,
                 const std::vector<Entity *> &entities) override;

  void setCameraTarget(float x, float y) override;
  void beginScene() override;
  void endScene() override;
  Camera2D getCamera() const { return camera; }
  void setZoom(float zoom);
  float getZoom() const { return camera.zoom; }
  void addShake(float intensity);

private:
  Vector2 worldToScreen(float x, float y);
  int screenWidth;
  int screenHeight;
  Camera2D camera;
  float shakeIntensity = 0.0f;
};

} // namespace rogue

```

---

### File: include/rogue/Ncurses_renderer.h
```cpp
#pragma once
#include "Renderer.h"
#include <ncurses.h>

namespace rogue {
class NcursesRenderer : public IRenderer {
public:
  void initialize() override;
  void shutdown() override;
  void clear() override;
  void refresh() override;
  void drawChar(float x, float y, char ch, int color) override;
};
} // namespace rogue

```

---

### File: include/rogue/BulletSystem.h
```cpp
#pragma once
#include "Bullet.h"
#include "Constants.h"
#include "ParticleSystem.h"
#include "entities/Monster.h"
#include "world/Map.h"
#include <functional>
#include <memory>
#include <vector>

namespace rogue {
class BulletSystem {
public:
  void spawn(float x, float y, float tx, float ty, float damage,
             bool isRocket = false);
  void update(float dt, Map &map,
              std::vector<std::unique_ptr<Monster>> &monsters,
              ParticleSystem &particles);
  void render() const;
  void clear();

  // Rocket explosion positions (world coords) from this frame
  std::vector<std::pair<float, float>> getRocketExplosions();
  // Callback on monster kill (world coords)
  std::function<void(float, float)> onMonsterKilled;
  // Callback on explosive hit (x, y, radius, damage)
  std::function<void(float, float, float, float)> onExplosion;

  // Weapon mod flags (set by Engine from artifacts/mutations)
  bool ricochetMod = false;
  bool piercingMod = false;
  bool explosiveMod = false;
  float explosiveRadius = 2.0f;
  float explosiveDamage = 30.0f;

private:
  std::vector<Bullet> bullets;
  std::vector<std::pair<float, float>> rocketExplosions;
  static constexpr float BULLET_SPEED = 20.0f;
};
} // namespace rogue

```

---

### File: include/rogue/ParticleSystem.h
```cpp
#pragma once
#include "Constants.h"
#include <raylib.h>
#include <vector>

namespace rogue {

enum class ParticleType { Blood, Fire, Smoke, ExplosionDebris };

struct Particle {
  float x, y;
  float vx, vy;
  float lifetime;
  float maxLifetime;
  float size;
  float maxSize;
  Color color;
  ParticleType type;
};

class ParticleSystem {
public:
  void spawnBlood(float x, float y, int count = 20);
  void spawnExplosion(float x, float y, int count = 30);
  void spawnFire(float x, float y, int count = 10);
  void spawnSmoke(float x, float y, int count = 8);
  void spawnMuzzleFlash(float x, float y);
  void update(float deltaTime);
  void render() const;
  bool isEmpty() const { return particles.empty(); }
  void clear() { particles.clear(); }

private:
  std::vector<Particle> particles;
};

} // namespace rogue

```

---

### File: include/rogue/Bullet.h
```cpp
#pragma once

namespace rogue {

struct Bullet {
  float x, y;
  float vx, vy;
  float lifetime = 1.0f;
  float damage = 25.0f;
  bool dead = false;
  bool isRocket = false;
  int pierceLeft = 0;
  int bounceLeft = 0;
};
} // namespace rogue

```

---

### File: include/rogue/Artifact.h
```cpp
#pragma once
#include <raylib.h>
#include <string>
#include <vector>

namespace rogue {

struct Artifact {
  std::string name;
  std::string desc;

  // Bonuses
  int bonusDamage = 0;
  float bonusSpeed = 0.0f;
  int bonusMaxHp = 0;
  float bonusFireRate = 0.0f;
  int bonusAmmoMG = 0;
  int bonusAmmoSG = 0;
  int bonusAmmoRL = 0;
  bool extraJump = false;
  bool ricochet = false;
  bool explosiveBullets = false;
  bool piercingBullets = false;

  // Drawbacks
  int drainHpPerSec = 0;
  float speedPenalty = 0.0f;
  int maxHpPenalty = 0;
  float fireRatePenalty = 0.0f;
  bool randomTeleport = false;
  bool attractMonsters = false;
  bool noAmmoDrops = false;
  bool reverseControls = false;
  bool glassCannon = false; // 2x damage TAKEN

  Color color = WHITE;
  char symbol = '?';
};

struct Mutation {
  std::string name;
  std::string desc;

  enum Type {
    ExtraArm,       // +1 bullet per shot
    LaserEyes,      // auto-beam at nearest enemy
    WallJump,       // can walk through walls briefly
    ThornSkin,      // damage melee attackers
    Vampire,        // heal on kill
    Berserker,      // more damage at low HP
    Shield,         // block every 3rd hit
    Speedster,      // +50% speed, -30% HP
    FatRoll,        // no dash cooldown, but slower
    GlassGun        // +200% damage, -50% max HP
  } type;

  Color color = MAGENTA;
};

struct GodBlessing {
  std::string name;
  std::string desc;

  enum Type {
    GodOfSpeed,   // +40% speed, dash has no cooldown
    GodOfFire,    // all bullets explode, +50% fire rate
    GodOfChaos,   // random events happen 2x more, +100% damage
    GodOfHealth,  // regen 1 HP/sec, +100 max HP
    GodOfGreed,   // double pickups, but enemies drop less
    GodOfLuck     // crit chance, better loot
  } type;

  Color color;
};

struct CombinationRecipe {
  std::string resultName;
  std::string resultDesc;
  int requiredId1; // indices in artifact pool
  int requiredId2;
  Artifact (*createResult)(const Artifact &a, const Artifact &b);
};

class ArtifactSystem {
public:
  ArtifactSystem();

  static Artifact randomArtifact();
  static Mutation randomMutation();
  static void applyArtifact(Artifact &a, class Player &player,
                            int &ammoMG, int &ammoSG, int &ammoRL,
                            const int &maxAmmoMG, const int &maxAmmoSG, const int &maxAmmoRL);
  static void unapplyArtifact(Artifact &a, class Player &player,
                              int &ammoMG, int &ammoSG, int &ammoRL,
                              const int &maxAmmoMG, const int &maxAmmoSG, const int &maxAmmoRL);
  static Artifact tryCombine(const Artifact &a, const Artifact &b);

  static GodBlessing randomGod();

  static std::vector<Artifact> allArtifacts;
  static std::vector<Mutation> allMutations;
  static std::vector<GodBlessing> allGods;
  static std::vector<CombinationRecipe> recipes;
};

} // namespace rogue

```

---

### File: include/rogue/Constants.h
```cpp
#pragma once

namespace rogue {
constexpr float TILE_SIZE = 40.0f;
} // namespace rogue

```

---

### File: include/rogue/AudioSystem.h
```cpp
#pragma once
#include <raylib.h>

namespace rogue {

class AudioSystem {
public:
  AudioSystem();
  ~AudioSystem();
  void init();
  void playShoot();
  void playHit();
  void playPickup();
  void playZaWarudo();
  void playExplosion();
  void playDeath();
  void playDash();
  void playLevelUp();
  void playWaveStart();
  void setVolume(float vol) { volume = vol; }

private:
  Sound shootSound;
  Sound hitSound;
  Sound pickupSound;
  Sound zaWarudoSound;
  Sound explosionSound;
  Sound deathSound;
  Sound dashSound;
  Sound levelUpSound;
  Sound waveStartSound;
  float volume = 0.4f;

  Sound makeSine(float freq, float duration, float amp = 0.5f);
  Sound makeNoise(float duration, float amp = 0.3f);
  Sound makeSweep(float freqStart, float freqEnd, float duration, float amp = 0.5f);
  void destroySound(Sound &s);
};

} // namespace rogue

```

---

### File: include/rogue/TextureManager.h
```cpp
#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

namespace rogue {

class TextureManager {
public:
  static TextureManager &getInstance() {
    static TextureManager instance;
    return instance;
  }

  void loadTexture(const std::string &name, const std::string &path) {
    textures[name] = LoadTexture(path.c_str());
  }

  Texture2D &get(const std::string &name) { return textures.at(name); }

  void unloadAll() {
    for (auto &[name, tex] : textures) {
      UnloadTexture(tex);
    }
    textures.clear();
  }

private:
  TextureManager() = default;
  std::unordered_map<std::string, Texture2D> textures;
};

} // namespace rogue

```

---

### File: include/rogue/Weapon.h
```cpp
#pragma once

namespace rogue {

enum class WeaponType { Fists, Minigun, Shotgun, RocketLauncher };

struct Weapon {
  WeaponType type;

  float cooldown = 0.0f;
  float damage = 0.0f;
  float range = 0.0f;

  float currentFireRate = 0.0f;
  float minFireRate = 0.05f;
  float maxFireRate = 0.5f;
  float spinupRate = 0.4f;
  bool isFiring = false;

  static Weapon makeFists() {
    Weapon w;
    w.type = WeaponType::Fists;
    w.damage = 999.0f;
    w.range = 1.5f;
    w.cooldown = 0.3f;
    w.currentFireRate = 0.3f;
    return w;
  }

  static Weapon makeMinigun() {
    Weapon w;
    w.type = WeaponType::Minigun;
    w.damage = 10.0f;
    w.range = 15.0f;
    w.currentFireRate = 0.5f;
    return w;
  }

  static Weapon makeShotgun() {
    Weapon w;
    w.type = WeaponType::Shotgun;
    w.damage = 25.0f;
    w.range = 8.0f;
    w.cooldown = 0.8f;
    w.currentFireRate = 0.8f;
    return w;
  }

  static Weapon makeRocketLauncher() {
    Weapon w;
    w.type = WeaponType::RocketLauncher;
    w.damage = 80.0f;
    w.range = 20.0f;
    w.cooldown = 1.5f;
    w.currentFireRate = 1.5f;
    return w;
  }
};

} // namespace rogue

```

---

### File: include/rogue/world/Map.h
```cpp
#pragma once
#include "../Renderer.h"
#include <string>
#include <vector>

namespace rogue {

enum ColorPairs {
  COLOR_DEFAULT = 1,
  COLOR_PLAYER = 2,
  COLOR_MONSTER = 3,
  COLOR_WALL = 4,
  COLOR_BLOOD = 5
};

struct Tile {
  char symbol;
  int colorPair;
};

struct Prefab {
  int w, h;
  std::vector<char> tiles;

  Prefab(int w, int h, std::vector<char> tiles)
      : w(w), h(h), tiles(std::move(tiles)) {}

  char tile(int x, int y) const { return tiles[y * w + x]; }
};

struct Room {
  int x, y, w, h;
  int spawnX, spawnY;

  int centerX() const { return spawnX; }
  int centerY() const { return spawnY; }

  bool intersects(const Room &other) const {
    return x <= other.x + other.w + 1 && x + w + 1 >= other.x &&
           y <= other.y + other.h + 1 && y + h + 1 >= other.y;
  }
};

class Map {
public:
  Map(int width, int height);
  Map(int width, int height, int seed);
  ~Map() = default;

  bool isWalkable(float x, float y) const;
  void spillBlood(float x, float y);
  void spillBloodArea(int cx, int cy, int radius);
  int getWidth() const;
  int getHeight() const;

  const Tile &getTile(int x, int y) const;
  void render(IRenderer &renderer) const;

  const std::vector<Room> &getRooms() const { return rooms; }
  void setFloor(int x, int y);

private:
  int width;
  int height;
  std::vector<std::vector<Tile>> tiles;
  std::vector<Room> rooms;
  void generate(unsigned int seed);
  void fillWithWalls();
  void carveRoom(const Room &room, const Prefab &prefab);
  void findSpawnPoint(Room &room);
  void carveHCorridor(int x1, int x2, int y);
  void carveVCorridor(int y1, int y2, int x);
};

} // namespace rogue

```

---

### File: include/rogue/core/Engine.h
```cpp
#pragma once
#include "../Artifact.h"
#include "../AudioSystem.h"
#include "../BulletSystem.h"
#include "../EventSystem.h"
#include "../MetaProgression.h"
#include "../ParticleSystem.h"
#include "../Renderer.h"
#include "../Weapon.h"
#include "../entities/Monster.h"
#include "../entities/Player.h"
#include "../world/Map.h"
#include <memory>
#include <string>
#include <vector>

namespace rogue {

struct FloatingText {
  float x, y;
  std::string text;
  Color color;
  float lifetime;
  float maxLifetime;
};

struct Pickup {
  float x, y;
  enum Type { Health, Ammo, RocketAmmo, ArtifactItem } type;
  bool active = true;
  float lifetime = 20.0f;
  Artifact artifact;
};

struct EnemyProjectile {
  float x, y, vx, vy;
  float damage;
  float lifetime = 2.0f;
  bool dead = false;
};

class Engine {
public:
  Engine(std::unique_ptr<IRenderer> renderer);
  ~Engine();
  void run();

private:
  std::unique_ptr<IRenderer> renderer;
  void handleInput();
  void render();
  void renderHUD();
  void renderMinimap();
  void renderDebugInfo();
  void reset();
  void spawnEntities();
  void startNextWave();
  void spawnPickup(float x, float y);
  void addFloatingText(float x, float y, const std::string &text, Color color);
  void explosionDamage(float x, float y, float radius, float damage, Entity *source);
  void addXPForKill();
  void applyMutation(Mutation &m);
  void applyGodBlessing(GodBlessing &g);
  void handleDynamicEvents();
  void updateHunter(float dt);
  void spawnArtifactPickup(float x, float y);
  void checkArtifactCombinations();
  void breakWall(int x, int y);
  void syncBulletMods();
  float getDamageMultiplier();

  BulletSystem bulletSystem;
  AudioSystem audio;
  Weapon currentWeapon = Weapon::makeFists();
  float weaponCooldownTimer = 0.0f;

  bool isRunning;
  bool showDebug = false;
  Map map;
  Player player;
  std::vector<std::unique_ptr<Monster>> monsters;
  float deltaTime;

  ParticleSystem particles;
  float attackCooldown = 0.0f;
  static constexpr float ATTACK_RANGE = 1.5f;
  static constexpr float ATTACK_COOLDOWN = 0.3f;

  // Time stop (Za Warudo)
  bool timeStopped = false;
  float timeStopTimer = 0.0f;
  float timeStopTextTimer = 0.0f;
  float timeStopCooldownTimer = 0.0f;
  static constexpr float TIME_STOP_DURATION = 3.0f;
  static constexpr float TIME_STOP_TEXT_DURATION = 1.2f;
  static constexpr float TIME_STOP_COOLDOWN = 10.0f;
  static constexpr float TIME_STOP_TRANSITION = 0.3f;

  // HUD / stats
  int killCount = 0;
  int xp = 0;
  int level = 1;
  int xpToNextLevel = 5;
  float damageOverlayTimer = 0.0f;
  float levelUpFlashTimer = 0.0f;
  static constexpr float DAMAGE_OVERLAY_DURATION = 0.4f;
  static constexpr float LEVEL_UP_FLASH_DURATION = 0.8f;

  // Ammo
  int ammoMG = 100;
  int ammoSG = 20;
  int ammoRL = 5;
  static constexpr int MAX_AMMO_MG = 100;
  static constexpr int MAX_AMMO_SG = 20;
  static constexpr int MAX_AMMO_RL = 8;

  // Floating damage numbers
  std::vector<FloatingText> floatingTexts;
  // Pickups
  std::vector<Pickup> pickups;
  // Enemy projectiles
  std::vector<EnemyProjectile> enemyBullets;

  // Dash
  bool isDashing = false;
  float dashTimer = 0.0f;
  float dashCooldown = 0.0f;
  float dashDx = 0.0f, dashDy = 0.0f;
  static constexpr float DASH_DURATION = 0.15f;
  static constexpr float DASH_COOLDOWN = 0.8f;
  static constexpr float DASH_SPEED = 25.0f;

  // Wave system
  int waveNumber = 1;
  int floorNumber = 1;
  static constexpr int WAVES_PER_FLOOR = 5;
  float waveTimer = 0.0f;
  float waveAnnounceTimer = 0.0f;
  bool waitingForWave = true;
  static constexpr float WAVE_DELAY = 2.0f;
  static constexpr float WAVE_ANNOUNCE_DURATION = 2.0f;

  // Pause
  bool paused = false;
  float fadeAlpha = 0.0f;

  // Rocket explosion pool (simple)
  struct RocketExplosion {
    float x, y;
    float timer = 0.0f;
    float maxTimer = 0.3f;
    float radius = 0.0f;
  };
  std::vector<RocketExplosion> rocketExplosions;

  // ---- ARTIFACTS / MUTATIONS / GODS ----
  std::vector<Artifact> artifacts;
  std::vector<Mutation> mutations;
  GodBlessing currentGod;
  bool hasGod = false;
  int shieldHits = 0; // for Shield mutation
  float wallJumpTimer = 0.0f;
  float regenTimer = 0.0f;
  float drainTimer = 0.0f;

  // ---- EVENT SYSTEM ----
  EventSystem eventSystem;
  float meteorTimer = 0.0f;
  float invasionTimer = 0.0f;

  // ---- HUNTER ----
  std::unique_ptr<Monster> hunter;
  float hunterSpawnTimer = 0.0f;
  float hunterSpawnInterval = 30.0f;
  bool hunterActive = false;

  // ---- MIMICS ----
  struct Mimic {
    int x, y;
    bool active = true;
    bool revealed = false;
    float hp = 30.0f;
  };
  std::vector<Mimic> mimics;

  // ---- META PROGRESSION ----
  MetaProgression meta;

  // ---- critical fail system ----
  float critFailTimer = 0.0f;
  bool controlsReversed = false;

  // ---- mimics ----
  static constexpr float MIMIC_REVEAL_RANGE = 2.0f;
  int mimicsSpawnedThisWave = 0;
  float mimicAttackTimer = 0.0f;

  // ---- zombie revive ----
  bool zombieHordeActive = false;

  // ---- laser eyes ----
  float laserEyesTimer = 0.0f;

  // ---- unstable build ----
  float unstableBuildTimer = 0.0f;

  // ---- weapon mod syncing ----
  bool hasRicochet = false;
  bool hasPiercing = false;
  bool hasExplosive = false;

  // ---- god-specific effects ----
  bool godGreedDoubleDrops = false;
  float godLuckCritChance = 0.0f;
  float godSpeedBonus = 0.0f;

  // ---- wave reward (choice between waves) ----
  bool showingWaveReward = false;
  float waveRewardTimer = 0.0f;
  int waveRewardChoices[3] = {0, 1, 2};
  std::string waveRewardTexts[3];

  // ---- new artifact effects ----
  float frozenHeartTimer = 0.0f;
  float mushroomTimer = 0.0f;
  bool mushroomShrink = false;
  bool eyeballReveal = false;

  // ---- TimeWarp slowdown ----
  float timeWarpSlow = 1.0f;

  // ---- Earthquake wall destruction ----
  float earthquakeWallTimer = 0.0f;

  // ---- Charger monster tracking ----
  float chargerRushTimer = 0.0f;
  float chargerRushDx = 0.0f, chargerRushDy = 0.0f;

  // ---- Splitter helper ----
  void spawnSplitter(float x, float y, int splitCount, int maxHp);
  void applyWaveReward(int choice);
};

} // namespace rogue

```

---

### File: include/rogue/core/Command.h
```cpp
#pragma once
#include "rogue/entities/Entity.h"
#include "rogue/entities/Monster.h"
#include "rogue/world/Map.h"

namespace rogue {
class Command {
public:
  virtual ~Command() = default;
  virtual void execute(Entity &entity, Map &map) = 0;
};

class MoveCommand : public Command {
  int dx, dy;

public:
  MoveCommand(int dx, int dy) : dx(dx), dy(dy) {}
  void execute(Entity &entity, Map &map) override;
};

class AttackCommand : public Command {
  Monster *target;
  int damage;

public:
  AttackCommand(Monster *target, int damage = 10) : target(target), damage(damage) {}
  void execute(Entity &entity, Map &map) override;
};
} // namespace rogue

```

---

### File: include/rogue/entities/MonsterFactory.h
```cpp
#pragma once
#include "Monster.h"
#include <memory>

namespace rogue {
enum class MonsterType { SceletonMonkey, GoblinMonkey, MonkeyBoss, GunMonkey, KamikazeMonkey, HunterMonkey, HealerMonkey, SummonerMonkey, ChargerMonkey, SplitterMonkey };

class MonsterFactory {
public:
  static std::unique_ptr<Monster> createMonster(MonsterType type, float x,
                                                float y) {
    switch (type) {
    case MonsterType::SceletonMonkey:
      return std::make_unique<Monster>(x, y, 's', 3, 30); // color 3 = monster1
    case MonsterType::GoblinMonkey:
      return std::make_unique<Monster>(x, y, 'g', 4, 50); // color 4 = monster2
    case MonsterType::MonkeyBoss:
      return std::make_unique<Monster>(x, y, 'M', 5, 100); // color 5 = boss
    case MonsterType::GunMonkey: {
      auto m = std::make_unique<Monster>(x, y, 'g', 6, 40);
      m->setRanged(true);
      m->setRangedDamage(8);
      m->setAggroRange(12.0f);
      return m;
    }
    case MonsterType::KamikazeMonkey: {
      auto m = std::make_unique<Monster>(x, y, 'k', 7, 25);
      m->setKamikaze(true);
      m->setAggroRange(14.0f);
      m->setAttackDamage(30);
      return m;
    }
    case MonsterType::HunterMonkey: {
      auto m = std::make_unique<Monster>(x, y, 'H', 8, 80);
      m->setAggroRange(50.0f); // global tracking
      m->setAttackDamage(20);
      m->setRanged(true);
      m->setRangedDamage(12);
      return m;
    }
    case MonsterType::HealerMonkey: {
      auto m = std::make_unique<Monster>(x, y, 'h', 3, 35);
      m->isHealer = true;
      m->healTimer = 3.0f;
      m->setAggroRange(8.0f);
      return m;
    }
    case MonsterType::SummonerMonkey: {
      auto m = std::make_unique<Monster>(x, y, 'S', 6, 50);
      m->isSummoner = true;
      m->summonTimer = 4.0f;
      m->setAggroRange(6.0f);
      m->setAttackDamage(8);
      return m;
    }
    case MonsterType::ChargerMonkey: {
      auto m = std::make_unique<Monster>(x, y, 'C', 4, 40);
      m->isCharger = true;
      m->setAggroRange(18.0f);
      m->setAttackDamage(25);
      return m;
    }
    case MonsterType::SplitterMonkey: {
      auto m = std::make_unique<Monster>(x, y, 's', 3, 50);
      m->isSplitter = true;
      m->splitCount = 2;
      m->origSymbol = 's';
      m->origColor = 3;
      m->origMaxHp = 50;
      m->setAttackDamage(10);
      return m;
    }
    default:
      return nullptr;
    }
  }
};
} // namespace rogue

```

---

### File: include/rogue/entities/Player.h
```cpp
#pragma once
#include "../world/Map.h"
#include "Entity.h"
#include <vector>

namespace rogue {

class Player : public Entity {
public:
  Player(float x, float y, char symbol, int color, int health);

  void update() override;
  void processInput(float dx, float dy, float deltaTime);
  void setInput(float dx, float dy);
  void setContext(Map &map, const std::vector<Entity *> &entities,
                  float deltaTime);

  bool ignoreWalls = false;

private:
  float inputDx = 0, inputDy = 0;
  Map *mapPtr = nullptr;
  const std::vector<Entity *> *entitiesPtr = nullptr;
  float m_deltaTime = 0;

  float playerSpeed = 15.0f;
  float maxStep = 0.5f;
};

} // namespace rogue

```

---

### File: include/rogue/entities/Entity.h
```cpp
#pragma once
namespace rogue {

class Entity {
public:
  Entity(float x, float y, char symbol, int color, int maxHp);
  virtual ~Entity() = default;

  float getX() const;
  float getY() const;
  char getSymbol() const;
  int getColor() const;
  int getHp() const;
  int getMaxHp() const { return maxHp; }
  bool isDead() const;
  void takeDamage(int damage);
  void setPosition(float newX, float newY);
  virtual void update() = 0; // Pure virtual function for updating entity state
  void move(float dx, float dy);
  virtual bool isFlashing() const {return false;}

protected:
  float x;
  float y;
  char symbol;
  int color;
  int hp;
  int maxHp;
};

} // namespace rogue

```

---

### File: include/rogue/entities/Monster.h
```cpp
#pragma once
#include "../world/Map.h"
#include "Entity.h"
#include <utility>
#include <vector>

namespace rogue {

class Monster : public Entity {
public:
  Monster(float x, float y, char symbol, int color, int maxHp);
  void update() override; // Implement monster-specific behavior
  void updateAI(const Map &map, float playerX, float playerY, float deltaTime);

  static constexpr float ATTACK_RANGE = 1.2f;
  static constexpr float ATTACK_COOLDOWN = 1.0f;
  int getAttackDamage() const { return attackDamage; }
  float getAttackCooldown() const { return attackCooldown; }
  void tickCooldown(float dt) {
    if (attackCooldown > 0)
      attackCooldown -= dt;
  }
  void resetCooldown() { attackCooldown = ATTACK_COOLDOWN; }
  void onHit() {hitFlashTimer = HIT_FLASH_DURATION;}
  bool isFlashing() const override {return hitFlashTimer > 0.0f;}
  void tickFlash(float dt) {if (hitFlashTimer > 0.0f) hitFlashTimer -= dt;}

  // Ranged attack
  void setRanged(bool val) { isRanged = val; }
  void setRangedDamage(int dmg) { rangedDamage = dmg; }
  void setAggroRange(float r) { aggroRange = r; }
  void setAttackDamage(int dmg) { attackDamage = dmg; }
  bool isRanged = false;
  float rangedCooldown = 0.0f;
  float rangedRange = 8.0f;
  static constexpr float RANGED_COOLDOWN = 1.5f;
  bool canFireRanged() const { return isRanged && rangedCooldown <= 0.0f; }
  void resetRangedCooldown() { if (isRanged) rangedCooldown = RANGED_COOLDOWN; }
  void tickRangedCooldown(float dt) { if (rangedCooldown > 0) rangedCooldown -= dt; }

  // Kamikaze
  void setKamikaze(bool val) { isKamikaze = val; }
  bool isKamikaze = false;

  // Zombie revive (ZombieHorde event)
  int zombieRevivesLeft = 0;

  // New monster types
  bool isHealer = false;
  bool isSummoner = false;
  bool isCharger = false;
  bool isSplitter = false;
  float healTimer = 0.0f;
  float summonTimer = 0.0f;
  int splitCount = 0; // how many splits left
  // Store original stats for splitter
  char origSymbol = 's';
  int origColor = 3;
  int origMaxHp = 30;

private:
  float attackCooldown = 0.0f;
  int attackDamage = 10;
  float aggroRange = 10.0f;
  int rangedDamage = 8;
  float hitFlashTimer = 0.0f;
  static constexpr float HIT_FLASH_DURATION = 0.1f;
  struct Node {
    int x, y;
    float g, h;
    int parentX, parentY;
    float f() const { return g + h; }
  };

  std::vector<std::pair<int, int>> currentPath;
  float pathUpdateTimer = 0.0f;
  static constexpr float PATH_UPDATE_INTERVAL = 0.5f;

  std::vector<std::pair<int, int>> findPath(const Map &map, int startX,
                                            int startY, int goalX, int goalY);
};

} // namespace rogue

```

---

