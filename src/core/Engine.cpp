#include "rogue/core/Engine.h"
#include "rogue/Exceptions.h"
#include "rogue/ParticleSystem.h"
#include "rogue/Raylib_renderer.h"
#include "rogue/Renderer.h"
#include "rogue/TextureManager.h"
#include "rogue/entities/Monster.h"
#include "rogue/entities/MonsterFactory.h"
#include "rogue/entities/Player.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <raylib.h>

namespace rogue {

Engine::Engine(std::unique_ptr<IRenderer> r)
    : isRunning(true), map(120, 80), player(0.0f, 0.0f, '@', COLOR_PLAYER, 100),
      deltaTime(0.0f) {

  if (!r) {
    throw InitializationException("Renderer cannot be null");
  }
  renderer = std::move(r);
  renderer->initialize();
  auto &tm = TextureManager::getInstance();
  tm.loadTexture("wall", "../assets/tile_0040.png");
  tm.loadTexture("floor", "../assets/tile_0049.png");
  tm.loadTexture("player", "../assets/tile_0088.png");
  tm.loadTexture("monster1", "../assets/tile_0109.png");
  tm.loadTexture("monster2", "../assets/tile_0111.png");
  tm.loadTexture("boss", "../assets/tile_0108.png");
  spawnEntities();
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
      MonsterType type = static_cast<MonsterType>(rng() % 3);
      monsters.push_back(MonsterFactory::createMonster(type, mx, my));
    }
  }
}

