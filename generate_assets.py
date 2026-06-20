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
        20,
        18,
        16,  # 1: very dark brown
        35,
        30,
        25,  # 2: dark brown
        50,
        42,
        35,  # 3: medium dark brown
        70,
        60,
        50,  # 4: brown
        90,
        80,
        65,  # 5: light brown
        115,
        105,
        88,  # 6: tan
        145,
        135,
        115,  # 7: light tan
        180,
        170,
        150,  # 8: pale tan
        220,
        215,
        200,  # 9: warm white
        255,
        60,
        40,  # 10: red eyes
        180,
        25,
        20,  # 11: dark red
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


def monster_16x16(draw_func):
    pixels = [[0] * 16 for _ in range(16)]
    draw_func(pixels)
    return pixels


# SceletonMonkey - skull-like skeleton monkey
def _sceleton(pixels):
    draw_circle(pixels, 7, 6, 5, 2)
    draw_circle(pixels, 7, 6, 4, 10)
    draw_circle(pixels, 2, 5, 2, 2)
    draw_circle(pixels, 2, 5, 1, 11)
    draw_circle(pixels, 12, 5, 2, 2)
    draw_circle(pixels, 12, 5, 1, 11)
    draw_circle(pixels, 5, 5, 2, 1)
    draw_circle(pixels, 9, 5, 2, 1)
    draw_circle(pixels, 5, 5, 1, 9)
    draw_circle(pixels, 9, 5, 1, 9)
    draw_rect(pixels, 6, 9, 3, 1, 1)
    draw_rect(pixels, 6, 9, 1, 1, 9)
    draw_rect(pixels, 8, 9, 1, 1, 9)
    draw_rect(pixels, 5, 11, 5, 4, 3)
    draw_rect(pixels, 4, 12, 2, 2, 4)
    draw_rect(pixels, 10, 12, 2, 2, 4)


sceleton = monster_16x16(_sceleton)


# GoblinMonkey - green goblin monkey with pointy ears
def _goblin(pixels):
    draw_circle(pixels, 7, 6, 5, 2)
    draw_circle(pixels, 7, 6, 4, 4)
    draw_circle(pixels, 2, 5, 2, 2)
    draw_circle(pixels, 2, 5, 1, 6)
    draw_circle(pixels, 12, 5, 2, 2)
    draw_circle(pixels, 12, 5, 1, 6)
    draw_rect(pixels, 0, 3, 2, 3, 2)
    draw_rect(pixels, 0, 4, 1, 1, 1)
    draw_rect(pixels, 14, 3, 2, 3, 2)
    draw_rect(pixels, 15, 4, 1, 1, 1)
    draw_circle(pixels, 5, 5, 1, 9)
    draw_circle(pixels, 9, 5, 1, 9)
    draw_rect(pixels, 5, 5, 1, 1, 1)
    draw_rect(pixels, 9, 5, 1, 1, 1)
    draw_rect(pixels, 3, 3, 3, 1, 10)
    draw_rect(pixels, 10, 3, 3, 1, 10)
    draw_rect(pixels, 5, 0, 6, 2, 10)
    draw_rect(pixels, 6, 9, 3, 1, 1)
    draw_rect(pixels, 5, 11, 5, 4, 5)
    draw_rect(pixels, 4, 12, 2, 2, 6)
    draw_rect(pixels, 10, 12, 2, 2, 6)


goblin = monster_16x16(_goblin)


# GunMonkey - monkey with a gun barrel arm
def _gun_monkey(pixels):
    draw_circle(pixels, 7, 6, 5, 2)
    draw_circle(pixels, 7, 6, 4, 5)
    draw_circle(pixels, 2, 5, 2, 2)
    draw_circle(pixels, 2, 5, 1, 6)
    draw_circle(pixels, 12, 5, 2, 2)
    draw_circle(pixels, 12, 5, 1, 6)
    draw_circle(pixels, 5, 5, 1, 1)
    draw_circle(pixels, 9, 5, 1, 1)
    draw_rect(pixels, 6, 9, 3, 1, 1)
    draw_rect(pixels, 5, 11, 5, 4, 3)
    draw_rect(pixels, 4, 12, 2, 2, 4)
    draw_rect(pixels, 10, 12, 2, 2, 4)
    draw_rect(pixels, 12, 12, 4, 2, 7)
    draw_rect(pixels, 14, 11, 2, 4, 2)
    draw_rect(pixels, 13, 13, 3, 1, 8)


