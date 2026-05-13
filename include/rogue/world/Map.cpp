#include "Map.h"
#include "../Renderer.h"

namespace rogue {
Map::Map(int width, int height) : width(width), height(height) {
  // Initialize the map with default tiles (for simplicity)
  tiles.resize(height, std::vector<Tile>(width, {' ', 1}));

  for (int x = 0; x < width; ++x) {
    tiles[0][x] = {'#', 1};          // Top wall
    tiles[height - 1][x] = {'#', 1}; // Bottom wall
  }
  for (int y = 0; y < height; ++y) {
    tiles[y][0] = {'#', 1};         // Left wall
    tiles[y][width - 1] = {'#', 1}; // Right wall
  }
};

bool Map::isWalkable(float x, float y) const {
  int gridX = static_cast<int>(x / tileSize);
  int gridY = static_cast<int>(y / tileSize);

  if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height) {
    return false; // Out of bounds
  }
  return tiles[gridY][gridX].symbol != '#'; // Walkable if not a wall
}

void Map::spillBlood(float x, float y) {
  int gridX = static_cast<int>(x / tileSize);
  int gridY = static_cast<int>(y / tileSize);
  if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height) {
    return; // Out of bounds
  }
  tiles[gridY][gridX] = {'%', 2}; // Example blood tile
}

int Map::getHeight() const { return height; }

int Map::getWidth() const { return width; }

void Map::render(IRenderer &renderer) const {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const Tile &tile = tiles[y][x];
      renderer.drawChar(x, y, tile.symbol, tile.colorPair);
    }
  }
}
} // namespace rogue