void Engine::handleInput() {
  if (player.isDead()) {
    if (IsKeyPressed(KEY_Q))
      isRunning = false;
    if (IsKeyPressed(KEY_R)) {
      reset();
    }
    return;
  }

  float dx = 0.0f, dy = 0.0f;
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    dy -= 1.0f;
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    dy += 1.0f;
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    dx -= 1.0f;
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    dx += 1.0f;
  player.processInput(dx, dy, deltaTime);

  if (IsKeyPressed(KEY_Q))
    isRunning = false;

  if (IsKeyPressed(KEY_ONE))
    currentWeapon = Weapon::makeFists();
  if (IsKeyPressed(KEY_TWO))
    currentWeapon = Weapon::makeMinigun();

  if (IsKeyPressed(KEY_E) && timeStopCooldownTimer <= 0.0f && !timeStopped) {
    timeStopped = true;
    timeStopTimer = TIME_STOP_DURATION;
    timeStopEffectTimer = TIME_STOP_EFFECT_DURATION;
    timeStopCooldownTimer = TIME_STOP_COOLDOWN;
    particles.spawnExplosion(player.getX(), player.getY(), 20);
  }

  weaponCooldownTimer -= deltaTime;

  if (currentWeapon.type == WeaponType::Fists) {
    currentWeapon.isFiring = false;
    if (IsKeyPressed(KEY_SPACE) && weaponCooldownTimer <= 0.0f) {
      weaponCooldownTimer = currentWeapon.cooldown;
      for (auto &m : monsters) {
        if (m->isDead())
          continue;
        float ddx = m->getX() - player.getX();
        float ddy = m->getY() - player.getY();
        float dist = std::sqrt(ddx * ddx + ddy * ddy);
        if (dist <= currentWeapon.range) {
          m->takeDamage(static_cast<int>(currentWeapon.damage));
          m->onHit();
          map.spillBloodArea((int)m->getX(), (int)m->getY(), 2);
          particles.spawnBlood(m->getX(), m->getY(), 60);
          if (m->isDead()) {
            particles.spawnExplosion(m->getX(), m->getY(), 40);
            particles.spawnSmoke(m->getX(), m->getY(), 15);
            map.spillBloodArea((int)m->getX(), (int)m->getY(), 3);
          }
        }
      }
    }
  }

  if (currentWeapon.type == WeaponType::Minigun) {
    currentWeapon.isFiring = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (currentWeapon.isFiring) {
      currentWeapon.currentFireRate -= currentWeapon.spinupRate * deltaTime;
      if (currentWeapon.currentFireRate < currentWeapon.minFireRate)
        currentWeapon.currentFireRate = currentWeapon.minFireRate;
    } else {
      currentWeapon.currentFireRate += currentWeapon.spinupRate * deltaTime;
      if (currentWeapon.currentFireRate > currentWeapon.maxFireRate)
        currentWeapon.currentFireRate = currentWeapon.maxFireRate;
    }

    auto *raylibRenderer = dynamic_cast<RaylibRenderer *>(renderer.get());
    if (raylibRenderer && currentWeapon.isFiring &&
        weaponCooldownTimer <= 0.0f) {
      particles.spawnMuzzleFlash(player.getX(), player.getY());
      weaponCooldownTimer = currentWeapon.currentFireRate;

      Vector2 mouseScreen = GetMousePosition();
      Vector2 mouseWorld =
          GetScreenToWorld2D(mouseScreen, raylibRenderer->getCamera());

      float tx = mouseWorld.x / TILE_SIZE;
      float ty = mouseWorld.y / TILE_SIZE;

      float spreadAngles[] = {-0.15f, 0.0f, 0.15f};
      for (float angleOffset : spreadAngles) {
        float dx = tx - player.getX();
        float dy = ty - player.getY();
        float len = std::sqrt(dx * dx + dy * dy);
        if (len == 0)
          continue;
        dx /= len;
        dy /= len;

        float rotatedX =
            dx * std::cos(angleOffset) - dy * std::sin(angleOffset);
        float rotatedY =
            dx * std::sin(angleOffset) + dy * std::cos(angleOffset);

        float btx = player.getX() + rotatedX * 10.0f;
        float bty = player.getY() + rotatedY * 10.0f;

        bulletSystem.spawn(player.getX(), player.getY(), btx, bty,
                           currentWeapon.damage);
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
  DrawText(TextFormat("Delta: %.4f ms", deltaTime * 1000.0f), 10, y, fontSize,
           YELLOW);
  y += lineHeight;

  DrawText(TextFormat("Pos: %.2f, %.2f", player.getX(), player.getY()), 10, y,
           fontSize, GREEN);
  y += lineHeight;
  DrawText(TextFormat("HP: %d", player.getHp()), 10, y, fontSize, GREEN);
  y += lineHeight;
  DrawText(TextFormat("Tile: %d, %d", (int)player.getX(), (int)player.getY()),
           10, y, fontSize, GREEN);
  y += lineHeight;

  DrawText(TextFormat("Monsters alive: %d", (int)monsters.size()), 10, y,
           fontSize, RED);
  y += lineHeight;
  for (auto &m : monsters) {
    DrawText(TextFormat("  [%c] pos: %.1f,%.1f hp:%d", m->getSymbol(),
                        m->getX(), m->getY(), m->getHp()),
             10, y, fontSize, RED);
    y += lineHeight;
  }

  DrawText(TextFormat("Map: %dx%d", map.getWidth(), map.getHeight()), 10, y,
           fontSize, GRAY);
  y += lineHeight;

  const Tile &t = map.getTile((int)player.getX(), (int)player.getY());
  DrawText(
      TextFormat("Tile under player: '%c' color:%d", t.symbol, t.colorPair), 10,
      y, fontSize, GRAY);
}
void Engine::render() {
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
  particles.render();
  bulletSystem.render();
  renderer->endScene();
  renderDebugInfo();

  if (timeStopped) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // 1. Dark background overlay
    DrawRectangle(0, 0, sw, sh, Color{0, 0, 10, 120});

    // 2. Desaturated blue-purple tint
    DrawRectangle(0, 0, sw, sh, Color{50, 30, 90, 50});

    // 3. Expanding shockwave (first 0.6s)
    float effectAge = TIME_STOP_EFFECT_DURATION - timeStopEffectTimer;
    if (effectAge < 0.6f) {
      float t = effectAge / 0.6f;
      float radius = t * 450.0f;
      unsigned char alpha = (unsigned char)(140 * (1.0f - t * t));

      Vector2 pc = {sw / 2.0f, sh / 2.0f};

      DrawCircleLines((int)pc.x, (int)pc.y, radius,
                      Color{200, 180, 255, alpha});
      DrawCircleLines((int)pc.x, (int)pc.y, radius * 0.65f,
                      Color{200, 180, 255, (unsigned char)(alpha / 2)});
      DrawCircleLines((int)pc.x, (int)pc.y, radius * 0.35f,
                      Color{200, 180, 255, (unsigned char)(alpha / 3)});
    }

    // 4. Chromatic aberration border glow (subtle colored edges)
    int borderSize = 4;
    DrawRectangle(0, 0, sw, borderSize, Color{100, 50, 180, 40});
    DrawRectangle(0, sh - borderSize, sw, borderSize, Color{180, 50, 100, 40});
    DrawRectangle(0, 0, borderSize, sh, Color{100, 50, 180, 40});
    DrawRectangle(sw - borderSize, 0, borderSize, sh, Color{180, 50, 100, 40});

    // 5. "ZA WARUDO!" anime text
    if (timeStopEffectTimer > 0) {
      float t = timeStopEffectTimer / TIME_STOP_EFFECT_DURATION;
      int fontSize = 90 + (int)(30 * (1.0f - t));
      unsigned char alpha = (unsigned char)(255 * t);
      Color zaColor = {255, 215, 0, alpha};

      int w1 = MeasureText("ZA WARUDO!", fontSize);
      int w2 = MeasureText("TOKI WO TOMARE", fontSize / 2);

      DrawText("ZA WARUDO!", (sw - w1) / 2, sh / 2 - fontSize - 10,
               fontSize, zaColor);
      DrawText("TOKI WO TOMARE", (sw - w2) / 2, sh / 2 + 10,
               fontSize / 2,
               Color{255, 215, 0, (unsigned char)(alpha * 0.6f)});

      // Golden glow behind text
      DrawRectangle((sw - w1) / 2 - 20, sh / 2 - fontSize - 20,
                    w1 + 40, fontSize + 50,
                    Color{255, 215, 0, (unsigned char)(alpha * 0.08f)});
    }
  }

  if (timeStopCooldownTimer > 0 && !timeStopped) {
    DrawText(TextFormat("ZA WARUDO COOLDOWN: %.1f", timeStopCooldownTimer),
             10, GetScreenHeight() - 30, 18, Color{200, 180, 255, 180});
  } else if (!timeStopped) {
    DrawText("PRESS E: ZA WARUDO", 10, GetScreenHeight() - 30, 18,
             Color{200, 180, 255, 180});
  }

  renderer->refresh();
}

void Engine::reset() {
  map = Map(120, 80);

  monsters.clear();
  bulletSystem.clear();
  currentWeapon = Weapon::makeFists();
  weaponCooldownTimer = 0.0f;

  player =
      Player(0.0f, 0.0f, '@', COLOR_PLAYER, 100);
  spawnEntities();

  particles = ParticleSystem();
  attackCooldown = 0.0f;
  timeStopped = false;
  timeStopTimer = 0.0f;
  timeStopCooldownTimer = 0.0f;
  timeStopEffectTimer = 0.0f;
  isRunning = true;
}

void Engine::run() {
  try {
    while (isRunning && !WindowShouldClose()) {
      deltaTime = std::min(GetFrameTime(),0.05f);

      handleInput();

      std::vector<Entity *> allEntities;
      allEntities.push_back(&player);
      for (auto &m : monsters) {
        if (!m->isDead())
          allEntities.push_back(m.get());
      }

      player.setContext(map, allEntities, deltaTime);
      player.update();

      if (timeStopped) {
        timeStopTimer -= deltaTime;
        timeStopEffectTimer -= deltaTime;
        timeStopCooldownTimer -= deltaTime;
        if (timeStopTimer <= 0.0f) {
          timeStopped = false;
          particles.spawnExplosion(player.getX(), player.getY(), 15);
        }
      } else {
        timeStopCooldownTimer -= deltaTime;
        particles.update(deltaTime);
        bulletSystem.update(deltaTime, map, monsters, particles);

        for (auto &monster : monsters) {
          monster->updateAI(map, player.getX(), player.getY(), deltaTime);

          float dx = player.getX() - monster->getX();
          float dy = player.getY() - monster->getY();
          float dist = std::sqrt(dx * dx + dy * dy);

          if (dist <= Monster::ATTACK_RANGE &&
              monster->getAttackCooldown() <= 0.0f) {
            player.takeDamage(monster->getAttackDamage());
            particles.spawnBlood(player.getX(), player.getY(), 5);
            monster->resetCooldown();
          }
        }
      }

      monsters.erase(
          std::remove_if(monsters.begin(), monsters.end(),
                         [](const std::unique_ptr<Monster> &monster) {
                           return monster->isDead();
                         }),
          monsters.end());
      if (player.isDead()) {
        while (!WindowShouldClose()) {
          BeginDrawing();
          ClearBackground(BLACK);
          DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                        {0, 0, 0, 180});
          DrawText("YOU FUCK DIED", 250, 220, 80, RED);
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

} // namespace rogue
