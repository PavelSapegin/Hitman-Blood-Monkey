#pragma once
#include <raylib.h>

#include <vector>

#include "Constants.h"

namespace rogue {

enum class ParticleType { Blood, Fire, Smoke, ExplosionDebris };

struct Particle {
  float x, y;
  float vx, vy;
  float lifetime;
  float maxLifetime;
  float size;
  float maxSize;
  Color color;
  ParticleType type;
};

class ParticleSystem {
public:
  void spawnBlood(float x, float y, int count = 20);
  void spawnExplosion(float x, float y, int count = 30);
  void spawnFire(float x, float y, int count = 10);
  void spawnSmoke(float x, float y, int count = 8);
  void spawnMuzzleFlash(float x, float y);
  void update(float deltaTime);
  void render() const;
  bool isEmpty() const { return particles.empty(); }
  void clear() { particles.clear(); }

private:
  std::vector<Particle> particles;
};

}  // namespace rogue
