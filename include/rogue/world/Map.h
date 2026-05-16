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

  bool isWalkable(float x, float y) const;
  void spillBlood(float x, float y);
  int getWidth() const;
  int getHeight() const;

  const Tile &getTile(int x, int y) const;
  void render(IRenderer &renderer) const;

private:
  int width;
  int height;
  std::vector<std::vector<Tile>> tiles;
};

} // namespace rogue
