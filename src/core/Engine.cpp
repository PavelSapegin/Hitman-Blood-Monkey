#include "rogue/core/Engine.h"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

#include "rogue/Exceptions.h"
#include "rogue/ParticleSystem.h"
#include "rogue/Raylib_renderer.h"
#include "rogue/Renderer.h"
#include "rogue/TextureManager.h"
#include "rogue/entities/Monster.h"
#include "rogue/entities/MonsterFactory.h"
#include "rogue/entities/Player.h"

namespace rogue {

Engine::Engine(std::unique_ptr<IRenderer> r)
  : isRunning(true), map(120, 80), player(0.0f, 0.0f, '@', COLOR_PLAYER, 100), deltaTime(0.0f) {
  if (!r) {
    throw InitializationException("Renderer cannot be null");
  }
  renderer = std::move(r);
  renderer->initialize();
  auto &tm = TextureManager::getInstance();
  tm.loadTexture("wall", "../assets/tile_0040.png");
  tm.loadTexture("wall_stone", "../assets/wall_stone.png");
  tm.loadTexture("wall_bloody", "../assets/wall_bloody.png");
  tm.loadTexture("wall_reinforced", "../assets/wall_reinforced.png");
  tm.loadTexture("wall_destroyed", "../assets/wall_destroyed.png");
  tm.loadTexture("floor", "../assets/tile_0049.png");
  tm.loadTexture("player", "../assets/tile_0088.png");
  tm.loadTexture("monster1", "../assets/tile_0109.png");
  tm.loadTexture("monster2", "../assets/tile_0111.png");
  tm.loadTexture("boss", "../assets/tile_0108.png");
  tm.loadTexture("monster_sceleton", "../assets/monster_sceleton.png");
  tm.loadTexture("monster_goblin", "../assets/monster_goblin.png");
  tm.loadTexture("monster_boss", "../assets/monster_boss.png");
  tm.loadTexture("monster_gun", "../assets/monster_gun.png");
  tm.loadTexture("monster_kamikaze", "../assets/monster_kamikaze.png");
  tm.loadTexture("monster_hunter", "../assets/monster_hunter.png");
  tm.loadTexture("weapon_minigun", "../assets/weapon_minigun.png");
  tm.loadTexture("weapon_shotgun", "../assets/weapon_shotgun.png");
  tm.loadTexture("weapon_rocket", "../assets/weapon_rocket.png");
  tm.loadTexture("effect_explosion", "../assets/effect_explosion.png");
  tm.loadTexture("effect_muzzle", "../assets/effect_muzzle.png");
  tm.loadTexture("effect_shield", "../assets/effect_shield.png");
  tm.loadTexture("effect_blood", "../assets/effect_blood.png");
  tm.loadTexture("pickup_health", "../assets/pickup_health.png");
  tm.loadTexture("pickup_ammo", "../assets/pickup_ammo.png");
  audio.init();
  ArtifactSystem artifactSys;  // init static pools
  bulletSystem.onMonsterKilled = [this](float mx, float my) {
    audio.playDeath();
    meta.addKill();
    // Vampire mutation: heal on kill
    for (auto &m : mutations) {
      if (m.type == Mutation::Vampire) {
        player.takeDamage(-3);
        addFloatingText(mx, my - 1.0f, "+3 HP (Vampire)", MAROON);
      }
    }
  };
  bulletSystem.onExplosion = [this](float x, float y, float radius, float damage) {
    explosionDamage(x, y, radius, damage, &player);
  };
  // Place player in first room
  const auto &rooms = map.getRooms();
  if (!rooms.empty()) {
    player.setPosition(rooms[0].centerX(), rooms[0].centerY());
  }
  // God selection at game start
  int godChoice = rand() % 6;
  GodBlessing startGod = ArtifactSystem::allGods[godChoice];
  applyGodBlessing(startGod);
  // Hunter timer start
  hunterSpawnTimer = hunterSpawnInterval;
  // Meta tracking
  meta.addRun();
  syncBulletMods();
  startNextWave();
}

Engine::~Engine() {
  TextureManager::getInstance().unloadAll();
  renderer->shutdown();
}

void Engine::spawnEntities() {
  const auto &rooms = map.getRooms();
  if (rooms.empty())
    return;

  player.setPosition(rooms[0].centerX(), rooms[0].centerY());

  std::mt19937 rng(std::random_device{}());
  for (int i = 1; i < (int)rooms.size(); ++i) {
    const auto &room = rooms[i];
    int count = 3 + rng() % 5;
    for (int j = 0; j < count; ++j) {
      float mx = room.x + 1 + rng() % (room.w - 2);
      float my = room.y + 1 + rng() % (room.h - 2);
      MonsterType type = static_cast<MonsterType>(rng() % 4);
      monsters.push_back(MonsterFactory::createMonster(type, mx, my));
    }
  }
}

void Engine::spawnPickup(float x, float y) {
  auto spawnOne = [&]() {
    Pickup p;
    p.x = x;
    p.y = y;
    int r = rand() % 5;
    if (r == 0)
      p.type = Pickup::Ammo;
    else if (r == 1)
      p.type = Pickup::RocketAmmo;
    else
      p.type = Pickup::Health;
    p.lifetime = 20.0f;
    pickups.push_back(p);
  };
  spawnOne();
  if (godGreedDoubleDrops)
    spawnOne();
}

void Engine::addFloatingText(float x, float y, const std::string &text, Color color) {
  FloatingText ft;
  ft.x = x;
  ft.y = y;
  ft.text = text;
  ft.color = color;
  ft.lifetime = 1.2f;
  ft.maxLifetime = 1.2f;
  floatingTexts.push_back(ft);
}

void Engine::startNextWave() {
  monsters.clear();
  const auto &rooms = map.getRooms();
  if (rooms.empty())
    return;

  std::mt19937 rng(std::random_device{}());

  // Boss wave every 10 waves
  if (waveNumber % 10 == 0 && !rooms.empty()) {
    int bi = rng() % rooms.size();
    const auto &broom = rooms[bi];
    float bmx = broom.centerX();
    float bmy = broom.centerY();
    auto boss = MonsterFactory::createMonster(MonsterType::MonkeyBoss, bmx, bmy);
    if (boss) {
      int bossHP = (waveNumber * 25) + floorNumber * 50;
      boss->takeDamage(-bossHP);
      boss->setAttackDamage(15 + waveNumber);
      monsters.push_back(std::move(boss));
    }
    eventSystem.triggerComment("BOSS WAVE! GOOD LUCK!");
  } else {
    // Normal wave spawning
    int monstersPerRoom = 2 + waveNumber / 2;
    for (size_t i = 1; i < rooms.size(); ++i) {
      const auto &room = rooms[i];
      for (int j = 0; j < monstersPerRoom; ++j) {
        float mx = room.x + 1 + rng() % std::max(1, room.w - 2);
        float my = room.y + 1 + rng() % std::max(1, room.h - 2);
        MonsterType type;
        int rt = rng() % 100;
        if (waveNumber >= 8 && rt < 8)
          type = MonsterType::SplitterMonkey;
        else if (waveNumber >= 6 && rt < 18)
          type = MonsterType::ChargerMonkey;
        else if (waveNumber >= 4 && rt < 28)
          type = MonsterType::SummonerMonkey;
        else if (waveNumber >= 3 && rt < 38)
          type = MonsterType::HealerMonkey;
        else if (waveNumber >= 5 && rt < 46)
          type = MonsterType::MonkeyBoss;
        else if (waveNumber >= 3 && rt < 58)
          type = MonsterType::GunMonkey;
        else if (rt < 68)
          type = MonsterType::KamikazeMonkey;
        else if (rt < 82)
          type = MonsterType::GoblinMonkey;
        else
          type = MonsterType::SceletonMonkey;

        auto m = MonsterFactory::createMonster(type, mx, my);
        if (m) {
          int bonusHP = (waveNumber - 1) * 10;
          for (int h = 0; h < bonusHP; ++h)
            m->takeDamage(-1);
          monsters.push_back(std::move(m));
        }
      }
    }
  }

  // Spawn extra monsters in corridors (30-60% of room count)
  int corridorCount = (int)(rooms.size() * (0.3f + rng() % 30 / 100.0f));
  for (int i = 0; i < corridorCount; ++i) {
    float mx = rng() % (map.getWidth() - 2) + 1;
    float my = rng() % (map.getHeight() - 2) + 1;
    if (!map.isWalkable(mx, my))
      continue;
    // Check not too close to player
    float dx = mx - player.getX();
    float dy = my - player.getY();
    if (std::sqrt(dx * dx + dy * dy) < 5.0f)
      continue;

    MonsterType type;
    int rt = rng() % 100;
    if (rt < 18)
      type = MonsterType::KamikazeMonkey;
    else if (rt < 35)
      type = MonsterType::GunMonkey;
    else if (rt < 50)
      type = MonsterType::HealerMonkey;
    else if (rt < 65)
      type = MonsterType::ChargerMonkey;
    else
      type = MonsterType::SceletonMonkey;

    auto m = MonsterFactory::createMonster(type, mx, my);
    if (m) {
      int bonusHP = (waveNumber - 1) * 10;
      for (int h = 0; h < bonusHP; ++h)
        m->takeDamage(-1);
      monsters.push_back(std::move(m));
    }
  }

  // Spawn mimics in some rooms (skip room 0)
  mimics.clear();
  mimicsSpawnedThisWave = 0;
  std::mt19937 mimicRng(std::random_device{}());
  for (size_t i = 1; i < rooms.size(); ++i) {
    if (mimicRng() % 100 < 20) {  // 20% chance per room
      const auto &room = rooms[i];
      Mimic m;
      m.x = room.x + 1 + mimicRng() % std::max(1, room.w - 2);
      m.y = room.y + 1 + mimicRng() % std::max(1, room.h - 2);
      m.active = true;
      m.revealed = false;
      m.hp = 30.0f + waveNumber * 10;
      mimics.push_back(m);
      ++mimicsSpawnedThisWave;
    }
  }
  if (mimicsSpawnedThisWave > 0) {
    eventSystem.maybeComment("I wonder if that chest is safe...", 0.8f);
  }

  waitingForWave = false;
  waveAnnounceTimer = WAVE_ANNOUNCE_DURATION;
  audio.playWaveStart();
}

void Engine::explosionDamage(float x, float y, float radius, float damage, Entity *source) {
  particles.spawnExplosion(x, y, 40);
  audio.playExplosion();
  auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
  if (rr)
    rr->addShake(8.0f);

  // Damage monsters
  for (auto &m : monsters) {
    if (m->isDead() || m.get() == source)
      continue;
    float dx = m->getX() - x;
    float dy = m->getY() - y;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist < radius) {
      float falloff = 1.0f - dist / radius;
      int dmg = (int)(damage * falloff);
      if (dmg < 1)
        dmg = 1;
      m->takeDamage(dmg);
      m->onHit();
      addFloatingText(m->getX(), m->getY() - 0.5f, TextFormat("%d", dmg), ORANGE);
      if (m->isDead()) {
        particles.spawnExplosion(m->getX(), m->getY(), 20);
        particles.spawnSmoke(m->getX(), m->getY(), 10);
        spawnPickup(m->getX(), m->getY());
        ++killCount;
        addFloatingText(m->getX(), m->getY() - 1.0f, "KILL!", RED);
      }
    }
  }

