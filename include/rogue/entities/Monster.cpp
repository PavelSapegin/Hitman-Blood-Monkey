#include "Monster.h"

namespace rogue {

Monster::Monster(float x, float y, char symbol, int color, int maxHp)
    : Entity(x, y, symbol, color, maxHp) {}

void Monster::update() {
  // Implement simple monster behavior (e.g., random movement)
  // For now, this is just a placeholder
}

} // namespace rogue
