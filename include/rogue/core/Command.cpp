#include "rogue/core/Command.h"

namespace rogue {
void MoveCommand::execute(Entity &entity, Map &map) {
  int newX = entity.getX() + dx;
  int newY = entity.getY() + dy;
  if (map.isWalkable(newX, newY)) {
    entity.setPosition(newX, newY);
  }
}

void AttackCommand::execute(Entity &entity, Map &map) {
  if (!target)
    return; // No target to attack

  // Simple attack logic: reduce target's health by a fixed amount
  int damage = 10; // Example damage value
  target->takeDamage(damage);
  if (target->isDead()) {
    map.spillBlood(target->getX(), target->getY());
  }
}
} // namespace rogue