  // Damage player if not source
  if (source != &player) {
    float dx = player.getX() - x;
    float dy = player.getY() - y;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist < radius) {
      float falloff = 1.0f - dist / radius;
      int dmg = (int)(damage * falloff * 0.5f);
      if (dmg > 0) {
        player.takeDamage(dmg);
        damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
        addFloatingText(player.getX(), player.getY() - 0.5f, TextFormat("-%d", dmg), RED);
      }
    }
  }
}

void Engine::addXPForKill() {
  int bonus = eventSystem.getBonusXp();
  xp += 1 + bonus;
  if (xp >= xpToNextLevel) {
    xp = 0;
    ++level;
    xpToNextLevel += 3;
    levelUpFlashTimer = LEVEL_UP_FLASH_DURATION;
    addFloatingText(player.getX(), player.getY() - 1.5f, "LEVEL UP!", GOLD);
    audio.playLevelUp();
    int heal = 10 + level * 5;
    player.takeDamage(-heal);
    // Mutation on level up
    Mutation mut = ArtifactSystem::randomMutation();
    applyMutation(mut);
  }
}

void Engine::applyMutation(Mutation &m) {
  mutations.push_back(m);
  addFloatingText(player.getX(), player.getY() - 2.0f, TextFormat("MUTATION: %s", m.name.c_str()),
                  MAGENTA);

  switch (m.type) {
    case Mutation::ExtraArm:
      break;  // handled in shooting code
    case Mutation::LaserEyes:
      break;  // auto-beam
    case Mutation::WallJump:
      wallJumpTimer = 3.0f;
      break;
    case Mutation::ThornSkin:
      break;  // reflected damage
    case Mutation::Vampire:
      break;  // heal on kill
    case Mutation::Berserker:
      break;  // dmg when low hp
    case Mutation::Shield:
      shieldHits = 0;
      break;
    case Mutation::Speedster:
      player.takeDamage(player.getMaxHp() * 30 / 100);
      break;
    case Mutation::FatRoll:
      break;
    case Mutation::GlassGun:
      for (int i = 0; i < player.getMaxHp() / 2; ++i)
        player.takeDamage(1);
      break;
  }
}

void Engine::applyGodBlessing(GodBlessing &g) {
  hasGod = true;
  currentGod = g;
  addFloatingText(player.getX(), player.getY() - 2.0f, TextFormat("BLESSING: %s", g.name.c_str()),
                  g.color);

  // Reset god-specific fields
  godGreedDoubleDrops = false;
  godLuckCritChance = 0.0f;
  godSpeedBonus = 0.0f;

  switch (g.type) {
    case GodBlessing::GodOfSpeed:
      godSpeedBonus = 0.4f;
      addFloatingText(player.getX(), player.getY() - 0.5f, "+40% SPEED + INFINITE DASH", GREEN);
      break;
    case GodBlessing::GodOfFire:
      syncBulletMods();
      addFloatingText(player.getX(), player.getY() - 0.5f, "EXPLOSIVE BULLETS + FIRE RATE", RED);
      break;
    case GodBlessing::GodOfChaos:
      eventSystem.forceEvent(EventSystem::randomEvent());
      addFloatingText(player.getX(), player.getY() - 0.5f, "EVENTS x2 +100% DMG", PURPLE);
      break;
    case GodBlessing::GodOfHealth:
      for (int i = 0; i < 100; ++i)
        player.takeDamage(-1);
      addFloatingText(player.getX(), player.getY() - 0.5f, "+100 HP + REGEN", LIME);
      break;
    case GodBlessing::GodOfGreed:
      godGreedDoubleDrops = true;
      addFloatingText(player.getX(), player.getY() - 0.5f, "DOUBLE DROPS!", GOLD);
      break;
    case GodBlessing::GodOfLuck:
      godLuckCritChance = 0.25f;
      addFloatingText(player.getX(), player.getY() - 0.5f, "25% CRIT CHANCE!", SKYBLUE);
      break;
  }
}

void Engine::handleDynamicEvents() {
  // Meteor rain
  if (eventSystem.getCurrentEvent() == DynamicEvent::MeteorRain &&
      eventSystem.getEventWarning() <= 0.0f) {
    meteorTimer -= deltaTime;
    if (meteorTimer <= 0.0f) {
      meteorTimer = 0.3f;
      float mx = (float)(rand() % map.getWidth());
      float my = (float)(rand() % map.getHeight());
      if (map.isWalkable(mx, my)) {
        explosionDamage(mx, my, 2.0f, 30.0f, nullptr);
        // Check player hit
        float dx = player.getX() - mx;
        float dy = player.getY() - my;
        if (std::sqrt(dx * dx + dy * dy) < 3.0f) {
          player.takeDamage(15);
          damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
          addFloatingText(player.getX(), player.getY() - 0.5f, "-15", RED);
        }
      }
    }
  }

  // Monster invasion
  if (eventSystem.getCurrentEvent() == DynamicEvent::MonsterInvasion &&
      eventSystem.getEventWarning() <= 0.0f) {
    invasionTimer -= deltaTime;
    if (invasionTimer <= 0.0f) {
      invasionTimer = 0.5f;
      const auto &rooms = map.getRooms();
      if (!rooms.empty()) {
        int ri = rand() % rooms.size();
        const auto &room = rooms[ri];
        float mx = room.x + 1 + rand() % std::max(1, room.w - 2);
        float my = room.y + 1 + rand() % std::max(1, room.h - 2);
        int rt = rand() % 100;
        MonsterType type;
        if (rt < 40)
          type = MonsterType::SceletonMonkey;
        else if (rt < 70)
          type = MonsterType::GunMonkey;
        else if (rt < 90)
          type = MonsterType::KamikazeMonkey;
        else
          type = MonsterType::GoblinMonkey;
        auto m = MonsterFactory::createMonster(type, mx, my);
        if (m) {
          for (int h = 0; h < (waveNumber - 1) * 10; ++h)
            m->takeDamage(-1);
          if (zombieHordeActive)
            m->zombieRevivesLeft = 1;
          monsters.push_back(std::move(m));
        }
      }
    }
  }
}

void Engine::updateHunter(float dt) {
  if (!hunterActive) {
    hunterSpawnTimer -= dt;
    if (hunterSpawnTimer <= 0.0f) {
      hunterActive = true;
      // Spawn in far room
      const auto &rooms = map.getRooms();
      if (rooms.size() > 1) {
        int ri = rooms.size() - 1;
        const auto &room = rooms[ri];
        float hx = room.centerX();
        float hy = room.centerY();
        hunter = MonsterFactory::createMonster(MonsterType::HunterMonkey, hx, hy);
        if (hunter) {
          int bonusHP = (waveNumber - 1) * 15 + floorNumber * 20;
          for (int h = 0; h < bonusHP; ++h)
            hunter->takeDamage(-1);
          addFloatingText(hx, hy - 1.0f, "HUNTER APPEARED!", RED);
          eventSystem.triggerComment("THE HUNTER IS COMING FOR YOU!");
        }
      }
    }
  } else if (hunter && !hunter->isDead()) {
    hunter->updateAI(map, player.getX(), player.getY(), dt);
    hunter->tickRangedCooldown(dt);

    float dx = player.getX() - hunter->getX();
    float dy = player.getY() - hunter->getY();
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist <= Monster::ATTACK_RANGE && hunter->getAttackCooldown() <= 0.0f) {
      player.takeDamage(hunter->getAttackDamage());
      damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
      particles.spawnBlood(player.getX(), player.getY(), 5);
      hunter->resetCooldown();
      addFloatingText(player.getX(), player.getY() - 0.5f,
                      TextFormat("-%d", hunter->getAttackDamage()), RED);
    }

    if (hunter->canFireRanged() && dist < hunter->rangedRange && dist > Monster::ATTACK_RANGE) {
      EnemyProjectile eb;
      eb.x = hunter->getX();
      eb.y = hunter->getY();
      float ndx = player.getX() - hunter->getX();
      float ndy = player.getY() - hunter->getY();
      float nlen = std::sqrt(ndx * ndx + ndy * ndy);
      if (nlen > 0) {
        ndx /= nlen;
        ndy /= nlen;
      }
      eb.vx = ndx * 8.0f;
      eb.vy = ndy * 8.0f;
      eb.damage = 12.0f;
      enemyBullets.push_back(eb);
      hunter->resetRangedCooldown();
    }
  } else if (hunter && hunter->isDead()) {
    addFloatingText(hunter->getX(), hunter->getY() - 1.0f, "HUNTER DEFEATED!", GOLD);
    spawnArtifactPickup(hunter->getX(), hunter->getY());
    spawnPickup(hunter->getX(), hunter->getY());
    hunter.reset();
    hunterActive = false;
    hunterSpawnTimer = hunterSpawnInterval;
    if (hunterSpawnInterval > 10.0f)
      hunterSpawnInterval -= 2.0f;
  }
}

void Engine::spawnArtifactPickup(float x, float y) {
  Pickup p;
  p.x = x;
  p.y = y;
  p.type = Pickup::ArtifactItem;
  p.artifact = ArtifactSystem::randomArtifact();
  p.lifetime = 20.0f;
  pickups.push_back(p);
}

void Engine::checkArtifactCombinations() {
  for (size_t i = 0; i < artifacts.size(); ++i) {
    for (size_t j = i + 1; j < artifacts.size(); ++j) {
      Artifact result = ArtifactSystem::tryCombine(artifacts[i], artifacts[j]);
      if (result.name.empty())
        continue;
      // Remove originals, add combined
      ArtifactSystem::unapplyArtifact(artifacts[i], player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                      MAX_AMMO_SG, MAX_AMMO_RL);
      ArtifactSystem::unapplyArtifact(artifacts[j], player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                      MAX_AMMO_SG, MAX_AMMO_RL);
      artifacts.erase(artifacts.begin() + j);
      artifacts.erase(artifacts.begin() + i);
      artifacts.push_back(result);
      ArtifactSystem::applyArtifact(result, player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                    MAX_AMMO_SG, MAX_AMMO_RL);
      syncBulletMods();
      addFloatingText(player.getX(), player.getY() - 1.0f,
                      TextFormat("COMBINED: %s!", result.name.c_str()), ORANGE);
      particles.spawnExplosion(player.getX(), player.getY(), 30);
      return;
    }
  }
}

void Engine::breakWall(int x, int y) {
  map.setFloor(x, y);
  particles.spawnExplosion((float)x, (float)y, 15);
  auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
  if (rr)
    rr->addShake(2.0f);
  // Check if secret room revealed
  addFloatingText((float)x, (float)y - 0.5f, "WALL DESTROYED!", ORANGE);
}

void Engine::spawnSplitter(float x, float y, int splitCount, int maxHp) {
  if (splitCount <= 0)
    return;
  for (int i = 0; i < 2; ++i) {
    float ox = (i == 0) ? -0.5f : 0.5f;
    float oy = (i == 0) ? 0.5f : -0.5f;
    auto s = MonsterFactory::createMonster(MonsterType::SceletonMonkey, x + ox, y + oy);
    if (s) {
      s->isSplitter = true;
      s->splitCount = splitCount - 1;
      s->setAttackDamage(8);
      int newHp = std::max(10, maxHp / 2);
      for (int h = 0; h < newHp; ++h)
        s->takeDamage(-1);
      monsters.push_back(std::move(s));
    }
  }
}

