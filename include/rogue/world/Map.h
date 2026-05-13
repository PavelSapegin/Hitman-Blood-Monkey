#pragma once
#include "../Renderer.h"
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

class Map {
public:
  Map(int width, int height);
  ~Map() = default;

  int getWidth() const;
  int getHeight() const;

  bool isWalkable(float x, float y) const;
  void render(IRenderer &renderer) const;
  void spillBlood(float x, float y);

private:
  int width;
  int height;
  std::vector<std::vector<Tile>> tiles; // Example tile representation
  // Need to store actual tile size for coordinate conversions
  float tileSize = 20.0f; // Each tile is 20x20 pixels
};
} // namespace rogue
