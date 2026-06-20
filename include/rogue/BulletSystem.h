#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "Bullet.h"
#include "Constants.h"
#include "ParticleSystem.h"
#include "entities/Monster.h"
#include "world/Map.h"

namespace rogue {
class BulletSystem {
public:
  void spawn(float x, float y, float tx, float ty, float damage, bool isRocket = false);
  void update(float dt, Map &map, std::vector<std::unique_ptr<Monster>> &monsters,
              ParticleSystem &particles);
  void render() const;
  void clear();

  // Rocket explosion positions (world coords) from this frame
  std::vector<std::pair<float, float>> getRocketExplosions();
  // Callback on monster kill (world coords)
  std::function<void(float, float)> onMonsterKilled;
  // Callback on explosive hit (x, y, radius, damage)
  std::function<void(float, float, float, float)> onExplosion;

  // Weapon mod flags (set by Engine from artifacts/mutations)
  bool ricochetMod = false;
  bool piercingMod = false;
  bool explosiveMod = false;
  float explosiveRadius = 2.0f;
  float explosiveDamage = 30.0f;

private:
  std::vector<Bullet> bullets;
  std::vector<std::pair<float, float>> rocketExplosions;
  static constexpr float BULLET_SPEED = 20.0f;
};
}  // namespace rogue