void Engine::applyWaveReward(int choice) {
  if (!showingWaveReward)
    return;
  showingWaveReward = false;
  int r = waveRewardChoices[choice];
  const char *name = waveRewardTexts[choice].c_str();

  switch (r) {
    case 0:  // +20 MAX HP
      for (int i = 0; i < 20; ++i)
        player.takeDamage(-1);
      break;
    case 1:  // +30% DMG (adds to damage multiplier via artifact bonus)
      artifacts.push_back({"Wave Reward (+30% DMG)",
                           "",
                           30,
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
                           GOLD,
                           'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                    MAX_AMMO_SG, MAX_AMMO_RL);
      syncBulletMods();
      break;
    case 2:  // +SPEED
      artifacts.push_back({"Wave Reward (+SPEED)",
                           "",
                           0,
                           0.3f,
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
                           GREEN,
                           'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                    MAX_AMMO_SG, MAX_AMMO_RL);
      break;
    case 3:  // +FIRE RATE
      artifacts.push_back({"Wave Reward (+FIRE RATE)",
                           "",
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
                           ORANGE,
                           'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                    MAX_AMMO_SG, MAX_AMMO_RL);
      break;
    case 4:  // +AMMO
      ammoMG = std::min(MAX_AMMO_MG + 50, ammoMG + 50);
      ammoSG = std::min(MAX_AMMO_SG + 10, ammoSG + 10);
      ammoRL = std::min(MAX_AMMO_RL + 5, ammoRL + 5);
      break;
    case 5:  // +ARMOR (reduced damage taken - not implemented, give HP instead)
      for (int i = 0; i < 30; ++i)
        player.takeDamage(-1);
      break;
    case 6:  // +1 JUMP
      artifacts.push_back({"Wave Reward (+JUMP)",
                           "",
                           0,
                           0,
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
                           SKYBLUE,
                           'R'});
      ArtifactSystem::applyArtifact(artifacts.back(), player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                    MAX_AMMO_SG, MAX_AMMO_RL);
      break;
    case 7:  // VAMPIRE
      mutations.push_back({"Wave Reward (Vampire)", "heal 3 HP on kill", Mutation::Vampire});
      break;
  }
  addFloatingText(player.getX(), player.getY() - 1.5f, TextFormat("REWARD: %s", name), GOLD);
  particles.spawnExplosion(player.getX(), player.getY(), 20);
}

void Engine::syncBulletMods() {
  hasRicochet = false;
  hasPiercing = false;
  hasExplosive = false;
  for (auto &a : artifacts) {
    if (a.ricochet)
      hasRicochet = true;
    if (a.piercingBullets || a.noAmmoDrops)
      hasPiercing = true;
    if (a.explosiveBullets)
      hasExplosive = true;
  }
  if (hasGod && currentGod.type == GodBlessing::GodOfFire) {
    hasExplosive = true;
  }
  bulletSystem.ricochetMod = hasRicochet;
  bulletSystem.piercingMod = hasPiercing;
  bulletSystem.explosiveMod = hasExplosive;
}

float Engine::getDamageMultiplier() {
  // God of Luck: crit chance - roll now, cache result
  if (godLuckCritChance > 0.0f && (float)rand() / (float)RAND_MAX < godLuckCritChance) {
    return 3.0f;  // crit = 3x damage + visual indicated by caller
  }
  float mult = 1.0f;
  for (auto &a : artifacts) {
    if (a.bonusDamage > 0)
      mult += a.bonusDamage / 100.0f;
  }
  for (auto &m : mutations) {
    if (m.type == Mutation::GlassGun)
      mult += 2.0f;
    if (m.type == Mutation::Berserker && player.getHp() > 0 &&
        player.getHp() < player.getMaxHp() * 30 / 100) {
      mult += 1.0f;
    }
  }
  if (hasGod && currentGod.type == GodBlessing::GodOfFire)
    mult += 0.5f;
  if (hasGod && currentGod.type == GodBlessing::GodOfChaos)
    mult += 1.0f;
  return mult;
}

void Engine::handleInput() {
  // Pause toggle
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
    if (!player.isDead())
      paused = !paused;
  }
  if (paused)
    return;

  // Debug toggle
  if (IsKeyPressed(KEY_F1))
    showDebug = !showDebug;

  if (player.isDead()) {
    if (IsKeyPressed(KEY_Q))
      isRunning = false;
    if (IsKeyPressed(KEY_R)) {
      reset();
    }
    return;
  }

  // Dash input (stored, applied in run loop)
  if (IsKeyPressed(KEY_LEFT_SHIFT) && dashCooldown <= 0.0f && !isDashing) {
    isDashing = true;
    dashTimer = DASH_DURATION;
    bool godSpeed = hasGod && currentGod.type == GodBlessing::GodOfSpeed;
    bool fatRoll = false;
    for (auto &m : mutations) {
      if (m.type == Mutation::FatRoll)
        fatRoll = true;
    }
    if (!godSpeed && !fatRoll)
      dashCooldown = DASH_COOLDOWN;
    dashDx = 0.0f;
    dashDy = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      dashDy -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      dashDy += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      dashDx -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      dashDx += 1.0f;
    if (dashDx == 0 && dashDy == 0)
      dashDy = -1.0f;  // default dash forward
    float len = std::sqrt(dashDx * dashDx + dashDy * dashDy);
    if (len > 0) {
      dashDx /= len;
      dashDy /= len;
    }
    audio.playDash();
  }

  // Panic mode + movement input (skip if dashing)
  if (!isDashing) {
    float dx = 0.0f, dy = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      dy -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      dy += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      dx -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      dx += 1.0f;
    // Reverse controls curse (Seven-League Boots artifact)
    for (auto &a : artifacts) {
      if (a.reverseControls) {
        dx = -dx;
        dy = -dy;
      }
    }
    // Critical fail reversed controls
    if (controlsReversed) {
      dx = -dx;
      dy = -dy;
    }
    // WallJump mutation: pass through walls while active
    player.ignoreWalls = (wallJumpTimer > 0.0f);
    player.processInput(dx, dy, deltaTime);

    // Speedster mutation: +50% speed
    for (auto &m : mutations) {
      if (m.type == Mutation::Speedster) {
        player.move(dx * deltaTime * 7.5f, dy * deltaTime * 7.5f);
      }
    }
    // God of Speed bonus
    if (hasGod && currentGod.type == GodBlessing::GodOfSpeed) {
      player.move(dx * deltaTime * 6.0f, dy * deltaTime * 6.0f);
    }
    // Panic mode: +20% speed when HP < 30
    if (player.getHp() > 0 && player.getHp() < 30) {
      float panicBonus = 1.2f;
      float pdx = dx * deltaTime * 3.0f * panicBonus;
      float pdy = dy * deltaTime * 3.0f * panicBonus;
      if (map.isWalkable(player.getX() + pdx, player.getY() + pdy)) {
        player.move(pdx, pdy);
      }
    }
  }

  if (IsKeyPressed(KEY_Q))
    isRunning = false;

  // Weapon switching
  if (IsKeyPressed(KEY_ONE))
    currentWeapon = Weapon::makeFists();
  if (IsKeyPressed(KEY_TWO))
    currentWeapon = Weapon::makeMinigun();
  if (IsKeyPressed(KEY_THREE))
    currentWeapon = Weapon::makeShotgun();
  if (IsKeyPressed(KEY_FOUR))
    currentWeapon = Weapon::makeRocketLauncher();

  // Destructible environment: break nearest wall (F key)
  if (IsKeyPressed(KEY_F)) {
    for (int dy = -2; dy <= 2; ++dy) {
      for (int dx = -2; dx <= 2; ++dx) {
        int wx = (int)player.getX() + dx;
        int wy = (int)player.getY() + dy;
        if (wx < 0 || wx >= map.getWidth() || wy < 0 || wy >= map.getHeight())
          continue;
        const Tile &t = map.getTile(wx, wy);
        if (t.symbol == '#') {
          breakWall(wx, wy);
          dx = 3;
          dy = 3;  // break out
        }
      }
    }
  }

  if (IsKeyPressed(KEY_E) && timeStopCooldownTimer <= 0.0f && !timeStopped) {
    timeStopped = true;
    timeStopTimer = TIME_STOP_DURATION;
    timeStopTextTimer = TIME_STOP_TEXT_DURATION;
    timeStopCooldownTimer = TIME_STOP_COOLDOWN;
    particles.spawnExplosion(player.getX(), player.getY(), 25);
    audio.playZaWarudo();
  }

  // Critical fail system
  if (critFailTimer > 0.0f)
    critFailTimer -= deltaTime;
  if (critFailTimer <= 0.0f && !artifacts.empty() && rand() % 100 < 3) {
    int failType = rand() % 3;
    if (failType == 0) {
      addFloatingText(player.getX(), player.getY() - 1.0f, "CRITICAL FAIL! Weapon jam!", RED);
      weaponCooldownTimer = 1.0f;
    } else if (failType == 1) {
      int selfDmg = 5 + rand() % 10;
      player.takeDamage(selfDmg);
      damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
      addFloatingText(player.getX(), player.getY() - 0.5f, TextFormat("-%d (misfire!)", selfDmg),
                      RED);
    } else {
      addFloatingText(player.getX(), player.getY() - 1.0f, "CONTROLS REVERSED!", ORANGE);
      controlsReversed = true;
      critFailTimer = 3.0f;
    }
    critFailTimer = 2.0f;
  }
  // Reversed controls timer
  if (controlsReversed && critFailTimer <= 0.0f) {
    controlsReversed = false;
    addFloatingText(player.getX(), player.getY() - 1.0f, "Controls normal again", GREEN);
  }

  weaponCooldownTimer -= deltaTime;

  auto *raylibRenderer = dynamic_cast<RaylibRenderer *>(renderer.get());
  auto getMouseTarget = [&]() -> std::pair<float, float> {
    if (!raylibRenderer)
      return {player.getX() + 1, player.getY()};
    Vector2 ms = GetMousePosition();
    Vector2 mw = GetScreenToWorld2D(ms, raylibRenderer->getCamera());
    return {mw.x / TILE_SIZE, mw.y / TILE_SIZE};
  };

  if (currentWeapon.type == WeaponType::Fists) {
    currentWeapon.isFiring = false;
    if (IsKeyPressed(KEY_SPACE) && weaponCooldownTimer <= 0.0f) {
      weaponCooldownTimer = currentWeapon.cooldown;
      if (raylibRenderer)
        raylibRenderer->addShake(4.0f);
      audio.playHit();
      float dmgMult = getDamageMultiplier();
      bool isCrit = dmgMult >= 3.0f;
      int fistDmg = (int)(currentWeapon.damage * dmgMult);
      for (auto &m : monsters) {
        if (m->isDead())
          continue;
        float ddx = m->getX() - player.getX();
        float ddy = m->getY() - player.getY();
        float dist = std::sqrt(ddx * ddx + ddy * ddy);
        if (dist <= currentWeapon.range) {
          m->takeDamage(fistDmg);
          m->onHit();
          map.spillBloodArea((int)m->getX(), (int)m->getY(), 2);
          particles.spawnBlood(m->getX(), m->getY(), 60);
          addFloatingText(m->getX(), m->getY() - 0.5f,
                          isCrit ? TextFormat("CRIT! %d", fistDmg) : TextFormat("%d", fistDmg),
                          isCrit ? GOLD : YELLOW);
          audio.playHit();
          if (m->isDead()) {
            particles.spawnExplosion(m->getX(), m->getY(), 40);
            particles.spawnSmoke(m->getX(), m->getY(), 15);
            map.spillBloodArea((int)m->getX(), (int)m->getY(), 3);
            spawnPickup(m->getX(), m->getY());
            ++killCount;
            addFloatingText(m->getX(), m->getY() - 1.0f, "KILL!", RED);
            addXPForKill();
          }
        }
      }
    }
  }

  if (currentWeapon.type == WeaponType::Minigun) {
    currentWeapon.isFiring = IsMouseButtonDown(MOUSE_LEFT_BUTTON) && ammoMG > 0;

    if (currentWeapon.isFiring) {
      currentWeapon.currentFireRate -= currentWeapon.spinupRate * deltaTime;
      if (currentWeapon.currentFireRate < currentWeapon.minFireRate)
        currentWeapon.currentFireRate = currentWeapon.minFireRate;
    } else {
      currentWeapon.currentFireRate += currentWeapon.spinupRate * deltaTime;
      if (currentWeapon.currentFireRate > currentWeapon.maxFireRate)
        currentWeapon.currentFireRate = currentWeapon.maxFireRate;
    }

    if (raylibRenderer && currentWeapon.isFiring && weaponCooldownTimer <= 0.0f && ammoMG > 0) {
      --ammoMG;
      particles.spawnMuzzleFlash(player.getX(), player.getY());
      weaponCooldownTimer = currentWeapon.currentFireRate;
      raylibRenderer->addShake(1.5f);
      audio.playShoot();

      auto [tx, ty] = getMouseTarget();
      float dmgMult = getDamageMultiplier();
      int baseDmg = (int)(currentWeapon.damage * dmgMult);
      float spreadAngles[] = {-0.15f, 0.0f, 0.15f};
      int shotCount = 1;
      for (auto &m : mutations) {
        if (m.type == Mutation::ExtraArm)
          shotCount = 2;
      }
      for (int si = 0; si < shotCount; ++si) {
        for (float angleOffset : spreadAngles) {
          float extraOff = (si == 1) ? 0.4f : 0.0f;
          float dx = tx - player.getX();
          float dy = ty - player.getY();
          float len = std::sqrt(dx * dx + dy * dy);
          if (len == 0)
            continue;
          dx /= len;
          dy /= len;
          float rx = dx * std::cos(angleOffset + extraOff) - dy * std::sin(angleOffset + extraOff);
          float ry = dx * std::sin(angleOffset + extraOff) + dy * std::cos(angleOffset + extraOff);
          bulletSystem.spawn(player.getX(), player.getY(), player.getX() + rx * 10.0f,
                             player.getY() + ry * 10.0f, (float)baseDmg);
        }
      }
    }
  }

  if (currentWeapon.type == WeaponType::Shotgun) {
    currentWeapon.isFiring = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && ammoSG > 0;
    if (raylibRenderer && currentWeapon.isFiring && weaponCooldownTimer <= 0.0f && ammoSG > 0) {
      --ammoSG;
      weaponCooldownTimer = currentWeapon.cooldown;
      raylibRenderer->addShake(6.0f);
      audio.playShoot();
      particles.spawnMuzzleFlash(player.getX(), player.getY());

      auto [tx, ty] = getMouseTarget();
      float dmgMult = getDamageMultiplier();
      int baseDmg = (int)(currentWeapon.damage * dmgMult);
      float spreadAngles[] = {-0.25f, -0.15f, -0.05f, 0.05f, 0.15f, 0.25f};
      int shotCount = 1;
      for (auto &m : mutations) {
        if (m.type == Mutation::ExtraArm)
          shotCount = 2;
      }
      for (int si = 0; si < shotCount; ++si) {
        for (float angleOffset : spreadAngles) {
          float extraOff = (si == 1) ? 0.4f : 0.0f;
          float dx = tx - player.getX();
          float dy = ty - player.getY();
          float len = std::sqrt(dx * dx + dy * dy);
          if (len == 0)
            continue;
          dx /= len;
          dy /= len;
          float rx = dx * std::cos(angleOffset + extraOff) - dy * std::sin(angleOffset + extraOff);
          float ry = dx * std::sin(angleOffset + extraOff) + dy * std::cos(angleOffset + extraOff);
          bulletSystem.spawn(player.getX(), player.getY(), player.getX() + rx * 10.0f,
                             player.getY() + ry * 10.0f, (float)baseDmg);
        }
      }
    }
  }

  if (currentWeapon.type == WeaponType::RocketLauncher) {
    currentWeapon.isFiring = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && ammoRL > 0;
    if (raylibRenderer && currentWeapon.isFiring && weaponCooldownTimer <= 0.0f && ammoRL > 0) {
      --ammoRL;
      weaponCooldownTimer = currentWeapon.cooldown;
      raylibRenderer->addShake(10.0f);
      audio.playShoot();

      auto [tx, ty] = getMouseTarget();
      float dx = tx - player.getX();
      float dy = ty - player.getY();
      float len = std::sqrt(dx * dx + dy * dy);
      if (len > 0) {
        dx /= len;
        dy /= len;
      }
      float targetX = player.getX() + dx * currentWeapon.range;
      float targetY = player.getY() + dy * currentWeapon.range;

      bulletSystem.spawn(player.getX(), player.getY(), targetX, targetY, 0.0f, true);
      int rCount = 1;
      for (auto &m : mutations) {
        if (m.type == Mutation::ExtraArm)
          rCount = 2;
      }
      if (rCount > 1) {
        bulletSystem.spawn(player.getX() + dy * 0.3f, player.getY() - dx * 0.3f,
                           targetX + dy * 0.5f, targetY - dx * 0.5f, 0.0f, true);
      }
    }
  }
}
void Engine::renderDebugInfo() {
  int y = 10;
  const int lineHeight = 20;
  const int fontSize = 20;

  DrawText(TextFormat("FPS: %d", GetFPS()), 10, y, fontSize, YELLOW);
  y += lineHeight;
  DrawText(TextFormat("Delta: %.4f ms", deltaTime * 1000.0f), 10, y, fontSize, YELLOW);
  y += lineHeight;

  DrawText(TextFormat("Pos: %.2f, %.2f", player.getX(), player.getY()), 10, y, fontSize, GREEN);
  y += lineHeight;
  DrawText(TextFormat("HP: %d", player.getHp()), 10, y, fontSize, GREEN);
  y += lineHeight;
  DrawText(TextFormat("Tile: %d, %d", (int)player.getX(), (int)player.getY()), 10, y, fontSize,
           GREEN);
  y += lineHeight;

  DrawText(TextFormat("Monsters alive: %d", (int)monsters.size()), 10, y, fontSize, RED);
  y += lineHeight;
  for (auto &m : monsters) {
    DrawText(
      TextFormat("  [%c] pos: %.1f,%.1f hp:%d", m->getSymbol(), m->getX(), m->getY(), m->getHp()),
      10, y, fontSize, RED);
    y += lineHeight;
  }

  DrawText(TextFormat("Map: %dx%d", map.getWidth(), map.getHeight()), 10, y, fontSize, GRAY);
  y += lineHeight;

  const Tile &t = map.getTile((int)player.getX(), (int)player.getY());
  DrawText(TextFormat("Tile under player: '%c' color:%d", t.symbol, t.colorPair), 10, y, fontSize,
           GRAY);
}
void Engine::renderHUD() {
  auto &texMgr = TextureManager::getInstance();
  int sw = GetScreenWidth();
  int sh = GetScreenHeight();
  int hPosX = 10, hPosY = sh - 50;

  // Health bar
  const int barW = 200, barH = 20;
  float hpRatio = (float)player.getHp() / player.getMaxHp();
  DrawRectangle(hPosX, hPosY, barW, barH, Color{50, 50, 50, 200});
  Color hpColor = hpRatio > 0.5f ? GREEN : (hpRatio > 0.25f ? ORANGE : RED);
  DrawRectangle(hPosX, hPosY, (int)(barW * hpRatio), barH, hpColor);
  DrawRectangleLines(hPosX, hPosY, barW, barH, Color{200, 200, 200, 100});
  DrawText(TextFormat("%d/%d", player.getHp(), player.getMaxHp()), hPosX + barW + 10, hPosY + 2, 16,
           WHITE);

  // Weapon indicator + ammo
  const char *weaponName = "FISTS";
  Color weaponColor = SKYBLUE;
  std::string ammoText = "";
  const char *weaponTexName = nullptr;
  if (currentWeapon.type == WeaponType::Minigun) {
    weaponName = "MINIGUN";
    weaponColor = ORANGE;
    ammoText = TextFormat(" [%d/%d]", ammoMG, MAX_AMMO_MG);
    weaponTexName = "weapon_minigun";
  } else if (currentWeapon.type == WeaponType::Shotgun) {
    weaponName = "SHOTGUN";
    weaponColor = PINK;
    ammoText = TextFormat(" [%d/%d]", ammoSG, MAX_AMMO_SG);
    weaponTexName = "weapon_shotgun";
  } else if (currentWeapon.type == WeaponType::RocketLauncher) {
    weaponName = "ROCKET";
    weaponColor = RED;
    ammoText = TextFormat(" [%d/%d]", ammoRL, MAX_AMMO_RL);
    weaponTexName = "weapon_rocket";
  }
  if (weaponTexName) {
    float iconSize = 42.0f;
    DrawTexturePro(texMgr.get(weaponTexName), {0, 0, 32, 32},
                   {(float)hPosX, (float)hPosY - 22, iconSize, iconSize}, {0, 0}, 0.0f, WHITE);
    DrawText(TextFormat("[%d] %s%s", (int)currentWeapon.type + 1, weaponName, ammoText.c_str()),
             hPosX + (int)iconSize + 4, hPosY - 22, 16, weaponColor);
  } else {
    DrawText(TextFormat("[%d] %s%s", (int)currentWeapon.type + 1, weaponName, ammoText.c_str()),
             hPosX, hPosY - 22, 16, weaponColor);
  }

  // Floor / XP / Level
  DrawText(TextFormat("FLOOR %d  WAVE %d", floorNumber, waveNumber), sw - 200, hPosY - 22, 14,
           LIME);
  DrawText(TextFormat("LVL %d  XP %d/%d", level, xp, xpToNextLevel), sw - 200, hPosY, 14, SKYBLUE);

  // Kill counter
  DrawText(TextFormat("KILLS: %d", killCount), sw - 100, hPosY + 16, 14, RED);

  // Weapon switch hint
  DrawText("1:FISTS  2:MINIGUN  3:SHOTGUN  4:ROCKET", sw / 2 - 180, sh - 25, 12,
           Color{180, 180, 180, 150});
}

void Engine::render() {
  // ---- Camera zoom for time stop ----
  float targetZoom = 1.0f;
  if (timeStopped) {
    float prog = timeStopTimer / TIME_STOP_DURATION;
    float tN = TIME_STOP_TRANSITION / TIME_STOP_DURATION;
    if (prog > (1.0f - tN)) {
      float t = (prog - (1.0f - tN)) / tN;
      targetZoom = 1.0f + 0.15f * t;
    } else if (prog < tN) {
      float t = prog / tN;
      targetZoom = 1.0f + 0.15f * (1.0f - t);
    } else {
      targetZoom = 1.15f;
    }
  }
  auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
  if (rr)
    rr->setZoom(targetZoom);

  renderer->clear();

  renderer->setCameraTarget(player.getX(), player.getY());
  renderer->beginScene();

  std::vector<Entity *> allEntities;
  allEntities.push_back(&player);
  for (auto &m : monsters) {
    if (!m->isDead())
      allEntities.push_back(m.get());
  }

  renderer->renderMap(map, allEntities);

  // Pickups
  auto &tm = TextureManager::getInstance();
  for (auto &p : pickups) {
    if (!p.active)
      continue;
    float sx = p.x * TILE_SIZE;
    float sy = p.y * TILE_SIZE;
    if (p.type == Pickup::Health) {
      DrawTexturePro(tm.get("pickup_health"), {0, 0, 16, 16},
                     {sx - TILE_SIZE / 2, sy - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE}, {0, 0}, 0.0f,
                     WHITE);
    } else if (p.type == Pickup::ArtifactItem) {
      DrawRectangle((int)sx - 8, (int)sy - 8, 16, 16, {200, 100, 255, 220});
      DrawText("?", (int)sx - 4, (int)sy - 7, 16, WHITE);
    } else {
      DrawTexturePro(tm.get("pickup_ammo"), {0, 0, 16, 16},
                     {sx - TILE_SIZE / 2, sy - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE}, {0, 0}, 0.0f,
                     WHITE);
    }
  }

  // Mimics (chests before reveal)
  for (auto &mim : mimics) {
    if (!mim.active)
      continue;
    float sx = mim.x * TILE_SIZE;
    float sy = mim.y * TILE_SIZE;
    if (!mim.revealed) {
      DrawRectangle((int)sx - 10, (int)sy - 8, 20, 16, {180, 140, 40, 220});
      DrawRectangle((int)sx - 6, (int)sy - 4, 12, 8, {120, 90, 20, 220});
      DrawText("?", (int)sx - 4, (int)sy - 7, 16, GOLD);
    } else {
      DrawRectangle((int)sx - 10, (int)sy - 8, 20, 16, {200, 50, 50, 200});
      DrawText("!", (int)sx - 4, (int)sy - 7, 16, WHITE);
    }
  }

  // Enemy projectiles
  for (auto &b : enemyBullets) {
    if (b.dead)
      continue;
    float sx = b.x * TILE_SIZE;
    float sy = b.y * TILE_SIZE;
    DrawCircle((int)sx, (int)sy, 5, RED);
    DrawCircle((int)(sx - b.vx * 0.015f * TILE_SIZE), (int)(sy - b.vy * 0.015f * TILE_SIZE), 3,
               MAROON);
  }

  particles.render();
  bulletSystem.render();

  // Weapon in player hand, rotated toward mouse cursor
  if (currentWeapon.type != WeaponType::Fists && rr) {
    const char *wname = nullptr;
    if (currentWeapon.type == WeaponType::Minigun)
      wname = "weapon_minigun";
    else if (currentWeapon.type == WeaponType::Shotgun)
      wname = "weapon_shotgun";
    else if (currentWeapon.type == WeaponType::RocketLauncher)
      wname = "weapon_rocket";
    if (wname) {
      Vector2 ms = GetMousePosition();
      Vector2 mw = GetScreenToWorld2D(ms, rr->getCamera());
      float px = player.getX() * TILE_SIZE;
      float py = player.getY() * TILE_SIZE;
      float dx = mw.x - px;
      float dy = mw.y - py;
      float angle = atan2(dy, dx) * RAD2DEG;
      float dist = sqrt(dx * dx + dy * dy);
      float ox = dist > 0.0f ? dx / dist : 1.0f;
      float oy = dist > 0.0f ? dy / dist : 0.0f;
      float weaponDist = TILE_SIZE * 0.75f;
      float ws = TILE_SIZE * 1.3f;
      float wx = px + ox * weaponDist;
      float wy = py + oy * weaponDist;
      DrawTexturePro(tm.get(wname), {0, 0, 32, 32}, {wx, wy, ws, ws}, {ws / 2, ws / 2}, angle,
                     WHITE);
    }
  }

  renderer->endScene();
  renderDebugInfo();

  // ---- Floating damage numbers ----
  for (auto &ft : floatingTexts) {
    float alpha = ft.lifetime / ft.maxLifetime;
    float sx = ft.x * TILE_SIZE;
    float sy = (ft.y - (1.0f - alpha) * 0.5f) * TILE_SIZE;
    Color c = ft.color;
    c.a = (unsigned char)(alpha * 255);
    DrawText(ft.text.c_str(), (int)sx, (int)sy, 18, c);
  }

  // ---- HUD ----
  renderHUD();

  // ---- Wave announcement ----
  if (waveAnnounceTimer > 0.0f) {
    int sw = GetScreenWidth();
    float t = waveAnnounceTimer / WAVE_ANNOUNCE_DURATION;
    unsigned char wa = (unsigned char)(255 * t);
    int fontSize = 60;
    const char *waveText = TextFormat("FLOOR %d - WAVE %d", floorNumber, waveNumber);
    int tw = MeasureText(waveText, fontSize);
    DrawText(waveText, (sw - tw) / 2, 100, fontSize, Color{255, 200, 50, wa});
    waveAnnounceTimer -= deltaTime;
  }

  // ---- Low HP warning ----
  if (player.getHp() > 0 && player.getHp() < 30) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float pulse = std::sin(GetTime() * 5.0f) * 0.3f + 0.7f;
    unsigned char hA = (unsigned char)(80 * pulse);
    DrawRectangle(0, 0, sw, sh, Color{180, 0, 0, hA});
    int bW = 8;
    DrawRectangle(0, 0, sw, bW, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
    DrawRectangle(0, sh - bW, sw, bW, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
    DrawRectangle(0, 0, bW, sh, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
    DrawRectangle(sw - bW, 0, bW, sh, Color{180, 0, 0, (unsigned char)(hA * 1.5f)});
  }

  // ---- Damage overlay (blood splatter) ----
  if (damageOverlayTimer > 0.0f) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float t = damageOverlayTimer / DAMAGE_OVERLAY_DURATION;
    unsigned char dA = (unsigned char)(120 * t);
    DrawRectangle(0, 0, sw, sh, Color{100, 0, 0, dA});
  }

  // ---- Dynamic event overlays ----
  if (eventSystem.isEventActive()) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DynamicEvent ev = eventSystem.getCurrentEvent();
    float warn = eventSystem.getEventWarning();
    float prog = warn > 0.0f ? 1.0f : eventSystem.getEventTimer() / eventSystem.getEventDuration();

    if (ev == DynamicEvent::Eclipse && warn <= 0.0f) {
      unsigned char eA = (unsigned char)(100 * prog);
      DrawRectangle(0, 0, sw, sh, Color{10, 0, 30, eA});
      DrawText("ECLIPSE - monsters deal +50% damage", sw / 2 - 200, 50, 18,
               Color{100, 50, 200, (unsigned char)(200 * prog)});
    }
    if (ev == DynamicEvent::Blackout && warn <= 0.0f) {
      unsigned char bA = (unsigned char)(200 * prog);
      DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, bA});
      DrawCircle(sw / 2, sh / 2, 120, BLANK);
      DrawText("BLACKOUT", sw / 2 - 70, 50, 24, Color{100, 100, 100, (unsigned char)(200 * prog)});
    }
    if (ev == DynamicEvent::GoldRush && warn <= 0.0f) {
      DrawText("GOLD RUSH - double drops!", sw / 2 - 150, 50, 18,
               Color{255, 215, 0, (unsigned char)(180)});
    }
    if (ev == DynamicEvent::MeteorRain && warn > 0.0f) {
      DrawText("METEOR RAIN INCOMING!", sw / 2 - 160, 50, 22, ORANGE);
    }
    if (ev == DynamicEvent::MonsterInvasion && warn > 0.0f) {
      DrawText("MONSTER INVASION!", sw / 2 - 140, 50, 22, RED);
    }
    if (ev == DynamicEvent::ZombieHorde && warn <= 0.0f) {
      DrawText("ZOMBIE HORDE - enemies revive!", sw / 2 - 170, 50, 18,
               Color{0, 200, 0, (unsigned char)(180)});
    }
    if (ev == DynamicEvent::Earthquake && warn <= 0.0f) {
      float intensity = 0.5f + 0.5f * std::sin(GetTime() * 10.0f);
      unsigned char eA = (unsigned char)(60 * intensity);
      DrawRectangle(0, 0, sw, sh, Color{80, 50, 0, eA});
      DrawText("EARTHQUAKE", sw / 2 - 90, 50, 22,
               Color{150, 100, 50, (unsigned char)(200 * intensity)});
    }
    if (ev == DynamicEvent::TimeWarp && warn <= 0.0f) {
      unsigned char tA = (unsigned char)(100 * prog);
      DrawRectangle(0, 0, sw, sh, Color{0, 100, 200, tA});
      DrawText("TIME WARP - slowed!", sw / 2 - 130, 80, 20,
               Color{0, 200, 255, (unsigned char)(200 * prog)});
    }
    // Warning flash
    if (warn > 0.0f) {
      float flash = std::sin(GetTime() * 15.0f) * 0.5f + 0.5f;
      DrawRectangle(0, 0, sw, sh, Color{255, 0, 0, (unsigned char)(30 * flash)});
    }
  }

  if (timeStopped) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    Vector2 center = {sw * 0.5f, sh * 0.5f};

    // Normalised progress: 1.0 = just activated, 0.0 = ending
    float prog = timeStopTimer / TIME_STOP_DURATION;
    float transN = TIME_STOP_TRANSITION / TIME_STOP_DURATION;

    bool activating = prog > (1.0f - transN);
    bool deactivating = prog < transN;

    float actT = activating ? (prog - (1.0f - transN)) / transN : 0.0f;
    float deactT = deactivating ? 1.0f - prog / transN : 0.0f;
    float intensity = activating ? actT : (deactivating ? deactT : 1.0f);

    // ---- 1. White/violet flash on activation ----
    if (activating) {
      unsigned char fa = (unsigned char)(220 * (1.0f - actT));
      DrawRectangle(0, 0, sw, sh, Color{210, 180, 255, fa});
    }

    // ---- 2. Violet flash on deactivation ----
    if (deactivating) {
      unsigned char fa = (unsigned char)(180 * (1.0f - deactT));
      DrawRectangle(0, 0, sw, sh, Color{180, 120, 255, fa});
    }

    // ---- 3. Desaturation (gray overlay) + violet tint + darkening ----
    if (intensity > 0.01f) {
      unsigned char gA = (unsigned char)(70 * intensity);
      unsigned char tA = (unsigned char)(55 * intensity);
      unsigned char dA = (unsigned char)(90 * intensity);

      DrawRectangle(0, 0, sw, sh, Color{128, 128, 128, gA});  // desat
      DrawRectangle(0, 0, sw, sh, Color{70, 50, 130, tA});    // tint
      DrawRectangle(0, 0, sw, sh, Color{0, 0, 8, dA});        // dark
    }

    // ---- 4. Radial blur (concentric tint rings from centre) ----
    if (intensity > 0.01f) {
      float maxR = std::sqrt((float)(sw * sw + sh * sh)) * 0.7f;
      for (int i = 1; i <= 25; ++i) {
        float t = (float)i / 25.0f;
        float radius = maxR * t * t;
        unsigned char a = (unsigned char)(18 * intensity * (1.0f - t));
        DrawCircleLines((int)center.x, (int)center.y, (int)radius, Color{140, 120, 230, a});
      }

      // Chromatic aberration rings (R & B offset)
      for (int i = 1; i <= 18; ++i) {
        float t = (float)i / 18.0f;
        float radius = maxR * t * t;
        unsigned char a = (unsigned char)(7 * intensity * (1.0f - t));
        int off = (int)(4.0f * t);
        DrawCircleLines((int)(center.x + off), (int)center.y, (int)radius, Color{255, 60, 60, a});
        DrawCircleLines((int)(center.x - off), (int)center.y, (int)radius, Color{60, 60, 255, a});
      }
    }

    // ---- 5. Expanding / contracting ring shockwave ----
    if (activating || deactivating) {
      float ringProg = activating ? actT : deactT;
      float ringR = ringProg * 550.0f;
      unsigned char rA = (unsigned char)(130 * (1.0f - ringProg));
      int cx = (int)center.x, cy = (int)center.y;
      DrawCircleLines(cx, cy, (int)ringR, Color{200, 180, 255, rA});
      DrawCircleLines(cx, cy, (int)(ringR * 0.65f), Color{200, 180, 255, (unsigned char)(rA / 2)});
      DrawCircleLines(cx, cy, (int)(ringR * 0.35f), Color{200, 180, 255, (unsigned char)(rA / 3)});
    }

    // ---- 6. Vignette ----
    if (intensity > 0.01f) {
      int vS = (int)(120 * intensity);
      unsigned char vA = (unsigned char)(200 * intensity);
      // top
      DrawRectangleGradientV(0, 0, sw, vS, Color{0, 0, 20, vA}, Color{0, 0, 20, 0});
      // bottom
      DrawRectangleGradientV(0, sh - vS, sw, vS, Color{0, 0, 20, 0}, Color{0, 0, 20, vA});
      // left
      DrawRectangleGradientH(0, 0, vS, sh, Color{0, 0, 20, vA}, Color{0, 0, 20, 0});
      // right
      DrawRectangleGradientH(sw - vS, 0, vS, sh, Color{0, 0, 20, 0}, Color{0, 0, 20, vA});
    }

    // ---- 7. Film grain ----
    if (intensity > 0.01f) {
      int dots = (int)(300 * intensity);
      for (int i = 0; i < dots; ++i) {
        int gx = GetRandomValue(0, sw - 1);
        int gy = GetRandomValue(0, sh - 1);
        unsigned char ga = (unsigned char)(GetRandomValue(0, 35) * intensity);
        DrawPixel(gx, gy, Color{140, 130, 160, ga});
      }
    }

    // ---- 8. Heat distortion (subtle wavy lines) ----
    if (intensity > 0.01f) {
      float hTime = GetTime();
      for (int y = 0; y < sh; y += 16) {
        float wave = std::sin(y * 0.04f + hTime * 3.5f);
        int xOff = (int)(wave * 4.0f * intensity);
        unsigned char hA = (unsigned char)(8 * intensity);
        DrawLine(xOff, y, sw + xOff, y, Color{160, 140, 220, hA});
      }
    }

    // ---- 9. Chromatic aberration coloured borders ----
    if (intensity > 0.01f) {
      int bW = 6;
      unsigned char cA = (unsigned char)(55 * intensity);
      DrawRectangle(0, 0, bW, sh, Color{0, 0, 255, cA});        // blue left
      DrawRectangle(sw - bW, 0, bW, sh, Color{255, 0, 0, cA});  // red right
      DrawRectangle(0, 0, sw, 2, Color{0, 255, 0, (unsigned char)(cA / 2)});
      DrawRectangle(0, sh - 2, sw, 2, Color{0, 255, 0, (unsigned char)(cA / 2)});
    }

    // ---- 10. Motion blur ghosting on deactivation ----
    if (deactivating) {
      float mb = 1.0f - deactT;
      int ghosts = (int)(8 * mb);
      for (int i = 1; i <= ghosts; ++i) {
        float spread = (float)i / ghosts * 12.0f * mb;
        unsigned char gA = (unsigned char)(18 * mb * (1.0f - (float)i / (ghosts + 1)));
        DrawRectangle((int)(-spread), 0, sw, sh, Color{80, 50, 130, gA});
        DrawRectangle((int)(spread), 0, sw, sh, Color{130, 50, 80, gA});
      }
    }

    // ---- 11. "ZA WARUDO!" text ----
    if (timeStopTextTimer > 0) {
      float t = timeStopTextTimer / TIME_STOP_TEXT_DURATION;
      int fontSize = 90 + (int)(30 * (1.0f - t));
      unsigned char alpha = (unsigned char)(255 * t);
      Color zaColor = {255, 215, 0, alpha};

      int w1 = MeasureText("ZA WARUDO!", fontSize);
      int w2 = MeasureText("TOKI WO TOMARE", fontSize / 2);

      DrawText("ZA WARUDO!", (sw - w1) / 2, sh / 2 - fontSize - 10, fontSize, zaColor);
      DrawText("TOKI WO TOMARE", (sw - w2) / 2, sh / 2 + 10, fontSize / 2,
               Color{255, 215, 0, (unsigned char)(alpha * 0.6f)});

      DrawRectangle((sw - w1) / 2 - 20, sh / 2 - fontSize - 20, w1 + 40, fontSize + 50,
                    Color{255, 215, 0, (unsigned char)(alpha * 0.08f)});
    }
  }

  // ---- Commentator text ----
  const auto &comments = eventSystem.getComments();
  int commY = 10;
  for (auto &c : comments) {
    float alpha = c.timer / c.duration;
    int tw = MeasureText(c.text.c_str(), 14);
    DrawText(c.text.c_str(), GetScreenWidth() - tw - 10, commY, 14,
             Color{255, 200, 100, (unsigned char)(255 * alpha)});
    commY += 20;
  }

  // ---- Artifact / Mutation HUD ----
  int hudX = GetScreenWidth() - 220;
  int hudY = 150;
  for (auto &a : artifacts) {
    DrawText(TextFormat("%c %s", a.symbol, a.name.c_str()), hudX, hudY, 12, a.color);
    hudY += 16;
  }
  for (auto &m : mutations) {
    DrawText(TextFormat("* %s", m.name.c_str()), hudX, hudY, 12, m.color);
    hudY += 16;
  }
  if (hasGod) {
    DrawText(TextFormat("GOD: %s", currentGod.name.c_str()), hudX, hudY + 4, 12, currentGod.color);
  }

  // ---- Risk timer display ----
  float timeOnFloor = eventSystem.getTimeOnFloor();
  if (timeOnFloor > 20.0f) {
    DrawText(
      TextFormat("RISK: %.1fs (x%.1f rewards)", timeOnFloor, eventSystem.getRiskMultiplier()), 10,
      80, 14, timeOnFloor > 90.0f ? RED : (timeOnFloor > 60.0f ? ORANGE : YELLOW));
  }

  if (timeStopCooldownTimer > 0 && !timeStopped) {
    DrawText(TextFormat("ZA WARUDO COOLDOWN: %.1f", timeStopCooldownTimer), 10,
             GetScreenHeight() - 30, 18, Color{200, 180, 255, 180});
  } else if (!timeStopped) {
    DrawText("PRESS E: ZA WARUDO", 10, GetScreenHeight() - 30, 18, Color{200, 180, 255, 180});
  }

  // Wave reward overlay
  if (showingWaveReward) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, 180});
    DrawText("CHOOSE YOUR REWARD:", sw / 2 - 200, sh / 2 - 100, 30, GOLD);
    for (int ri = 0; ri < 3; ++ri) {
      Color c = (ri == 0) ? RED : (ri == 1) ? GREEN : SKYBLUE;
      DrawText(TextFormat("[%d] %s", ri + 1, waveRewardTexts[ri].c_str()), sw / 2 - 120,
               sh / 2 - 40 + ri * 40, 22, c);
    }
  }

  renderer->refresh();
}

