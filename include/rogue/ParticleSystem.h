#pragma once
#include <raylib.h>
#include <vector>

namespace rogue {

struct Particle {
  float x, y;
  float vx, vy;   // speed in x and y directions
  float lifetime; // remaining lifetime in seconds
  float maxLifetime;
  char symbol; // '%', '*', 'x', 'o'
  Color color;
};

class ParticleSystem {
public:
  void spawnBlood(float x, float y, int count = 20);
  void update(float deltaTime);
  void render() const;
  bool isEmpty() const { return particles.empty(); }

private:
  std::vector<Particle> particles;
};

} // namespace rogue
