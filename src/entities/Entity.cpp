#include "rogue/entities/Entity.h"

namespace rogue {

Entity::Entity(float x, float y, char symbol, int color, int maxHp)
    : x(x), y(y), symbol(symbol), color(color), hp(maxHp), maxHp(maxHp) {}

float Entity::getX() const { return x; }

float Entity::getY() const { return y; }

char Entity::getSymbol() const { return symbol; }

int Entity::getColor() const { return color; }

int Entity::getHp() const { return hp; }

bool Entity::isDead() const { return hp <= 0; }

void Entity::takeDamage(int damage) {
  hp -= damage;
  if (hp < 0) {
    hp = 0;
  }
}

void Entity::setPosition(float newX, float newY) {
  x = newX;
  y = newY;
}

void Entity::move(float dx, float dy) {
  x += dx;
  y += dy;
}

} // namespace rogue
