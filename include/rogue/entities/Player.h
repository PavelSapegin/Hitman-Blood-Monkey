#pragma once
#include "Entity.h"

namespace rogue {

class Player : public Entity {
public:
  Player(int x, int y, char symbol, int color, int maxHp);
  void update() override; // Implement player-specific behavior
  void move(int dx, int dy);
};
} // namespace rogue