void Engine::reset() {
  map = Map(120, 80);

  monsters.clear();
  bulletSystem.clear();
  currentWeapon = Weapon::makeFists();
  weaponCooldownTimer = 0.0f;

  player = Player(0.0f, 0.0f, '@', COLOR_PLAYER, 100);

  particles = ParticleSystem();
  attackCooldown = 0.0f;
  timeStopped = false;
  timeStopTimer = 0.0f;
  timeStopTextTimer = 0.0f;
  timeStopCooldownTimer = 0.0f;
  floatingTexts.clear();
  pickups.clear();
  enemyBullets.clear();
  rocketExplosions.clear();

  // Reset all stats
  killCount = 0;
  damageOverlayTimer = 0.0f;
  levelUpFlashTimer = 0.0f;
  waveNumber = 1;
  floorNumber = 1;
  waveTimer = 0.0f;
  waveAnnounceTimer = 0.0f;
  waitingForWave = true;
  xp = 0;
  level = 1;
  xpToNextLevel = 5;
  ammoMG = MAX_AMMO_MG;
  ammoSG = MAX_AMMO_SG;
  ammoRL = MAX_AMMO_RL;
  isDashing = false;
  dashTimer = 0.0f;
  dashCooldown = 0.0f;
  dashDx = 0.0f;
  dashDy = 0.0f;
  paused = false;
  fadeAlpha = 0.0f;
  isRunning = true;

  // Clear new systems
  artifacts.clear();
  mutations.clear();
  hasGod = false;
  hunter.reset();
  hunterActive = false;
  hunterSpawnTimer = hunterSpawnInterval;
  mimics.clear();
  controlsReversed = false;
  shieldHits = 0;
  wallJumpTimer = 0.0f;
  regenTimer = 0.0f;
  drainTimer = 0.0f;
  zombieHordeActive = false;
  laserEyesTimer = 0.0f;
  unstableBuildTimer = 0.0f;
  mimicsSpawnedThisWave = 0;
  mimicAttackTimer = 0.0f;
  hasRicochet = false;
  hasPiercing = false;
  hasExplosive = false;
  critFailTimer = 0.0f;
  bulletSystem.ricochetMod = false;
  bulletSystem.piercingMod = false;
  bulletSystem.explosiveMod = false;
  godGreedDoubleDrops = false;
  godLuckCritChance = 0.0f;
  godSpeedBonus = 0.0f;
  showingWaveReward = false;
  waveRewardTimer = 0.0f;
  frozenHeartTimer = 0.0f;
  mushroomTimer = 0.0f;
  mushroomShrink = false;
  eyeballReveal = false;
  timeWarpSlow = 1.0f;
  earthquakeWallTimer = 0.0f;
  chargerRushTimer = 0.0f;
  eventSystem = EventSystem();
  meta.addRun();

  // Place player in first room
  const auto &rooms = map.getRooms();
  if (!rooms.empty()) {
    player.setPosition(rooms[0].centerX(), rooms[0].centerY());
  }
  // New god blessing
  int godChoice = rand() % 6;
  GodBlessing startGod = ArtifactSystem::allGods[godChoice];
  applyGodBlessing(startGod);
}

