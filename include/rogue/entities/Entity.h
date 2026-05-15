#pragma once

namespace rogue {

class Entity {
public:
  Entity(float x, float y, char symbol, int color, int maxHp);
  virtual ~Entity() = default;

  float getX() const;
  float getY() const;
  char getSymbol() const;
  int getColor() const;
  int getHp() const;
  bool isDead() const;
  void takeDamage(int damage);
  void setPosition(float newX, float newY);
  virtual void update() = 0; // Pure virtual function for updating entity state
  void move(float dx, float dy);

protected:
  float x;
  float y;
  char symbol;
  int color;
  int hp;
  int maxHp;
};

} // namespace rogue
