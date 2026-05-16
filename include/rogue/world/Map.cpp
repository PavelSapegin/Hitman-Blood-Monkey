#include "Map.h"
#include "../Renderer.h"
#include <cmath>
#include <stdexcept>

namespace rogue {
Map::Map(int width, int height) : width(width), height(height) {
  // Initialize the map with default tiles
  tiles.resize(height, std::vector<Tile>(width, {' ', 1}));

  // Walls (color 4 = COLOR_WALL)
  for (int x = 0; x < width; ++x) {
    tiles[0][x] = {'#', 4};
    tiles[height - 1][x] = {'#', 4};
  }
  for (int y = 0; y < height; ++y) {
    tiles[y][0] = {'#', 4};
    tiles[y][width - 1] = {'#', 4};
  }
};

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
