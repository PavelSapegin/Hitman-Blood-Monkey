#pragma once
#include <utility>
#include <vector>

#include "../world/Map.h"
#include "Entity.h"

namespace rogue {

class Monster : public Entity {
public:
  Monster(float x, float y, char symbol, int color, int maxHp);
  void update() override;  // Implement monster-specific behavior
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
  void onHit() { hitFlashTimer = HIT_FLASH_DURATION; }
  bool isFlashing() const override { return hitFlashTimer > 0.0f; }
  void tickFlash(float dt) {
    if (hitFlashTimer > 0.0f)
      hitFlashTimer -= dt;
  }

  // Ranged attack
  void setRanged(bool val) { isRanged = val; }
  void setRangedDamage(int dmg) { rangedDamage = dmg; }
  void setAggroRange(float r) { aggroRange = r; }
  void setAttackDamage(int dmg) { attackDamage = dmg; }
  bool isRanged = false;
  float rangedCooldown = 0.0f;
  float rangedRange = 8.0f;
  static constexpr float RANGED_COOLDOWN = 1.5f;
  bool canFireRanged() const { return isRanged && rangedCooldown <= 0.0f; }
  void resetRangedCooldown() {
    if (isRanged)
      rangedCooldown = RANGED_COOLDOWN;
  }
  void tickRangedCooldown(float dt) {
    if (rangedCooldown > 0)
      rangedCooldown -= dt;
  }

  // Kamikaze
  void setKamikaze(bool val) { isKamikaze = val; }
  bool isKamikaze = false;

  // Zombie revive (ZombieHorde event)
  int zombieRevivesLeft = 0;

  // New monster types
  bool isHealer = false;
  bool isSummoner = false;
  bool isCharger = false;
  bool isSplitter = false;
  float healTimer = 0.0f;
  float summonTimer = 0.0f;
  int splitCount = 0;  // how many splits left
  // Store original stats for splitter
  char origSymbol = 's';
  int origColor = 3;
  int origMaxHp = 30;

private:
  float attackCooldown = 0.0f;
  int attackDamage = 10;
  float aggroRange = 10.0f;
  int rangedDamage = 8;
  float hitFlashTimer = 0.0f;
  static constexpr float HIT_FLASH_DURATION = 0.1f;
  struct Node {
    int x, y;
    float g, h;
    int parentX, parentY;
    float f() const { return g + h; }
  };

  std::vector<std::pair<int, int>> currentPath;
  float pathUpdateTimer = 0.0f;
  static constexpr float PATH_UPDATE_INTERVAL = 0.5f;

  std::vector<std::pair<int, int>> findPath(const Map &map, int startX, int startY, int goalX,
                                            int goalY);
};

}  // namespace rogue
