#pragma once
#include "../world/Map.h"
#include "Entity.h"
#include <utility>
#include <vector>

namespace rogue {

class Monster : public Entity {
public:
  Monster(float x, float y, char symbol, int color, int maxHp);
  void update() override; // Implement monster-specific behavior
  void updateAI(const Map &map, float playerX, float playerY, float deltaTime);

  static constexpr float ATTACK_RANGE = 1.2f;
  static constexpr float ATTACK_COOLDOWN = 1.0f;
  int getAttackDamage() const { return attackDamage; }
  float getAttackCooldown() const { return attackCooldown; }
  void tickCooldown(float dt) {
    if (attackCooldown > 0)
      attackCooldown -= dt;
  }
  void resetCooldown() { attackCooldown = ATTACK_COOLDOWN; }

private:
  float attackCooldown = 0.0f;
  int attackDamage = 10;
  float aggroRange = 10.0f;
  struct Node {
    int x, y;
    float g, h;
    int parentX, parentY;
    float f() const { return g + h; }
  };

  std::vector<std::pair<int, int>> currentPath;
  float pathUpdateTimer = 0.0f;
  static constexpr float PATH_UPDATE_INTERVAL = 0.5f;

  std::vector<std::pair<int, int>> findPath(const Map &map, int startX,
                                            int start, int goalX, int goalY);
};

} // namespace rogue
