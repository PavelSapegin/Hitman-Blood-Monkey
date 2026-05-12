#include "../include/rogue/core/Engine.h"
#include "../include/rogue/Exceptions.h"
#include "../include/rogue/core/Command.h"
#include "../include/rogue/entities/MonsterFactory.h"
#include <algorithm>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>

namespace rogue {
Engine::Engine(std::unique_ptr<IRenderer> r)
    : isRunning(true), map(80, 25), player(1, 1, '@', COLOR_PLAYER, 100) {

  if (!r) {
    throw InitializationException("Renderer cannot be null");
  }
  renderer = std::move(r);
  renderer->initialize();

  // Add some monsters for testing
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::SceletonMonkey, 5, 5));
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::GoblinMonkey, 10, 10));
  monsters.push_back(
      MonsterFactory::createMonster(MonsterType::MonkeyBoss, 15, 15));
}

Engine::~Engine() {
  if (renderer) {
    renderer->shutdown(); // End ncurses mode
  }
}

void Engine::handleInput() {
  int ch = getch();
  int dx = 0, dy = 0;
  std::unique_ptr<Command> command = nullptr;
  switch (ch) {
  case 'q':
    isRunning = false;
    break;
  case KEY_UP:
    dy = -1;
    break;
  case KEY_DOWN:
    dy = 1;
    break;
  case KEY_LEFT:
    dx = -1;
    break;
  case KEY_RIGHT:
    dx = 1;
    break;
  }

  int targetX = player.getX() + dx;
  int targetY = player.getY() + dy;

  auto targetMonster = std::find_if(
      monsters.begin(), monsters.end(),
      [&](const std::unique_ptr<Monster> &monster) {
        return monster->getX() == targetX && monster->getY() == targetY;
      });
  if (targetMonster != monsters.end()) {
    command = std::make_unique<AttackCommand>(targetMonster->get());
  } else {
    command = std::make_unique<MoveCommand>(dx, dy);
  }
  if (command) {
    command->execute(player, map);
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
    while (isRunning) {
      render();
      handleInput();

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
