#pragma once
#include "../Renderer.h"
#include "../entities/Monster.h"
#include "../entities/Player.h"
#include "../world/Map.h"
#include <memory>
#include <vector>

namespace rogue {

class Engine {
public:
  Engine(std::unique_ptr<IRenderer> renderer);
  ~Engine();

  void run(); // Main game loop

private:
  std::unique_ptr<IRenderer>
      renderer; // Using unique_ptr for automatic memory management
  void handleInput();
  void render();

  bool isRunning;
  Map map;
  Player player;
  std::vector<std::unique_ptr<Monster>>
      monsters; // Using unique_ptr for automatic memory management
};

} // namespace rogue
