#include "Map.h"
#include "../Renderer.h"
#include <cmath>
#include <stdexcept>
#include <random>
#include <algorithm>

namespace rogue {

static constexpr int MAX_ROOMS    = 20;
static constexpr int MIN_ROOM_W   = 8;
static constexpr int MAX_ROOM_W   = 18;
static constexpr int MIN_ROOM_H   = 6;
static constexpr int MAX_ROOM_H   = 14;

Map::Map(int width, int height) : width(width), height(height) {
  // Initialize the map with default tiles
  tiles.resize(height, std::vector<Tile>(width, {'#', COLOR_WALL}));
  std::random_device rd;
  generate(rd());
}
Map::Map(int width, int height, int seed)
    : width(width), height(height) {
    tiles.resize(height, std::vector<Tile>(width, {'#', COLOR_WALL}));
    generate(seed);
}

void Map::fillWithWalls() {
    for (auto& row : tiles)
        for (auto& tile : row)
            tile = {'#', COLOR_WALL};
}

void Map::setFloor(int x, int y) {
    if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
        tiles[y][x] = {'.', COLOR_DEFAULT};
}

void Map::carveRoom(const Room& room) {
    for (int y = room.y; y < room.y + room.h; ++y)
        for (int x = room.x; x < room.x + room.w; ++x)
            setFloor(x, y);
}
void Map::carveHCorridor(int x1, int x2, int y) {
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
        setFloor(x, y);
    // Коридор шириной 2 для удобства месива
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

    rooms.clear();

    for (int attempt = 0; attempt < 200 && (int)rooms.size() < MAX_ROOMS; ++attempt) {
        Room room;
        room.w = randInt(MIN_ROOM_W, MAX_ROOM_W);
        room.h = randInt(MIN_ROOM_H, MAX_ROOM_H);
        room.x = randInt(1, width  - room.w - 1);
        room.y = randInt(1, height - room.h - 1);

        bool overlaps = false;
        for (const auto& r : rooms)
            if (room.intersects(r)) { overlaps = true; break; }

        if (!overlaps) {
            carveRoom(room);

            if (!rooms.empty()) {
                const Room& prev = rooms.back();
                // Случайно выбираем порядок коридоров
                if (randInt(0, 1) == 0) {
                    carveHCorridor(prev.centerX(), room.centerX(), prev.centerY());
                    carveVCorridor(prev.centerY(), room.centerY(), room.centerX());
                } else {
                    carveVCorridor(prev.centerY(), room.centerY(), prev.centerX());
                    carveHCorridor(prev.centerX(), room.centerX(), room.centerY());
                }
            }
            rooms.push_back(room);
        }
    }
}

bool Map::isWalkable(float x, float y) const {
  // Coordinates are in cell space (float for smooth movement)
  int gridX = static_cast<int>(std::floor(x));
  int gridY = static_cast<int>(std::floor(y));
  if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height) {
    return false; // Out of bounds
  }
  return tiles[gridY][gridX].symbol != '#'; // Walkable if not a wall
}

void Map::spillBlood(float x, float y) {
  int gridX = static_cast<int>(x);
  int gridY = static_cast<int>(y);
  if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height) {
    return; // Out of bounds
  }
  tiles[gridY][gridX] = {'%', 5}; // Blood tile (color 5 = COLOR_BLOOD)
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
