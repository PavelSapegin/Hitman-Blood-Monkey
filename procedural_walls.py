#!/usr/bin/env python3
"""
Процедурная генерация текстур стен для 2D Roguelike проекта.
Пиксель-арт стиль, генерация сразу в целевом разрешении.
"""

from PIL import Image
import os
import math
import random

# -------------------------------------------------------------------
# ПАЛИТРЫ
# -------------------------------------------------------------------

PALETTES = {
    "wall_stone": [
        0,
        0,
        0,
        0,
        20,
        20,
        24,  # 1: mortar
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
    "wall_reinforced": [
        0,
        0,
        0,
        0,
        36,
        36,
        42,  # 1: mortar
        56,
        56,
        64,  # 2: base
        76,
        76,
        86,  # 3: medium
        100,
        100,
        112,  # 4: light
        130,
        130,
        140,  # 5: highlight
        70,
        45,
        20,  # 6: dark wood
        110,
        78,
        40,  # 7: medium wood
        150,
        110,
        60,  # 8: light wood
    ],
    "wall_destroyed": [
        0,
        0,
        0,
        0,
        36,
        36,
        42,  # 1: mortar
        56,
        56,
        64,  # 2: base
        76,
        76,
        86,  # 3: medium
        100,
        100,
        112,  # 4: light
        130,
        130,
        140,  # 5: highlight
        18,
        15,
        12,  # 6: dark void
    ],
    "wall_bloody": [
        0,
        0,
        0,
        0,
        36,
        36,
        42,  # 1: mortar
        56,
        56,
        64,  # 2: base
        76,
        76,
        86,  # 3: medium
        100,
        100,
        112,  # 4: light
        130,
        130,
        140,  # 5: highlight
        140,
        25,
        25,  # 6: blood red
        190,
        35,
        35,  # 7: bright blood
        90,
        12,
        12,  # 8: dark blood
    ],
    "wall_metal": [
        0,
        0,
        0,
        0,
        40,
        42,
        48,  # 1: gap
        60,
        62,
        68,  # 2: panel base
        80,
        82,
        88,  # 3: panel mid
        110,
        112,
        118,  # 4: panel light
        150,
        152,
        158,  # 5: highlight
        200,
        205,
        215,  # 6: bright edge
        25,
        28,
        35,  # 7: rivet
    ],
    "grayscale": [
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
    ],
}


def save_palette_png(name, pixels, palette_key, out_dir):
    h = len(pixels)
    w = len(pixels[0])
    palette = PALETTES[palette_key]
    img = Image.new("P", (w, h))
    img.putpalette(palette)
    for y in range(h):
        for x in range(w):
            img.putpixel((x, y), pixels[y][x])
    img.info["transparency"] = 0
    path = os.path.join(out_dir, name)
    img.save(path)
    print(f"  -> {path}")


def _fill(px, x, y, w, h, color):
    for j in range(y, y + h):
        for i in range(x, x + w):
            if 0 <= j < len(px) and 0 <= i < len(px[0]):
                px[j][i] = color


def _draw_vline(px, x, y0, y1, color):
    for y in range(y0, y1 + 1):
        if 0 <= y < len(px) and 0 <= x < len(px[0]):
            px[y][x] = color


def _draw_hline(px, y, x0, x1, color):
    for x in range(x0, x1 + 1):
        if 0 <= y < len(px) and 0 <= x < len(px[0]):
            px[y][x] = color


# ================================================================
# КАМЕННАЯ КЛАДКА
# ================================================================


def _bricks_16(S=16):
    """
    Генерация кирпичной кладки в S×S.
    Рендерер использует source rect {0,0,16,16}, поэтому S=16.
    Кирпичи 5×3, раствор 1px, перевязка рядов.
    """
    rng = random.Random(42)
    px = [[0] * S for _ in range(S)]

    bw, bh = 5, 3
    mg = 1
    step_y = bh + mg
    step_x = bw + mg

    # Рисуем кирпичи
    for row in range(0, S + step_y, step_y):
        row_idx = row // step_y
        offset = (row_idx % 2) * ((bw + mg) // 2)
        for col in range(-step_x, S + step_x, step_x):
            bx = col + offset
            _fill(px, bx, row, bw, bh, 1)

    # Убираем растворные линии (там где mg) — они уже 0, оставляем

    # Превращаем 1 → цвета с фаской и шумом
    for y in range(S):
        for x in range(S):
            if px[y][x] == 0:
                continue  # раствор

            # проверка соседей для фаски
            left = px[y][x - 1] if x > 0 else 0
            up = px[y - 1][x] if y > 0 else 0
            right = px[y][x + 1] if x + 1 < S else 0
            down = px[y + 1][x] if y + 1 < S else 0

            if left == 0 or up == 0:
                px[y][x] = 4  # highlight (светлый)
            elif right == 0 or down == 0:
                px[y][x] = 1  # shadow (тёмный, как раствор)
            else:
                px[y][x] = 3  # base

    # Шум
    for y in range(S):
        for x in range(S):
            if px[y][x] not in (0, 4, 1) and rng.random() < 0.08:
                px[y][x] = rng.choice([2, 5])

    return px


# ================================================================
# СТЕНЫ
# ================================================================


def generate_stone(size=16):
    px = _bricks_16(size)
    return px, "wall_stone"


def generate_reinforced(size=16):
    px = _bricks_16(size)
    rng = random.Random(73)
    S = size

    # Два вертикальных бруса
    for bx in [S // 4, 3 * S // 4]:
        bw = max(4, S // 12)
        for y in range(S):
            for x in range(bx - bw // 2, bx + bw // 2):
                if 0 <= x < S:
                    if y % 4 < 2:
                        px[y][x] = 7  # medium wood
                    else:
                        px[y][x] = 8  # light wood
            # тень слева от балки
            lx = bx - bw // 2 - 1
            if lx >= 0:
                for y in range(S):
                    if px[y][lx] != 0:
                        px[y][lx] = 1

    # Горизонтальные распорки
    for by in [S // 3, 2 * S // 3]:
        bh = 4
        for x in range(S):
            for y in range(by - bh // 2, by + bh // 2):
                if 0 <= y < S:
                    px[y][x] = 6 if y == by - bh // 2 else 7

    return px, "wall_reinforced"


def generate_destroyed(size=16):
    px = _bricks_16(size)
    rng = random.Random(17)
    S = size

    # Трещины random walk
    for _ in range(6):
        cx = rng.randint(0, S - 1)
        cy = rng.randint(0, S - 1)
        length = rng.randint(4, 8)
        for _ in range(length):
            for dx in range(-1, 2):
                for dy in range(-1, 2):
                    x = (cx + dx) % S
                    y = (cy + dy) % S
                    px[y][x] = 6
            cx += rng.randint(-1, 2)
            cy += rng.randint(-1, 2)

    # Сколы
    for _ in range(3):
        cx = rng.randint(0, S - 1)
        cy = rng.randint(0, S - 1)
        r = rng.randint(2, 3)
        for dy in range(-r, r + 1):
            for dx in range(-r, r + 1):
                if dx * dx + dy * dy <= r * r:
                    x = (cx + dx) % S
                    y = (cy + dy) % S
                    if rng.random() < 0.6:
                        px[y][x] = 6

    return px, "wall_destroyed"


def generate_bloody(size=16):
    px = _bricks_16(size)
    rng = random.Random(99)
    S = size

    # Пятна крови
    for _ in range(rng.randint(5, 9)):
        bx = rng.randint(2, S - 3)
        by = rng.randint(2, S - 3)
        r = rng.randint(1, 3)
        for dy in range(-r, r + 1):
            for dx in range(-r, r + 1):
                d = dx * dx + dy * dy
                if d <= r * r and rng.random() < 0.5:
                    x = (bx + dx) % S
                    y = (by + dy) % S
                    if px[y][x] != 0:
                        px[y][x] = 6 if rng.random() < 0.7 else 7

    # Потеки вниз
    for _ in range(6):
        sx = rng.randint(0, S - 1)
        intensity = rng.uniform(0.4, 1.0)
        for dy in range(S):
            if rng.random() > intensity - (dy / S) * 0.5:
                continue
            x = int((sx + math.sin(dy * 0.3) * 2)) % S
            d = rng.randint(0, 1)
            for dx in range(-d, d + 1):
                xx = (x + dx) % S
                if px[dy][xx] != 0:
                    px[dy][xx] = 6 if rng.random() < 0.8 else 8

    return px, "wall_bloody"


def generate_metal(size=16):
    S = size
    px = [[1] * S for _ in range(S)]  # gaps

    pw, ph = 6, 5
    gap = 1
    st_x = pw + gap
    st_y = ph + gap

    for row in range(0, S + st_y, st_y):
        for col in range(0, S + st_x, st_x):
            for y in range(row, min(row + ph, S)):
                for x in range(col, min(col + pw, S)):
                    # bevel
                    is_top = y == row
                    is_left = x == col
                    is_bot = y == row + ph - 1
                    is_right = x == col + pw - 1

                    if is_top or is_left:
                        px[y][x] = 5  # highlight
                    elif is_bot or is_right:
                        px[y][x] = 2  # shadow edge
                    else:
                        px[y][x] = 3  # panel body

            # rivets
            for rx, ry in [
                (col + 1, row + 1),
                (col + pw - 2, row + 1),
                (col + 1, row + ph - 2),
                (col + pw - 2, row + ph - 2),
            ]:
                if 0 <= rx < S and 0 <= ry < S:
                    for dy in range(-1, 2):
                        for dx in range(-1, 2):
                            xx = rx + dx
                            yy = ry + dy
                            if 0 <= xx < S and 0 <= yy < S:
                                if dx == 0 and dy == 0:
                                    px[yy][xx] = 6  # bright rivet center
                                elif abs(dx) + abs(dy) <= 1:
                                    px[yy][xx] = 7  # rivet dark

    return px, "wall_metal"


# ================================================================
# ЗАПУСК
# ================================================================

if __name__ == "__main__":
    out_dir = "/home/bober/Desktop/mywork/Hitman-Blood-Monkey/assets"
    os.makedirs(out_dir, exist_ok=True)

    S = 16

    print("Генерация стен (16x16, source rect в рендерере {0,0,16,16})...")

    tasks = [
        ("wall_stone", generate_stone),
        ("wall_reinforced", generate_reinforced),
        ("wall_destroyed", generate_destroyed),
        ("wall_bloody", generate_bloody),
        ("wall_metal_proc", generate_metal),
    ]

    for name, func in tasks:
        print(f"  {name}.png ...", end=" ")
        px, pal = func(size=S)
        save_palette_png(f"{name}.png", px, pal, out_dir)

    # tile_0040 (как был раньше)
    print("  tile_0040.png ...", end=" ")
    save_palette_png("tile_0040.png", px, pal, out_dir)

    print("\nГотово!")
