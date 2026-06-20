#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../Artifact.h"
#include "../AudioSystem.h"
#include "../BulletSystem.h"
#include "../EventSystem.h"
#include "../MetaProgression.h"
#include "../ParticleSystem.h"
#include "../Renderer.h"
#include "../Weapon.h"
#include "../entities/Monster.h"
#include "../entities/Player.h"
#include "../world/Map.h"

namespace rogue {

struct FloatingText {
  float x, y;
  std::string text;
  Color color;
  float lifetime;
  float maxLifetime;
};

struct Pickup {
  float x, y;
  enum Type { Health, Ammo, RocketAmmo, ArtifactItem } type;
  bool active = true;
  float lifetime = 20.0f;
  Artifact artifact;
};

struct EnemyProjectile {
  float x, y, vx, vy;
  float damage;
  float lifetime = 2.0f;
  bool dead = false;
};

class Engine {
public:
  Engine(std::unique_ptr<IRenderer> renderer);
  ~Engine();
  void run();

private:
  std::unique_ptr<IRenderer> renderer;
  void handleInput();
  void render();
  void renderHUD();
  void renderMinimap();
  void renderDebugInfo();
  void reset();
  void spawnEntities();
  void startNextWave();
  void spawnPickup(float x, float y);
  void addFloatingText(float x, float y, const std::string &text, Color color);
  void explosionDamage(float x, float y, float radius, float damage, Entity *source);
  void addXPForKill();
  void applyMutation(Mutation &m);
  void applyGodBlessing(GodBlessing &g);
  void handleDynamicEvents();
  void updateHunter(float dt);
  void spawnArtifactPickup(float x, float y);
  void checkArtifactCombinations();
  void breakWall(int x, int y);
  void syncBulletMods();
  float getDamageMultiplier();

  BulletSystem bulletSystem;
  AudioSystem audio;
  Weapon currentWeapon = Weapon::makeFists();
  float weaponCooldownTimer = 0.0f;

  bool isRunning;
  bool showDebug = false;
  Map map;
  Player player;
  std::vector<std::unique_ptr<Monster>> monsters;
  float deltaTime;

  ParticleSystem particles;
  float attackCooldown = 0.0f;
  static constexpr float ATTACK_RANGE = 1.5f;
  static constexpr float ATTACK_COOLDOWN = 0.3f;

  // Time stop (Za Warudo)
  bool timeStopped = false;
  float timeStopTimer = 0.0f;
  float timeStopTextTimer = 0.0f;
  float timeStopCooldownTimer = 0.0f;
  static constexpr float TIME_STOP_DURATION = 3.0f;
  static constexpr float TIME_STOP_TEXT_DURATION = 1.2f;
  static constexpr float TIME_STOP_COOLDOWN = 10.0f;
  static constexpr float TIME_STOP_TRANSITION = 0.3f;

  // HUD / stats
  int killCount = 0;
  int xp = 0;
  int level = 1;
  int xpToNextLevel = 5;
  float damageOverlayTimer = 0.0f;
  float levelUpFlashTimer = 0.0f;
  static constexpr float DAMAGE_OVERLAY_DURATION = 0.4f;
  static constexpr float LEVEL_UP_FLASH_DURATION = 0.8f;

  // Ammo
  int ammoMG = 100;
  int ammoSG = 20;
  int ammoRL = 5;
  static constexpr int MAX_AMMO_MG = 100;
  static constexpr int MAX_AMMO_SG = 20;
  static constexpr int MAX_AMMO_RL = 8;

  // Floating damage numbers
  std::vector<FloatingText> floatingTexts;
  // Pickups
  std::vector<Pickup> pickups;
  // Enemy projectiles
  std::vector<EnemyProjectile> enemyBullets;

  // Dash
  bool isDashing = false;
  float dashTimer = 0.0f;
  float dashCooldown = 0.0f;
  float dashDx = 0.0f, dashDy = 0.0f;
  static constexpr float DASH_DURATION = 0.15f;
  static constexpr float DASH_COOLDOWN = 0.8f;
  static constexpr float DASH_SPEED = 25.0f;

  // Wave system
  int waveNumber = 1;
  int floorNumber = 1;
  static constexpr int WAVES_PER_FLOOR = 5;
  float waveTimer = 0.0f;
  float waveAnnounceTimer = 0.0f;
  bool waitingForWave = true;
  static constexpr float WAVE_DELAY = 2.0f;
  static constexpr float WAVE_ANNOUNCE_DURATION = 2.0f;

  // Pause
  bool paused = false;
  float fadeAlpha = 0.0f;

  // Rocket explosion pool (simple)
  struct RocketExplosion {
    float x, y;
    float timer = 0.0f;
    float maxTimer = 0.3f;
    float radius = 0.0f;
  };
  std::vector<RocketExplosion> rocketExplosions;

  // ---- ARTIFACTS / MUTATIONS / GODS ----
  std::vector<Artifact> artifacts;
  std::vector<Mutation> mutations;
  GodBlessing currentGod;
  bool hasGod = false;
  int shieldHits = 0;  // for Shield mutation
  float wallJumpTimer = 0.0f;
  float regenTimer = 0.0f;
  float drainTimer = 0.0f;

  // ---- EVENT SYSTEM ----
  EventSystem eventSystem;
  float meteorTimer = 0.0f;
  float invasionTimer = 0.0f;

  // ---- HUNTER ----
  std::unique_ptr<Monster> hunter;
  float hunterSpawnTimer = 0.0f;
  float hunterSpawnInterval = 30.0f;
  bool hunterActive = false;

  // ---- MIMICS ----
  struct Mimic {
    int x, y;
    bool active = true;
    bool revealed = false;
    float hp = 30.0f;
  };
  std::vector<Mimic> mimics;

  // ---- META PROGRESSION ----
  MetaProgression meta;

  // ---- critical fail system ----
  float critFailTimer = 0.0f;
  bool controlsReversed = false;

  // ---- mimics ----
  static constexpr float MIMIC_REVEAL_RANGE = 2.0f;
  int mimicsSpawnedThisWave = 0;
  float mimicAttackTimer = 0.0f;

  // ---- zombie revive ----
  bool zombieHordeActive = false;

  // ---- laser eyes ----
  float laserEyesTimer = 0.0f;

  // ---- unstable build ----
  float unstableBuildTimer = 0.0f;

  // ---- weapon mod syncing ----
  bool hasRicochet = false;
  bool hasPiercing = false;
  bool hasExplosive = false;

  // ---- god-specific effects ----
  bool godGreedDoubleDrops = false;
  float godLuckCritChance = 0.0f;
  float godSpeedBonus = 0.0f;

  // ---- wave reward (choice between waves) ----
  bool showingWaveReward = false;
  float waveRewardTimer = 0.0f;
  int waveRewardChoices[3] = {0, 1, 2};
  std::string waveRewardTexts[3];

  // ---- new artifact effects ----
  float frozenHeartTimer = 0.0f;
  float mushroomTimer = 0.0f;
  bool mushroomShrink = false;
  bool eyeballReveal = false;

  // ---- TimeWarp slowdown ----
  float timeWarpSlow = 1.0f;

  // ---- Earthquake wall destruction ----
  float earthquakeWallTimer = 0.0f;

  // ---- Charger monster tracking ----
  float chargerRushTimer = 0.0f;
  float chargerRushDx = 0.0f, chargerRushDy = 0.0f;

  // ---- Splitter helper ----
  void spawnSplitter(float x, float y, int splitCount, int maxHp);
  void applyWaveReward(int choice);
};

}  // namespace rogue
