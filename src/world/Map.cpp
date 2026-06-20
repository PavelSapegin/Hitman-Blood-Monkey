#include "rogue/world/Map.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <stdexcept>

#include "rogue/Renderer.h"

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
    add({"######", "#....#", "#....#", "#....#", "######"});

    // 2 - empty medium
    add({"########", "#......#", "#......#", "#......#", "#......#", "########"});

    // 3 - empty large
    add({"###########", "#.........#", "#.........#", "#.........#", "#.........#", "#.........#",
         "###########"});

    // 4 - empty wide
    add({"#############", "#...........#", "#...........#", "#...........#", "#############"});

    // 5 - empty tall
    add({"#####", "#...#", "#...#", "#...#", "#...#", "#...#", "#...#", "#...#", "#####"});

    // 6 - four pillars
    add(
      {"#########", "#.......#", "#.#...#.#", "#.......#", "#.#...#.#", "#.......#", "#########"});

    // 7 - four pillars large
    add({"###########", "#.........#", "#.#.....#.#", "#.........#", "#.#.....#.#", "#.........#",
         "###########"});

    // 8 - center block
    add({"#######", "#.....#", "#.....#", "#..##.#", "#.....#", "#.....#", "#######"});

    // 9 - pillar grid 3x2
    add({"###########", "#.........#", "#.#..#..#.#", "#.........#", "#.#..#..#.#", "#.........#",
         "#.#..#..#.#", "###########"});

    // 10 - split middle
    add({"##########", "#........#", "#........#", "###....###", "#........#", "#........#",
         "##########"});

    // 11 - split offset
    add({"##########", "#........#", "#........#", "####..####", "#........#", "#........#",
         "##########"});

    // 12 - cross hall
    add({"#########", "#.......#", "#.......#", "###...###", "#.......#", "###...###", "#.......#",
         "#.......#", "#########"});

    // 13 - side alcoves
    add({"############", "#..........#", "#..##..##..#", "#..........#", "#..##..##..#",
         "#..........#", "############"});

    // 14 - top alcoves
    add({"########", "#......#", "##....##", "#......#", "#......#", "#......#", "##....##",
         "#......#", "########"});

    // 15 - throne room
    add({"##########", "#........#", "#........#", "#..####..#", "#.#....#.#", "#........#",
         "#........#", "#........#", "##########"});

    // 16 - barracks
    add({"###########", "#.........#", "#.##..##..#", "#.........#", "#..##..##.#", "#.........#",
         "#.##..##..#", "###########"});

    // 17 - library
    add({"############", "#..........#", "#.##.##.##.#", "#..........#", "#.##.##.##.#",
         "#..........#", "#.##.##.##.#", "############"});

    // 18 - dining hall
    add({"############", "#..........#", "#....##....#", "#..........#", "#....##....#",
         "#..........#", "#....##....#", "############"});

    // 19 - armory
    add({"##########", "#........#", "#........#", "#.##.##..#", "#........#", "#........#",
         "##########"});

    // 20 - temple
    add({"############", "#..........#", "#.#......#.#", "#.#......#.#", "#..........#",
         "#..........#", "#.#......#.#", "#.#......#.#", "#..........#", "############"});

    // 21 - crypt
    add({"#########", "#.......#", "#.##.##.#", "#.......#", "#.##.##.#", "#.......#", "#.##.##.#",
         "#########"});

    // 22 - garden
    add({"##########", "#........#", "#.##..##.#", "#........#", "#.##..##.#", "#........#",
         "#.##..##.#", "#........#", "##########"});

    // 23 - arena
    add({"##############", "#............#", "#............#", "#............#", "#............#",
         "#............#", "#............#", "#............#", "#............#", "##############"});

    // 24 - mazelet
    add({"#########", "#.......#", "#.#.###.#", "#.#...#.#", "#.###.#.#", "#...#.#.#", "#.###.#.#",
         "#.......#", "#########"});

    // 25 - octagon
    add({"#########", "##.....##", "#.......#", "#.......#", "#.......#", "#.......#", "#.......#",
         "##.....##", "#########"});

    // 26 - pillars four small
    add({"##########", "#........#", "#........#", "#..#..#..#", "#........#", "#........#",
         "##########"});

    // 27 - the vault
    add({"#########", "##.....##", "#.#...#.#", "#..#..#.#", "#...#...#", "#..#..#.#", "#.#...#.#",
         "##.....##", "#########"});

    // 28 - columned hall
    add({"###########", "#.........#", "#..#...#..#", "#.........#", "#..#...#..#", "#.........#",
         "#..#...#..#", "#.........#", "###########"});

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
        if (tx >= room.x && tx < room.x + room.w && ty >= room.y && ty < room.y + room.h &&
            tiles[ty][tx].symbol == '.') {
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
  auto randInt = [&](int lo, int hi) { return std::uniform_int_distribution<int>(lo, hi)(rng); };

  const auto &prefabs = prefabRegistry();
  rooms.clear();

  for (int attempt = 0; attempt < 200 && (int)rooms.size() < MAX_ROOMS; ++attempt) {
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

int Map::getHeight() const {
  return height;
}

int Map::getWidth() const {
  return width;
}

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

}  // namespace rogue
