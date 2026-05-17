#include "../include/rogue/core/Engine.h"
#include "../include/rogue/Exceptions.h"
#include "../include/rogue/ParticleSystem.h"
#include "../include/rogue/Renderer.h"
#include "../include/rogue/TextureManager.h"
#include "../include/rogue/entities/Monster.h"
#include "../include/rogue/entities/MonsterFactory.h"
#include "../include/rogue/entities/Player.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>
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
const auto& rooms = map.getRooms();
  if (!rooms.empty()) {
      // Игрок спавнится в первой комнате
      player.setPosition(rooms[0].centerX(), rooms[0].centerY());

      // Монстры спавнятся в остальных комнатах — по несколько в каждой
      std::mt19937 rng(std::random_device{}());
      for (int i = 1; i < (int)rooms.size(); ++i) {
          const auto& room = rooms[i];
          int count = 3 + rng() % 5; // 3-7 монстров в комнате
          for (int j = 0; j < count; ++j) {
              float mx = room.x + 1 + rng() % (room.w - 2);
              float my = room.y + 1 + rng() % (room.h - 2);
              MonsterType type = static_cast<MonsterType>(rng() % 3);
              monsters.push_back(MonsterFactory::createMonster(type, mx, my));
          }
      }
    }
}

Engine::~Engine() {
  TextureManager::getInstance().unloadAll();
  renderer->shutdown();
}

void Engine::handleInput() {
  float dx = 0.0f, dy = 0.0f;

  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
    dy -= 1.0f;
  }
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
    dy += 1.0f;
  }
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
    dx -= 1.0f;
  }
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
    dx += 1.0f;
  }

  player.processInput(dx, dy, deltaTime);

  if (IsKeyPressed(KEY_Q)) {
    isRunning = false;
  }

  if (attackCooldown > 0.0f)
    attackCooldown -= deltaTime;

  if (IsKeyPressed(KEY_SPACE) && attackCooldown <= 0.0f) {
    attackCooldown = ATTACK_COOLDOWN;

    for (auto &m : monsters) {
      if (m->isDead())
        continue;

      float dx = m->getX() - player.getX();
      float dy = m->getY() - player.getY();
      float dist = std::sqrt(dx * dx + dy * dy);

      if (dist <= ATTACK_RANGE) {
        m->takeDamage(999); // убиваем сразу
        map.spillBlood(m->getX(), m->getY());
        particles.spawnBlood(m->getX(), m->getY(), 30);
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
  renderer->endScene();
  renderDebugInfo();
  renderer->refresh();
}

void Engine::run() {
  try {
    while (isRunning && !WindowShouldClose()) {
      deltaTime = GetFrameTime();

      handleInput();

      std::vector<Entity *> allEntities;
      allEntities.push_back(&player);
      for (auto &m : monsters) {
        if (!m->isDead())
          allEntities.push_back(m.get());
      }

      player.setContext(map, allEntities, deltaTime);
      particles.update(deltaTime);
      player.update();
      for (auto &monster : monsters) {
        monster->update();
      }

      monsters.erase(
          std::remove_if(monsters.begin(), monsters.end(),
                         [](const std::unique_ptr<Monster> &monster) {
                           return monster->isDead();
                         }),
          monsters.end());

      render();
    }
  } catch (const GameException &e) {
    std::cout << "Game error: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cout << "Unexpected error: " << e.what() << std::endl;
  }
}

} // namespace rogue
