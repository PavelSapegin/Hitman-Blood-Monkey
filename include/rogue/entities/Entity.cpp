#include "Entity.h"

namespace rogue {

    Entity::Entity(int x, int y, char symbol, int color, int maxHp) : x(x), y(y), symbol(symbol), color(color), hp(maxHp), maxHp(maxHp) {}

    int Entity::getX() const {
        return x;
    }

    int Entity::getY() const {
        return y;
    }

    char Entity::getSymbol() const {
        return symbol;
    }

    int Entity::getColor() const {
        return color;
    }

    int Entity::getHp() const {
        return hp;
    }

    bool Entity::isDead() const {
        return hp <= 0;
    }

    void Entity::takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) {
            hp = 0;
        }
    }

    void Entity::setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    void Entity::move(int dx, int dy) {
        x += dx;
        y += dy;
    }

} // namespace rogue
