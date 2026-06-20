#include <algorithm>
#include <cstdlib>

#include "rogue/Artifact.h"
#include "rogue/entities/Player.h"

namespace rogue {

std::vector<Artifact> ArtifactSystem::allArtifacts;
std::vector<Mutation> ArtifactSystem::allMutations;
std::vector<GodBlessing> ArtifactSystem::allGods;
std::vector<CombinationRecipe> ArtifactSystem::recipes;

ArtifactSystem::ArtifactSystem() {
  if (allArtifacts.empty()) {
    allArtifacts = {
      {"Demon Eye", "+100% dmg, HP drains 2/sec",
       0,           0,
       0,           0,
       0,           0,
       0,           false,
       false,       false,
       false,       2,
       0,           0,
       0,           false,
       false,       false,
       false,       false,
       RED,         'D'},
      {"Angel Wings",
       "+1 jump, -30% speed",
       0,
       -0.3f,
       0,
       0,
       0,
       0,
       0,
       true,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       WHITE,
       'W'},
      {"Berserker Axe",
       "+50% dmg, +50% dmg taken",
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       true,
       ORANGE,
       'A'},
      {"Ricochet Lens",
       "bullets bounce off walls",
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       false,
       true,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       SKYBLUE,
       'R'},
      {"Cursed Crown",
       "monsters attracted to you, +30% dmg",
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       true,
       false,
       false,
       false,
       GOLD,
       'C'},
      {"Seven-League Boots",
       "+50% speed, reverse controls",
       0,
       0.5f,
       0,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       true,
       false,
       false,
       GREEN,
       'B'},
      {"Explosive Rounds",
       "bullets explode on hit, -40% fire rate",
       0,
       0,
       0,
       -0.4f,
       0,
       0,
       0,
       false,
       false,
       true,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       RED,
       'E'},
      {"Piercing Shot",
       "bullets pierce enemies, no ammo drops",
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       true,
       0,
       0,
       0,
       0,
       false,
       false,
       true,
       false,
       false,
       PURPLE,
       'P'},
      {"Vampire Fang",
       "heal 2 HP on kill, -20% max HP",
       0,
       0,
       -20,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       MAROON,
       'V'},
      {"Chaos Orb", "random teleport on damage, +100% dmg",
       0,           0,
       0,           0,
       0,           0,
       0,           false,
       false,       false,
       false,       0,
       0,           0,
       0,           true,
       false,       false,
       false,       false,
       PINK,        'O'},
      {"Unstable build v0.1",
       "random effect every 5s, +50% dmg",
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       Color{255, 100, 0, 255},
       'U'},
      {"Nuke", "all enemies take 50 dmg on pickup",
       0,      0,
       0,      0,
       0,      0,
       0,      false,
       false,  false,
       false,  0,
       0,      0,
       0,      false,
       false,  false,
       false,  false,
       RED,    'N'},
      {"Golden Toilet",
       "+100 max HP, -30% speed, enemies ignore you",
       0,
       -0.3f,
       100,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       GOLD,
       'T'},
      {"Eyeball",
       "reveals whole map, -20% dmg",
       -20,
       0,
       0,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       Color{200, 200, 255, 255},
       'E'},
      {"Frozen Heart",
       "every 3rd shot freezes enemy, +20% fire rate",
       0,
       0,
       0,
       0.2f,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       SKYBLUE,
       'F'},
      {"Mushroom",
       "random buff every 10s, might shrink you",
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       0,
       0,
       0,
       0,
       false,
       false,
       false,
       false,
       false,
       Color{200, 100, 255, 255},
       'M'},
    };
  }
  if (allMutations.empty()) {
    allMutations = {
      {"Extra Arm", "+1 bullet per shot", Mutation::ExtraArm},
      {"Laser Eyes", "auto-beam at nearest enemy", Mutation::LaserEyes},
      {"Wall Jump", "walk through walls 1s", Mutation::WallJump},
      {"Thorn Skin", "damage melee attackers", Mutation::ThornSkin},
      {"Vampire", "heal 3 HP on kill", Mutation::Vampire},
      {"Berserker", "+100% dmg when HP < 30%", Mutation::Berserker},
      {"Shield", "block every 3rd hit", Mutation::Shield},
      {"Speedster", "+50% speed, -30% HP", Mutation::Speedster},
      {"Fat Roll", "dash has no cd, -30% speed", Mutation::FatRoll},
      {"Glass Gun", "+200% dmg, -50% max HP", Mutation::GlassGun},
    };
  }
  if (allGods.empty()) {
    allGods = {
      {"God of Speed", "+40% speed, infinite dashes", GodBlessing::GodOfSpeed, GREEN},
      {"God of Fire", "all bullets explode, +50% fire rate", GodBlessing::GodOfFire, RED},
      {"God of Chaos", "2x events, +100% dmg", GodBlessing::GodOfChaos, PURPLE},
      {"God of Health", "regen 1 HP/s, +100 max HP", GodBlessing::GodOfHealth, LIME},
      {"God of Greed", "double pickups, fewer enemy drops", GodBlessing::GodOfGreed, GOLD},
      {"God of Luck", "crit chance, better loot", GodBlessing::GodOfLuck, SKYBLUE},
    };
  }
}

Artifact ArtifactSystem::randomArtifact() {
  if (allArtifacts.empty())
    return {"Placebo", "does nothing"};
  return allArtifacts[rand() % allArtifacts.size()];
}

Mutation ArtifactSystem::randomMutation() {
  if (allMutations.empty())
    return {"Placebo", "nothing happens", Mutation::ExtraArm};
  return allMutations[rand() % allMutations.size()];
}

GodBlessing ArtifactSystem::randomGod() {
  if (allGods.empty())
    return {"Atheist", "nothing", GodBlessing::GodOfSpeed, GRAY};
  return allGods[rand() % allGods.size()];
}

void ArtifactSystem::applyArtifact(Artifact &a, Player &player, int &ammoMG, int &ammoSG,
                                   int &ammoRL, const int & /*maxAmmoMG*/,
                                   const int & /*maxAmmoSG*/, const int & /*maxAmmoRL*/) {
  if (a.bonusMaxHp != 0) {
    if (a.bonusMaxHp > 0)
      player.takeDamage(-a.bonusMaxHp);
    else
      for (int i = 0; i < -a.bonusMaxHp; ++i)
        player.takeDamage(1);
  }
  ammoMG += a.bonusAmmoMG;
  ammoSG += a.bonusAmmoSG;
  ammoRL += a.bonusAmmoRL;
}

void ArtifactSystem::unapplyArtifact(Artifact &a, Player &player, int &ammoMG, int &ammoSG,
                                     int &ammoRL, const int & /*maxAmmoMG*/,
                                     const int & /*maxAmmoSG*/, const int & /*maxAmmoRL*/) {
  if (a.bonusMaxHp != 0) {
    for (int i = 0; i < a.bonusMaxHp; ++i)
      player.takeDamage(1);
  }
  ammoMG -= a.bonusAmmoMG;
  ammoSG -= a.bonusAmmoSG;
  ammoRL -= a.bonusAmmoRL;
}

Artifact ArtifactSystem::tryCombine(const Artifact &a, const Artifact &b) {
  // Simple combinations
  if ((a.name == "Explosive Rounds" && b.name == "Ricochet Lens") ||
      (a.name == "Ricochet Lens" && b.name == "Explosive Rounds")) {
    Artifact r;
    r.name = "Bouncing Bombs";
    r.desc = "bullets explode and bounce! +50% dmg";
    r.bonusDamage = 50;
    r.explosiveBullets = true;
    r.ricochet = true;
    r.color = ORANGE;
    r.symbol = 'B';
    return r;
  }
  if ((a.name == "Demon Eye" && b.name == "Angel Wings") ||
      (a.name == "Angel Wings" && b.name == "Demon Eye")) {
    Artifact r;
    r.name = "Fallen Angel";
    r.desc = "+2 jumps, +100% dmg, HP drains 1/sec";
    r.bonusDamage = 100;
    r.extraJump = true;
    r.drainHpPerSec = 1;
    r.color = Color{128, 0, 128, 255};
    r.symbol = 'F';
    return r;
  }
  if ((a.name == "Seven-League Boots" && b.name == "Berserker Axe") ||
      (a.name == "Berserker Axe" && b.name == "Seven-League Boots")) {
    Artifact r;
    r.name = "Berserker Rush";
    r.desc = "+100% speed, +100% dmg taken, +50% dmg";
    r.bonusSpeed = 1.0f;
    r.bonusDamage = 50;
    r.glassCannon = true;
    r.color = RED;
    r.symbol = 'R';
    return r;
  }
  // No valid combo
  Artifact none;
  none.name = "";
  return none;
}

}  // namespace rogue