void Engine::run() {
  try {
    while (isRunning && !WindowShouldClose()) {
      deltaTime = std::min(GetFrameTime(), 0.05f);

      // TimeWarp event: slow everything
      timeWarpSlow = 1.0f;
      if (eventSystem.isEventActive() && eventSystem.getCurrentEvent() == DynamicEvent::TimeWarp &&
          eventSystem.getEventWarning() <= 0.0f) {
        timeWarpSlow = 0.5f;
      }
      deltaTime *= timeWarpSlow;

      handleInput();

      std::vector<Entity *> allEntities;
      allEntities.push_back(&player);
      for (auto &m : monsters) {
        if (!m->isDead())
          allEntities.push_back(m.get());
      }

      player.setContext(map, allEntities, deltaTime);
      player.update();

      // Event system + risk timer (runs even during time stop)
      eventSystem.update(deltaTime, waveNumber, floorNumber,
                         eventSystem.getTimeOnFloor() + deltaTime, (float)player.getHp(),
                         (float)player.getMaxHp());

      // Wall jump timer
      if (wallJumpTimer > 0.0f)
        wallJumpTimer -= deltaTime;

      // Laser Eyes mutation: auto-beam at nearest enemy
      for (auto &m : mutations) {
        if (m.type == Mutation::LaserEyes) {
          laserEyesTimer -= deltaTime;
          if (laserEyesTimer <= 0.0f) {
            laserEyesTimer = 0.3f;
            Monster *nearest = nullptr;
            float nearestDist = 999.0f;
            for (auto &mon : monsters) {
              if (mon->isDead())
                continue;
              float ldx = mon->getX() - player.getX();
              float ldy = mon->getY() - player.getY();
              float ld = std::sqrt(ldx * ldx + ldy * ldy);
              if (ld < nearestDist) {
                nearestDist = ld;
                nearest = mon.get();
              }
            }
            if (nearest) {
              int laserDmg = 5 + level;
              nearest->takeDamage(laserDmg);
              nearest->onHit();
              particles.spawnBlood(nearest->getX(), nearest->getY(), 5);
              addFloatingText(nearest->getX(), nearest->getY() - 0.5f,
                              TextFormat("LASER: %d", laserDmg), SKYBLUE);
              auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
              if (rr)
                rr->addShake(1.0f);
              if (nearest->isDead()) {
                particles.spawnExplosion(nearest->getX(), nearest->getY(), 20);
                spawnPickup(nearest->getX(), nearest->getY());
                ++killCount;
                addXPForKill();
              }
            }
          }
        }
      }

      // AttractMonsters artifact: pull enemies toward player
      for (auto &a : artifacts) {
        if (a.attractMonsters) {
          for (auto &mon : monsters) {
            if (mon->isDead())
              continue;
            float adx = player.getX() - mon->getX();
            float ady = player.getY() - mon->getY();
            float adist = std::sqrt(adx * adx + ady * ady);
            if (adist > 0.5f && adist < 15.0f) {
              float pull = 2.0f * deltaTime;
              mon->move(adx / adist * pull, ady / adist * pull);
            }
          }
        }
      }

      // Unstable build v0.1 artifact: random effect every 5s
      for (auto &a : artifacts) {
        if (a.name == "Unstable build v0.1") {
          unstableBuildTimer -= deltaTime;
          if (unstableBuildTimer <= 0.0f) {
            unstableBuildTimer = 5.0f;
            int u = rand() % 4;
            if (u == 0) {
              player.takeDamage(-15);
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: +15 HP", GREEN);
            } else if (u == 1) {
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: teleport!", PINK);
              const auto &urooms = map.getRooms();
              if (!urooms.empty()) {
                int ri = rand() % urooms.size();
                player.setPosition(urooms[ri].centerX(), urooms[ri].centerY());
              }
            } else if (u == 2) {
              for (auto &mon : monsters) {
                if (!mon->isDead()) {
                  mon->takeDamage(20);
                  mon->onHit();
                }
              }
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: chain lightning!",
                              YELLOW);
            } else {
              addFloatingText(player.getX(), player.getY() - 1.0f, "UNSTABLE: dud...", GRAY);
            }
          }
        }
      }

      // Mimic reveal & attack
      mimicAttackTimer -= deltaTime;
      for (auto &mim : mimics) {
        if (!mim.active)
          continue;
        float mdx = player.getX() - mim.x;
        float mdy = player.getY() - mim.y;
        float mdist = std::sqrt(mdx * mdx + mdy * mdy);
        if (!mim.revealed && mdist < MIMIC_REVEAL_RANGE) {
          mim.revealed = true;
          eventSystem.triggerComment("IT'S A MIMIC!");
          audio.playDeath();
          auto mimicMonster =
            MonsterFactory::createMonster(MonsterType::SceletonMonkey, (float)mim.x, (float)mim.y);
          if (mimicMonster) {
            mimicMonster->setAttackDamage((int)mim.hp / 5);
            monsters.push_back(std::move(mimicMonster));
          }
          addFloatingText((float)mim.x, (float)mim.y - 1.0f, "MIMIC!", RED);
        }
        if (mim.revealed && mdist > 15.0f) {
          mim.active = false;
        }
      }

      // Frozen Heart artifact: freeze enemy every 3rd shot (stun = stop AI briefly)
      frozenHeartTimer -= deltaTime;
      if (frozenHeartTimer <= 0.0f) {
        for (auto &a : artifacts) {
          if (a.name == "Frozen Heart") {
            frozenHeartTimer = 0.5f;
            // Freeze nearest enemy for 0.3s (move them back so they lose pathfinding)
            for (auto &mon : monsters) {
              if (mon->isDead())
                continue;
              float fdx = mon->getX() - player.getX();
              float fdy = mon->getY() - player.getY();
              if (std::sqrt(fdx * fdx + fdy * fdy) < 8.0f) {
                mon->move(-fdx * 0.1f, -fdy * 0.1f);
                addFloatingText(mon->getX(), mon->getY() - 0.5f, "FROZEN!", SKYBLUE);
                break;
              }
            }
          }
        }
      }

      // Mushroom artifact: random buff every 10s
      mushroomTimer -= deltaTime;
      if (mushroomTimer <= 0.0f) {
        for (auto &a : artifacts) {
          if (a.name == "Mushroom") {
            mushroomTimer = 10.0f;
            int mu = rand() % 4;
            if (mu == 0) {
              player.takeDamage(-25);
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: +25 HP", GREEN);
            } else if (mu == 1) {
              for (auto &mon : monsters) {
                if (!mon->isDead()) {
                  mon->takeDamage(30);
                  mon->onHit();
                }
              }
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: SPORES!", PINK);
            } else if (mu == 2) {
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: SPEED UP!", LIME);
              // Give speed boost by moving player toward cursor
              float rx = (float)(rand() % 200 - 100) / 100.0f;
              float ry = (float)(rand() % 200 - 100) / 100.0f;
              player.move(rx, ry);
            } else {
              player.takeDamage(10);
              addFloatingText(player.getX(), player.getY() - 1.0f, "MUSHROOM: POISON!", RED);
            }
          }
        }
      }

      // Eyeball artifact: minimap always revealed (handled in render)
      for (auto &a : artifacts) {
        if (a.name == "Eyeball")
          eyeballReveal = true;
      }

      // Artifact HP drain
      drainTimer -= deltaTime;
      if (drainTimer <= 0.0f) {
        drainTimer = 1.0f;
        for (auto &a : artifacts) {
          if (a.drainHpPerSec > 0) {
            player.takeDamage(a.drainHpPerSec);
            addFloatingText(player.getX(), player.getY() - 0.5f,
                            TextFormat("-%d (curse)", a.drainHpPerSec), RED);
          }
        }
      }

      // God of Health: regen
      if (hasGod && currentGod.type == GodBlessing::GodOfHealth) {
        regenTimer -= deltaTime;
        if (regenTimer <= 0.0f) {
          regenTimer = 1.0f;
          if (player.getHp() < player.getMaxHp()) {
            player.takeDamage(-1);
          }
        }
      }

      if (timeStopped) {
        timeStopTimer -= deltaTime;
        timeStopTextTimer -= deltaTime;
        timeStopCooldownTimer -= deltaTime;
        if (timeStopTimer <= 0.0f) {
          timeStopped = false;
          particles.spawnExplosion(player.getX(), player.getY(), 20);
        }
      } else {
        timeStopCooldownTimer -= deltaTime;
        particles.update(deltaTime);
        bulletSystem.update(deltaTime, map, monsters, particles);

        // Process rocket explosions
        for (auto &[ex, ey] : bulletSystem.getRocketExplosions()) {
          explosionDamage(ex, ey, 3.0f, 80.0f, &player);
        }

        // Floating texts
        for (auto &ft : floatingTexts)
          ft.lifetime -= deltaTime;
        floatingTexts.erase(std::remove_if(floatingTexts.begin(), floatingTexts.end(),
                                           [](const FloatingText &ft) { return ft.lifetime <= 0; }),
                            floatingTexts.end());

        // Damage overlay
        if (damageOverlayTimer > 0.0f)
          damageOverlayTimer -= deltaTime;

        // Pickup lifetime / collection
        for (auto &p : pickups) {
          if (!p.active)
            continue;
          p.lifetime -= deltaTime;
          if (p.lifetime <= 0) {
            p.active = false;
            continue;
          }
          float dx = player.getX() - p.x;
          float dy = player.getY() - p.y;
          if (std::sqrt(dx * dx + dy * dy) < 0.5f) {
            p.active = false;
            if (p.type == Pickup::Health) {
              int heal = 30;
              player.takeDamage(-heal);
              addFloatingText(p.x, p.y - 0.5f, TextFormat("+%d HP", heal), GREEN);
              particles.spawnBlood(p.x, p.y, 10);
            } else if (p.type == Pickup::Ammo) {
              ammoMG = std::min(MAX_AMMO_MG, ammoMG + 30);
              ammoSG = std::min(MAX_AMMO_SG, ammoSG + 5);
              addFloatingText(p.x, p.y - 0.5f, "+AMMO", ORANGE);
            } else if (p.type == Pickup::RocketAmmo) {
              ammoRL = std::min(MAX_AMMO_RL, ammoRL + 3);
              addFloatingText(p.x, p.y - 0.5f, "+ROCKETS", ORANGE);
            } else if (p.type == Pickup::ArtifactItem) {
              ArtifactSystem::applyArtifact(p.artifact, player, ammoMG, ammoSG, ammoRL, MAX_AMMO_MG,
                                            MAX_AMMO_SG, MAX_AMMO_RL);
              artifacts.push_back(p.artifact);
              addFloatingText(p.x, p.y - 0.5f, TextFormat("ARTIFACT: %s", p.artifact.name.c_str()),
                              PINK);
              syncBulletMods();
              checkArtifactCombinations();
            }
            audio.playPickup();
          }
        }
        pickups.erase(
          std::remove_if(pickups.begin(), pickups.end(), [](const Pickup &p) { return !p.active; }),
          pickups.end());

        // Enemy projectiles
        for (auto &b : enemyBullets) {
          if (b.dead)
            continue;
          b.x += b.vx * deltaTime;
          b.y += b.vy * deltaTime;
          b.lifetime -= deltaTime;
          if (b.lifetime <= 0 || !map.isWalkable(b.x, b.y)) {
            b.dead = true;
            continue;
          }
          float dx = player.getX() - b.x;
          float dy = player.getY() - b.y;
          if (std::sqrt(dx * dx + dy * dy) < 0.5f) {
            player.takeDamage((int)b.damage);
            damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
            particles.spawnBlood(player.getX(), player.getY(), 10);
            addFloatingText(player.getX(), player.getY() - 0.5f, TextFormat("-%d", (int)b.damage),
                            RED);
            b.dead = true;
          }
        }
        enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                                          [](const EnemyProjectile &b) { return b.dead; }),
                           enemyBullets.end());

        // Monster AI + melee attacks + ranged attacks
        for (auto &monster : monsters) {
          monster->updateAI(map, player.getX(), player.getY(), deltaTime);
          monster->tickRangedCooldown(deltaTime);

          float dx = player.getX() - monster->getX();
          float dy = player.getY() - monster->getY();
          float dist = std::sqrt(dx * dx + dy * dy);

          // Melee
          if (dist <= Monster::ATTACK_RANGE && monster->getAttackCooldown() <= 0.0f) {
            // Shield mutation: block every 3rd hit
            for (auto &m : mutations) {
              if (m.type == Mutation::Shield) {
                shieldHits = (shieldHits + 1) % 3;
                if (shieldHits == 0) {
                  addFloatingText(player.getX(), player.getY() - 1.0f, "BLOCKED!", SKYBLUE);
                  monster->resetCooldown();
                  goto afterMelee;
                }
              }
            }
            player.takeDamage(monster->getAttackDamage());
            damageOverlayTimer = DAMAGE_OVERLAY_DURATION;
            particles.spawnBlood(player.getX(), player.getY(), 5);
            monster->resetCooldown();
            addFloatingText(player.getX(), player.getY() - 0.5f,
                            TextFormat("-%d", monster->getAttackDamage()), RED);
            auto *rr = dynamic_cast<RaylibRenderer *>(renderer.get());
            if (rr)
              rr->addShake(3.0f);
            // Thorn Skin: reflect damage
            for (auto &m : mutations) {
              if (m.type == Mutation::ThornSkin) {
                int reflectDmg = 5 + level;
                monster->takeDamage(reflectDmg);
                monster->onHit();
                addFloatingText(monster->getX(), monster->getY() - 0.5f,
                                TextFormat("-%d (thorn)", reflectDmg), GREEN);
              }
            }
          }
        afterMelee:;

          // Push player out of monster overlap
          float minDist = 0.6f;
          if (dist < minDist && dist > 0.01f) {
            float push = (minDist - dist) * 0.5f;
            player.move(-dx / dist * push, -dy / dist * push);
          }

          // Ranged
          if (monster->canFireRanged() && dist < monster->rangedRange &&
              dist > Monster::ATTACK_RANGE) {
            EnemyProjectile eb;
            eb.x = monster->getX();
            eb.y = monster->getY();
            float ndx = player.getX() - monster->getX();
            float ndy = player.getY() - monster->getY();
            float nlen = std::sqrt(ndx * ndx + ndy * ndy);
            if (nlen > 0) {
              ndx /= nlen;
              ndy /= nlen;
            }
            float speed = 6.0f;
            eb.vx = ndx * speed;
            eb.vy = ndy * speed;
            eb.damage = 8.0f;
            enemyBullets.push_back(eb);
            monster->resetRangedCooldown();
          }
        }

        // ---- New monster behaviors ----
        for (auto &mon : monsters) {
          if (mon->isDead())
            continue;

          // Healer: heal nearby monsters every 3s
          if (mon->isHealer) {
            mon->healTimer -= deltaTime;
            if (mon->healTimer <= 0.0f) {
              mon->healTimer = 3.0f;
              for (auto &other : monsters) {
                if (other->isDead() || other.get() == mon.get())
                  continue;
                float hdx = other->getX() - mon->getX();
                float hdy = other->getY() - mon->getY();
                if (std::sqrt(hdx * hdx + hdy * hdy) < 5.0f) {
                  other->takeDamage(-15);
                  addFloatingText(other->getX(), other->getY() - 0.5f, "+15 (heal)", GREEN);
                }
              }
              particles.spawnBlood(mon->getX(), mon->getY(), 8);
            }
          }

          // Summoner: spawn minion every 4s
          if (mon->isSummoner) {
            mon->summonTimer -= deltaTime;
            if (mon->summonTimer <= 0.0f) {
              mon->summonTimer = 4.0f;
              auto minion = MonsterFactory::createMonster(
                MonsterType::SceletonMonkey, mon->getX() + ((rand() % 2) ? 0.5f : -0.5f),
                mon->getY() + ((rand() % 2) ? 0.5f : -0.5f));
              if (minion) {
                monsters.push_back(std::move(minion));
                addFloatingText(mon->getX(), mon->getY() - 0.5f, "SUMMON!", MAGENTA);
              }
            }
          }

          // Charger: rush player very fast
          if (mon->isCharger) {
            float cdx = player.getX() - mon->getX();
            float cdy = player.getY() - mon->getY();
            float cdist = std::sqrt(cdx * cdx + cdy * cdy);
            if (cdist > 0) {
              cdx /= cdist;
              cdy /= cdist;
            }
            mon->move(cdx * 6.0f * deltaTime, cdy * 6.0f * deltaTime);
          }
        }

        // Splitter: on death, split into smaller copies
        for (size_t si = 0; si < monsters.size(); ++si) {
          auto &mon = monsters[si];
          if (mon->isDead() && mon->isSplitter && mon->splitCount > 0) {
            int sc = mon->splitCount;
            int hp = mon->getMaxHp();
            spawnSplitter(mon->getX(), mon->getY(), sc, hp);
            mon->splitCount = 0;
          }
        }

        // Earthquake: crack walls randomly
        if (eventSystem.isEventActive() &&
            eventSystem.getCurrentEvent() == DynamicEvent::Earthquake &&
            eventSystem.getEventWarning() <= 0.0f) {
          earthquakeWallTimer -= deltaTime;
          if (earthquakeWallTimer <= 0.0f) {
            earthquakeWallTimer = 0.5f;
            int wx = rand() % map.getWidth();
            int wy = rand() % map.getHeight();
            const Tile &et = map.getTile(wx, wy);
            if (et.symbol == '#') {
              breakWall(wx, wy);
            }
          }
        }

        // Update hunter (runs even in time stop for timer)
        updateHunter(deltaTime);

        // Dynamic events
        handleDynamicEvents();
      }

      // ZombieHorde: on activation, give all monsters a revive
      if (eventSystem.isEventActive() &&
          eventSystem.getCurrentEvent() == DynamicEvent::ZombieHorde &&
          eventSystem.getEventWarning() <= 0.0f && !zombieHordeActive) {
        zombieHordeActive = true;
        for (auto &mon : monsters) {
          if (!mon->isDead())
            mon->zombieRevivesLeft = 1;
        }
      }
      // ZombieHorde: revive dead monsters once
      if (zombieHordeActive && eventSystem.getCurrentEvent() != DynamicEvent::ZombieHorde) {
        zombieHordeActive = false;
      }
      if (zombieHordeActive) {
        for (auto &mon : monsters) {
          if (mon->isDead() && mon->zombieRevivesLeft > 0) {
            mon->zombieRevivesLeft = 0;
            for (int hi = 0; hi < mon->getMaxHp() / 2; ++hi)
              mon->takeDamage(-1);
            addFloatingText(mon->getX(), mon->getY() - 0.5f, "REVIVED!", LIME);
            particles.spawnExplosion(mon->getX(), mon->getY(), 15);
          }
        }
      }

      monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),
                       [](const std::unique_ptr<Monster> &monster) { return monster->isDead(); }),
        monsters.end());

      // Wave transition
      if (monsters.empty() && !waitingForWave && !player.isDead()) {
        ++waveNumber;
        waveTimer = 0.0f;
        waitingForWave = true;
        showingWaveReward = true;
        waveRewardTimer = 10.0f;

        // Generate 3 random choices
        const char *rewardNames[] = {"+20 MAX HP", "+30% DMG", "+SPEED",  "+FIRE RATE",
                                     "+AMMO",      "+ARMOR",   "+1 JUMP", "VAMPIRE"};
        for (int ri = 0; ri < 3; ++ri) {
          waveRewardChoices[ri] = rand() % 8;
          waveRewardTexts[ri] = rewardNames[waveRewardChoices[ri]];
        }

        // New floor every WAVES_PER_FLOOR waves
        if (waveNumber % WAVES_PER_FLOOR == 1) {
          ++floorNumber;
          map = Map(120, 80);
          const auto &newRooms = map.getRooms();
          if (!newRooms.empty()) {
            player.setPosition(newRooms[0].centerX(), newRooms[0].centerY());
          }
          addFloatingText(player.getX(), player.getY() - 2.0f,
                          TextFormat("- FLOOR %d -", floorNumber), GOLD);
          // Bonus HP on floor transition
          int heal = 20 + floorNumber * 5;
          player.takeDamage(-heal);
          addFloatingText(player.getX(), player.getY() - 1.0f, TextFormat("+%d HP", heal), GREEN);
        }
      }

      // Wave reward selection
      if (showingWaveReward && IsKeyPressed(KEY_ONE)) {
        applyWaveReward(0);
      }
      if (showingWaveReward && IsKeyPressed(KEY_TWO)) {
        applyWaveReward(1);
      }
      if (showingWaveReward && IsKeyPressed(KEY_THREE)) {
        applyWaveReward(2);
      }
      if (showingWaveReward) {
        waveRewardTimer -= deltaTime;
        if (waveRewardTimer <= 0.0f)
          showingWaveReward = false;
      }
      if (waitingForWave && !player.isDead()) {
        waveTimer += deltaTime;
        if (waveTimer >= WAVE_DELAY) {
          startNextWave();
        }
      }

      if (player.isDead()) {
        while (!WindowShouldClose()) {
          BeginDrawing();
          ClearBackground(BLACK);
          DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 180});
          meta.updateHighScore(waveNumber, floorNumber);
          DrawText("YOU FUCK DIED", 250, 160, 80, RED);
          DrawText(TextFormat("FLOOR: %d  WAVE: %d  LEVEL: %d  KILLS: %d", floorNumber, waveNumber,
                              level, killCount),
                   200, 245, 20, GRAY);
          DrawText(TextFormat("BEST: Wave %d  Floor %d", meta.data().highestWave,
                              meta.data().highestFloor),
                   260, 275, 16, GOLD);
          DrawText("PRESS R TO RESTART OR Q TO QUIT", 180, 320, 25, WHITE);
          EndDrawing();

          if (IsKeyPressed(KEY_Q)) {
            isRunning = false;
            break;
          }
          if (IsKeyPressed(KEY_R)) {
            reset();
            break;
          }
        }
        if (!isRunning)
          break;
        continue;
      }
      render();
    }
  } catch (const GameException &e) {
    std::cout << "Game error: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cout << "Unexpected error: " << e.what() << std::endl;
  }
}

}  // namespace rogue
