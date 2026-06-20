#include "rogue/entities/Player.h"

#include <algorithm>
#include <cmath>

namespace rogue {

Player::Player(float x, float y, char symbol, int color, int health)
  : Entity(x, y, symbol, color, health) {}

void Player::processInput(float dx, float dy, float dt) {
  inputDx = dx;
  inputDy = dy;
  m_deltaTime = dt;
}

void Player::setInput(float dx, float dy) {
  inputDx = dx;
  inputDy = dy;
}

void Player::setContext(Map &map, const std::vector<Entity *> &entities, float dt) {
  mapPtr = &map;
  entitiesPtr = &entities;
  m_deltaTime = dt;
}

void Player::update() {
  if (!mapPtr)
    return;

  float dx = inputDx;
  float dy = inputDy;

  // Normalize input to prevent faster diagonal movement
  float length = std::sqrt(dx * dx + dy * dy);
  if (length > 0) {
    dx /= length;
    dy /= length;
  }

  float moveAmount = playerSpeed * m_deltaTime;
  dx *= moveAmount;
  dy *= moveAmount;

  // Step limiting to prevent tunneling through walls
  float stepSize = std::sqrt(dx * dx + dy * dy);
  if (stepSize > maxStep) {
    dx = (dx / stepSize) * maxStep;
    dy = (dy / stepSize) * maxStep;
  }

  constexpr float collisionRadius = 0.35f;
  auto canMoveTo = [&](float testX, float testY) {
    if (ignoreWalls)
      return true;
    return mapPtr->isWalkable(testX + collisionRadius, testY) &&
           mapPtr->isWalkable(testX - collisionRadius, testY) &&
           mapPtr->isWalkable(testX, testY + collisionRadius) &&
           mapPtr->isWalkable(testX, testY - collisionRadius);
  };

  int steps = std::max(1, static_cast<int>(std::ceil(std::max(std::abs(dx), std::abs(dy)) / 0.1f)));
  float stepX = dx / steps;
  float stepY = dy / steps;

  for (int i = 0; i < steps; ++i) {
    float nextX = x + stepX;
    float nextY = y + stepY;

    if (canMoveTo(nextX, nextY)) {
      move(stepX, stepY);
      continue;
    }

    if (stepX != 0.0f && canMoveTo(x + stepX, y)) {
      move(stepX, 0.0f);
      continue;
    }

    if (stepY != 0.0f && canMoveTo(x, y + stepY)) {
      move(0.0f, stepY);
      continue;
    }

    break;
  }
}

}  // namespace rogue
