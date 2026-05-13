#include "../include/rogue/core/Engine.h"
#include "../include/rogue/Exceptions.h"
#include "../include/rogue/core/Command.h"
#include "../include/rogue/entities/MonsterFactory.h"
#include <algorithm>
#include <iostream>
#include <raylib.h>
#include <stdexcept>
#include <cmath>

namespace rogue {
Engine::Engine(std::unique_ptr<IRenderer> r)
    : isRunning(true), map(80, 25), player(1.0f, 1.0f, '@', 1, 100),
      lastFrameTime(0.0) {

  if (!r) {
    throw InitializationException("Renderer cannot be null");
  }
  renderer = std::move(r);
  renderer->initialize();

  // Add some monsters for testing
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::SceletonMonkey, 5.0f, 5.0f));
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::GoblinMonkey, 10.0f, 10.0f));
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::MonkeyBoss, 15.0f, 15.0f));
}

Engine::~Engine() {
  if (renderer) {
    renderer->shutdown();
  }
}

void Engine::handleInput() {
  float dx = 0.0f, dy = 0.0f;
  float moveAmount = playerSpeed * deltaTime;

  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    dy = -moveAmount;
  else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dy = moveAmount;
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx = -moveAmount;
  else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx = moveAmount;

  if (IsKeyPressed(KEY_Q)) {
    isRunning = false;
    return;
  }

  if (dx == 0.0f && dy == 0.0f) return;

  float targetX = player.getX() + dx;
  float targetY = player.getY() + dy;

  // Collision with walls (grid-based)
  int tileX = static_cast<int>(targetX);
  int tileY = static_cast<int>(targetY);
  if (!map.isWalkable(tileX, tileY)) {
    return; // Blocked by wall
  }

  // Check for monster at target position (adjacent tile)
  auto targetMonster = std::find_if(
      monsters.begin(), monsters.end(),
      [&](const std::unique_ptr<Monster> &monster) {
        float mx = monster->getX();
        float my = monster->getY();
        return std::abs(mx - targetX) < 0.5f && std::abs(my - targetY) < 0.5f;
      });

  if (targetMonster != monsters.end()) {
    auto command = std::make_unique<AttackCommand>(targetMonster->get());
    command->execute(player, map);
  } else {
    player.move(dx, dy);
  }
}

void Engine::render() {
  renderer->clear();
  map.render(*renderer);
  renderer->drawChar(player.getX(), player.getY(), player.getSymbol(),
                     player.getColor());
  for (const auto &monster : monsters) {
    renderer->drawChar(monster->getX(), monster->getY(), monster->getSymbol(),
                       monster->getColor());
  }
  renderer->refresh();
}

void Engine::run() {
  try {
    while (isRunning && !WindowShouldClose()) {
      deltaTime = GetFrameTime();

      handleInput();
      render();

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
    }

  } catch (const GameException &e) {
    renderer->shutdown();
    std::cout << "Game error: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    renderer->shutdown();
    std::cout << "Unexpected error: " << e.what() << std::endl;
  }
}

} // namespace rogue