gun_monkey = monster_16x16(_gun_monkey)


# KamikazeMonkey - crazy explosive monkey
def _kamikaze(pixels):
    draw_circle(pixels, 7, 6, 5, 2)
    draw_circle(pixels, 7, 6, 4, 4)
    draw_circle(pixels, 2, 5, 2, 2)
    draw_circle(pixels, 2, 5, 1, 5)
    draw_circle(pixels, 12, 5, 2, 2)
    draw_circle(pixels, 12, 5, 1, 5)
    draw_circle(pixels, 5, 5, 1, 1)
    draw_circle(pixels, 9, 5, 1, 1)
    draw_rect(pixels, 6, 9, 3, 1, 1)
    draw_rect(pixels, 3, 11, 10, 4, 10)
    draw_rect(pixels, 4, 11, 8, 4, 11)
    draw_rect(pixels, 6, 12, 2, 1, 12)
    draw_rect(pixels, 9, 12, 1, 1, 12)
    draw_rect(pixels, 7, 1, 1, 3, 10)
    draw_circle(pixels, 7, 0, 1, 11)
    draw_rect(pixels, 2, 12, 2, 2, 5)
    draw_rect(pixels, 12, 12, 2, 2, 5)


kamikaze = monster_16x16(_kamikaze)


# HunterMonkey - hooded stealth monkey hunter
def _hunter(pixels):
    draw_rect(pixels, 4, 0, 8, 2, 2)
    draw_rect(pixels, 3, 2, 10, 2, 3)
    draw_rect(pixels, 2, 4, 12, 2, 4)
    draw_circle(pixels, 7, 6, 4, 5)
    draw_circle(pixels, 2, 5, 2, 4)
    draw_circle(pixels, 2, 5, 1, 6)
    draw_circle(pixels, 12, 5, 2, 4)
    draw_circle(pixels, 12, 5, 1, 6)
    draw_circle(pixels, 5, 5, 1, 10)
    draw_circle(pixels, 9, 5, 1, 10)
    draw_rect(pixels, 6, 9, 3, 1, 11)
    draw_rect(pixels, 5, 11, 5, 4, 5)
    draw_rect(pixels, 4, 14, 8, 2, 3)
    draw_rect(pixels, 4, 12, 2, 2, 6)
    draw_rect(pixels, 10, 12, 2, 2, 6)


hunter = monster_16x16(_hunter)


# BossMonkey - big boss monkey with crown
def _boss(pixels):
    draw_rect(pixels, 4, 11, 8, 4, 3)
    draw_rect(pixels, 5, 11, 6, 4, 4)
    draw_circle(pixels, 7, 6, 5, 2)
    draw_circle(pixels, 7, 6, 4, 4)
    draw_circle(pixels, 2, 5, 2, 2)
    draw_circle(pixels, 2, 5, 1, 5)
    draw_circle(pixels, 12, 5, 2, 2)
    draw_circle(pixels, 12, 5, 1, 5)
    draw_rect(pixels, 4, 0, 8, 3, 10)
    draw_rect(pixels, 5, 0, 6, 3, 11)
    draw_rect(pixels, 7, 0, 2, 1, 10)
    draw_rect(pixels, 5, 1, 1, 1, 9)
    draw_rect(pixels, 10, 1, 1, 1, 9)
    draw_circle(pixels, 5, 5, 1, 1)
    draw_circle(pixels, 9, 5, 1, 1)
    draw_rect(pixels, 6, 9, 3, 1, 1)
    draw_rect(pixels, 5, 12, 6, 3, 12)
    draw_rect(pixels, 3, 12, 2, 3, 5)
    draw_rect(pixels, 11, 12, 2, 3, 5)


boss = monster_16x16(_boss)

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
