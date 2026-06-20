#pragma once
#include "Bullet.h"
#include "Constants.h"
#include "ParticleSystem.h"
#include "entities/Monster.h"
#include "world/Map.h"
#include <memory>
#include <vector>

namespace rogue {
class BulletSystem {
public:
  void spawn(float x, float y, float tx, float ty, float damage);
  void update(float dt, Map &map,
              std::vector<std::unique_ptr<Monster>> &monsters,
              ParticleSystem &particles);
  void render() const;
  void clear() { bullets.clear(); }

private:
  std::vector<Bullet> bullets;
  static constexpr float BULLET_SPEED = 20.0f;
};
} // namespace rogue
