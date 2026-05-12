#include "Map.h"
#include "../Renderer.h"
#include <ncurses.h>

namespace rogue {
Map::Map(int width, int height) : width(width), height(height) {
  // Initialize the map with default tiles (for simplicity)
  tiles.resize(height, std::vector<Tile>(width, {' ', COLOR_DEFAULT}));

  for (int x = 0; x < width; ++x) {
    tiles[0][x] = {'#', COLOR_WALL};          // Top wall
    tiles[height - 1][x] = {'#', COLOR_WALL}; // Bottom wall
  }
  for (int y = 0; y < height; ++y) {
    tiles[y][0] = {'#', COLOR_WALL};         // Left wall
    tiles[y][width - 1] = {'#', COLOR_WALL}; // Right wall
  }
};

bool Map::isWalkable(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return false; // Out of bounds
  }
  return tiles[y][x].symbol != '#'; // Walkable if not a wall
}

void Map::spillBlood(int x, int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return; // Out of bounds
  }
  tiles[y][x] = {'%', COLOR_BLOOD}; // Example blood tile
}

int Map::getHeight() const { return height; }

int Map::getWidth() const { return width; }

void Map::render(IRenderer &renderer) const {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const Tile &tile = tiles[y][x];
      renderer.drawChar(x, y, tile.symbol, tile.colorPair);
      attroff(COLOR_PAIR(tile.colorPair));
    }
  }
}
} // namespace rogue
