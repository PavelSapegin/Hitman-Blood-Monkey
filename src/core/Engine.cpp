#include "../include/rogue/core/Engine.h"
#include "../include/rogue/Exceptions.h"
#include "../include/rogue/Renderer.h"
#include "../include/rogue/core/Command.h"
#include "../include/rogue/entities/Monster.h"
#include "../include/rogue/entities/MonsterFactory.h"
#include "../include/rogue/entities/Player.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <stdexcept>

namespace rogue {

Engine::Engine(std::unique_ptr<IRenderer> r)
    : isRunning(true), map(80, 25), player(10.0f, 5.0f, '@', 2, 100),
      deltaTime(0.0f) {

  if (!r) {
    throw InitializationException("Renderer cannot be null");
  }
  renderer = std::move(r);
  renderer->initialize();

  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::SceletonMonkey, 5.0f, 5.0f));
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::GoblinMonkey, 10.0f, 10.0f));
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::MonkeyBoss, 15.0f, 15.0f));
}

Engine::~Engine() { renderer->shutdown(); }

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
}

void Engine::renderDebugInfo() {
  DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, YELLOW);
  DrawText(TextFormat("Player: %.1f, %.1f", player.getX(), player.getY()), 10,
           30, 20, YELLOW);
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
