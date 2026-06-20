#pragma once
#include "../BulletSystem.h"
#include "../Constants.h"
#include "../ParticleSystem.h"
#include "../Renderer.h"
#include "../Weapon.h"
#include "../entities/Monster.h"
#include "../entities/Player.h"
#include "../world/Map.h"
#include <memory>
#include <vector>

namespace rogue {

class Engine {
public:
  Engine(std::unique_ptr<IRenderer> renderer);
  ~Engine();

  void run(); // Main game loop

private:
  std::unique_ptr<IRenderer> renderer;
  void handleInput();
  void render();
  void renderDebugInfo();
  void reset();
  void spawnEntities();
  BulletSystem bulletSystem;
  Weapon currentWeapon = Weapon::makeFists();
  float weaponCooldownTimer = 0.0f;

  bool isRunning;
  Map map;
  Player player;
  std::vector<std::unique_ptr<Monster>> monsters;
  float playerSpeed = 60.0f; // Units per second
  float deltaTime;

  ParticleSystem particles;
  float attackCooldown = 0.0f;
  static constexpr float ATTACK_RANGE = 1.5f;
  static constexpr float ATTACK_COOLDOWN = 0.3f;

  // Time stop (Za Warudo)
  bool timeStopped = false;
  float timeStopTimer = 0.0f;
  float timeStopCooldownTimer = 0.0f;
  float timeStopEffectTimer = 0.0f;
  static constexpr float TIME_STOP_DURATION = 3.0f;
  static constexpr float TIME_STOP_COOLDOWN = 10.0f;
  static constexpr float TIME_STOP_EFFECT_DURATION = 1.2f;
};

} // namespace rogue
