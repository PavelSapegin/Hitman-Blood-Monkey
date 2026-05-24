#pragma once
#include "../world/Map.h"
#include "Entity.h"
#include <vector>

namespace rogue {

class Player : public Entity {
public:
  Player(float x, float y, char symbol, int color, int health);

  void update() override;
  void processInput(float dx, float dy, float deltaTime);
  void setInput(float dx, float dy);
  void setContext(Map &map, const std::vector<Entity *> &entities,
                  float deltaTime);

private:
  float inputDx = 0, inputDy = 0;
  Map *mapPtr = nullptr;
  const std::vector<Entity *> *entitiesPtr = nullptr;
  float m_deltaTime = 0;

  float playerSpeed = 15.0f;
  float maxStep = 0.5f;
};

} // namespace rogue
