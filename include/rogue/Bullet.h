#pragma once

namespace rogue {

struct Bullet {
  float x, y;
  float vx, vy;
  float lifetime = 1.0f;
  float damage = 25.0f;
  bool dead = false;
  bool isRocket = false;
  int pierceLeft = 0;
  int bounceLeft = 0;
};
}  // namespace rogue
