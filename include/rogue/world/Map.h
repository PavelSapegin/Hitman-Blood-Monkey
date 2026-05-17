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

struct Room {
  int x, y, w, h;
  int centerX() const { return x + w / 2; }
  int centerY() const { return y + h / 2; }
  bool intersects(const Room &other) const {
    return x <= other.x + other.w + 1 && x + w + 1 >= other.x &&
           y <= other.y + other.h + 1 && y + h + 1 >= other.y;
  }
};

class Map {
public:
  Map(int width, int height);
  Map(int width, int height, int seed); // generation for test
  ~Map() = default;

  bool isWalkable(float x, float y) const;
  void spillBlood(float x, float y);
  int getWidth() const;
  int getHeight() const;

  const Tile &getTile(int x, int y) const;
  void render(IRenderer &renderer) const;

  const std::vector<Room> &getRooms() const { return rooms; }

private:
  int width;
  int height;
  std::vector<std::vector<Tile>> tiles;
  std::vector<Room> rooms;
  void generate(unsigned int seed);
  void fillWithWalls();
  void carveRoom(const Room &room);
  void carveHCorridor(int x1, int x2, int y);
  void carveVCorridor(int y1, int y2, int x);
  void setFloor(int x, int y);
};

} // namespace rogue
