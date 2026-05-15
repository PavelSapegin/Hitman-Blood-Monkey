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

public:
  AttackCommand(Monster *target) : target(target) {}
  void execute(Entity &entity, Map &map) override;
};
} // namespace rogue
