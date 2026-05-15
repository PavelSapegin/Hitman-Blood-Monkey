#include "../include/rogue/core/Engine.h"
#include "../include/rogue/Exceptions.h"
#include "../include/rogue/Raylib_renderer.h"
#include "../include/rogue/core/Command.h"
#include "../include/rogue/entities/MonsterFactory.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <stdexcept>

namespace rogue {

Engine::Engine(std::unique_ptr<IRenderer> r)
    : isRunning(true), map(80, 25), player(10.0f, 5.0f, '@', 2, 100),
      lastFrameTime(0.0), deltaTime(0.0f) {

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
}

Engine::~Engine() {
  renderer->shutdown();
}

void Engine::handleInput() {
  float dx = 0.0f, dy = 0.0f;
  float moveAmount = playerSpeed * deltaTime;

  // Input handling for movement (independent checks for each direction)
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    { dx -= 1.0f; dy -= 1.0f; }
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { dx += 1.0f; dy += 1.0f; }
  
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  { dx -= 1.0f; dy += 1.0f; }
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { dx += 1.0f; dy -= 1.0f; }

  if (IsKeyPressed(KEY_Q)) {
    isRunning = false;
    return;
  }

  // Normalize movement vector if diagonal movement occurs
  if (dx != 0.0f || dy != 0.0f) {
    float length = std::sqrt(dx * dx + dy * dy);
    // Normalize to prevent faster diagonal movement
    if (length > 0) { 
      dx /= length;
      dy /= length;
    }
    
    // Apply speed
    dx *= moveAmount;
    dy *= moveAmount;

    float targetX = player.getX() + dx;
    float targetY = player.getY() + dy;

    // Collision with walls (grid-based)
    int tileX = static_cast<int>(std::round(targetX));
    int tileY = static_cast<int>(std::round(targetY));

    // Clamp to map bounds
    if (tileX < 0) tileX = 0;
    if (tileX >= map.getWidth()) tileX = map.getWidth() - 1;
    if (tileY < 0) tileY = 0;
    if (tileY >= map.getHeight()) tileY = map.getHeight() - 1;

    if (!map.isWalkable(tileX, tileY)) {
      return; // Blocked by wall
    }

    // Check for monster at target position
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
}

void Engine::renderDebugInfo() {
  // Draw debug info on screen
  DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, YELLOW);
  DrawText(TextFormat("Delta: %.4f", deltaTime), 10, 35, 20, YELLOW);
  DrawText(TextFormat("Player: %.1f, %.1f", player.getX(), player.getY()), 10,
           60, 20, YELLOW);
  DrawText(TextFormat("Speed: %.0f u/s", playerSpeed), 10, 85, 20, YELLOW);
  float moveAmount = playerSpeed * deltaTime;
  DrawText(TextFormat("Move/frame: %.2f", moveAmount), 10, 110, 20, YELLOW);

  // Show key states
  DrawText(TextFormat("W:%d A:%d S:%d D:%d", IsKeyDown(KEY_W), IsKeyDown(KEY_A),
                      IsKeyDown(KEY_S), IsKeyDown(KEY_D)),
           10, 140, 20, YELLOW);
  DrawText(TextFormat("UP:%d LT:%d DN:%d RT:%d", IsKeyDown(KEY_UP),
                      IsKeyDown(KEY_LEFT), IsKeyDown(KEY_DOWN),
                      IsKeyDown(KEY_RIGHT)),
           10, 165, 20, YELLOW);
}

void Engine::render() {
  renderer->clear();

  auto rayRenderer = dynamic_cast<RaylibRenderer *>(renderer.get());
  if (rayRenderer) {
    // Update camera target
    rayRenderer->updateCamera(player.getX(), player.getY());

    // Begin camera mode for everything
    BeginMode2D(rayRenderer->getCamera());

    // Draw map and dynamic objects
    map.render(*renderer);
    renderer->drawChar(player.getX(), player.getY(), player.getSymbol(),
                       player.getColor());
    for (const auto &monster : monsters) {
      renderer->drawChar(monster->getX(), monster->getY(), monster->getSymbol(),
                         monster->getColor());
    }

    EndMode2D();
  } else {
    // Fallback for non-Raylib renderers
    map.render(*renderer);
    renderer->drawChar(player.getX(), player.getY(), player.getSymbol(),
                       player.getColor());
    for (const auto &monster : monsters) {
      renderer->drawChar(monster->getX(), monster->getY(), monster->getSymbol(),
                         monster->getColor());
    }
  }

  // Debug overlay drawn before EndDrawing
  renderDebugInfo();
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