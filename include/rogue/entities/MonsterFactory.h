#pragma once
#include "Monster.h"
#include <memory>

namespace rogue {
enum class MonsterType { SceletonMonkey, GoblinMonkey, MonkeyBoss };

class MonsterFactory {
public:
  static std::unique_ptr<Monster> createMonster(MonsterType type, float x,
                                                float y) {
    switch (type) {
    case MonsterType::SceletonMonkey:
        return std::make_unique<Monster>(x, y, 's', 3, 30);   // color 3 = monster1
    case MonsterType::GoblinMonkey:
        return std::make_unique<Monster>(x, y, 'g', 4, 50);   // color 4 = monster2  
    case MonsterType::MonkeyBoss:
        return std::make_unique<Monster>(x, y, 'M', 5, 100);  // color 5 = boss
    default:
      return nullptr;
    }
  }
};
} // namespace rogue
