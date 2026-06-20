#pragma once
#include "rogue/entities/Entity.h"
#include "rogue/entities/Monster.h"
#include "rogue/world/Map.h"

namespace rogue {
class Command {
public:
  virtual ~Command() = default;
  virtual void execute(Entity &entity, Map &map) = 0;
};

class MoveCommand : public Command {
  int dx, dy;

public:
  MoveCommand(int dx, int dy) : dx(dx), dy(dy) {}
  void execute(Entity &entity, Map &map) override;
};

class AttackCommand : public Command {
  Monster *target;
  int damage;

public:
  AttackCommand(Monster *target, int damage = 10) : target(target), damage(damage) {}
  void execute(Entity &entity, Map &map) override;
};
} // namespace rogue
