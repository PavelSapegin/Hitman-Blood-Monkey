#include "Player.h"

namespace rogue {

Player::Player(int x, int y, char symbol, int color, int maxHp)
    : Entity(x, y, symbol, color, maxHp) {}

void Player::update() {
  // Implement player-specific behavior (e.g., handle input)
}

void Player::move(int dx, int dy) {
  x += dx;
  y += dy;
}

} // namespace rogue
