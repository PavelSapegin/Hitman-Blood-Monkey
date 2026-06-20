#pragma once
#include <raylib.h>

#include <string>
#include <vector>

namespace rogue {

struct Artifact {
  std::string name;
  std::string desc;

  // Bonuses
  int bonusDamage = 0;
  float bonusSpeed = 0.0f;
  int bonusMaxHp = 0;
  float bonusFireRate = 0.0f;
  int bonusAmmoMG = 0;
  int bonusAmmoSG = 0;
  int bonusAmmoRL = 0;
  bool extraJump = false;
  bool ricochet = false;
  bool explosiveBullets = false;
  bool piercingBullets = false;

  // Drawbacks
  int drainHpPerSec = 0;
  float speedPenalty = 0.0f;
  int maxHpPenalty = 0;
  float fireRatePenalty = 0.0f;
  bool randomTeleport = false;
  bool attractMonsters = false;
  bool noAmmoDrops = false;
  bool reverseControls = false;
  bool glassCannon = false;  // 2x damage TAKEN

  Color color = WHITE;
  char symbol = '?';
};

struct Mutation {
  std::string name;
  std::string desc;

  enum Type {
    ExtraArm,   // +1 bullet per shot
    LaserEyes,  // auto-beam at nearest enemy
    WallJump,   // can walk through walls briefly
    ThornSkin,  // damage melee attackers
    Vampire,    // heal on kill
    Berserker,  // more damage at low HP
    Shield,     // block every 3rd hit
    Speedster,  // +50% speed, -30% HP
    FatRoll,    // no dash cooldown, but slower
    GlassGun    // +200% damage, -50% max HP
  } type;

  Color color = MAGENTA;
};

struct GodBlessing {
  std::string name;
  std::string desc;

  enum Type {
    GodOfSpeed,   // +40% speed, dash has no cooldown
    GodOfFire,    // all bullets explode, +50% fire rate
    GodOfChaos,   // random events happen 2x more, +100% damage
    GodOfHealth,  // regen 1 HP/sec, +100 max HP
    GodOfGreed,   // double pickups, but enemies drop less
    GodOfLuck     // crit chance, better loot
  } type;

  Color color;
};

struct CombinationRecipe {
  std::string resultName;
  std::string resultDesc;
  int requiredId1;  // indices in artifact pool
  int requiredId2;
  Artifact (*createResult)(const Artifact &a, const Artifact &b);
};

class ArtifactSystem {
public:
  ArtifactSystem();

  static Artifact randomArtifact();
  static Mutation randomMutation();
  static void applyArtifact(Artifact &a, class Player &player, int &ammoMG, int &ammoSG,
                            int &ammoRL, const int &maxAmmoMG, const int &maxAmmoSG,
                            const int &maxAmmoRL);
  static void unapplyArtifact(Artifact &a, class Player &player, int &ammoMG, int &ammoSG,
                              int &ammoRL, const int &maxAmmoMG, const int &maxAmmoSG,
                              const int &maxAmmoRL);
  static Artifact tryCombine(const Artifact &a, const Artifact &b);

  static GodBlessing randomGod();

  static std::vector<Artifact> allArtifacts;
  static std::vector<Mutation> allMutations;
  static std::vector<GodBlessing> allGods;
  static std::vector<CombinationRecipe> recipes;
};

}  // namespace rogue
