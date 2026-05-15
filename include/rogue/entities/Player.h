#pragma once
#include "Entity.h"

namespace rogue {

class Player : public Entity {
public:
  Player(float x, float y, char symbol, int color, int maxHp);
  void update() override; // Implement player-specific behavior
  void move(float dx, float dy);
};
} // namespace rogue
