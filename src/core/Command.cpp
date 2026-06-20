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
    return;

  target->takeDamage(damage);
  if (target->isDead()) {
    map.spillBlood(target->getX(), target->getY());
  }
}
} // namespace rogue
