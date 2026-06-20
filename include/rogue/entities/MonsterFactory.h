#pragma once
#include <memory>

#include "Monster.h"

namespace rogue {
enum class MonsterType {
  SceletonMonkey,
  GoblinMonkey,
  MonkeyBoss,
  GunMonkey,
  KamikazeMonkey,
  HunterMonkey,
  HealerMonkey,
  SummonerMonkey,
  ChargerMonkey,
  SplitterMonkey
};

class MonsterFactory {
public:
  static std::unique_ptr<Monster> createMonster(MonsterType type, float x, float y) {
    switch (type) {
      case MonsterType::SceletonMonkey:
        return std::make_unique<Monster>(x, y, 's', 3, 30);  // color 3 = monster1
      case MonsterType::GoblinMonkey:
        return std::make_unique<Monster>(x, y, 'g', 4, 50);  // color 4 = monster2
      case MonsterType::MonkeyBoss:
        return std::make_unique<Monster>(x, y, 'M', 5, 100);  // color 5 = boss
      case MonsterType::GunMonkey: {
        auto m = std::make_unique<Monster>(x, y, 'g', 6, 40);
        m->setRanged(true);
        m->setRangedDamage(8);
        m->setAggroRange(12.0f);
        return m;
      }
      case MonsterType::KamikazeMonkey: {
        auto m = std::make_unique<Monster>(x, y, 'k', 7, 25);
        m->setKamikaze(true);
        m->setAggroRange(14.0f);
        m->setAttackDamage(30);
        return m;
      }
      case MonsterType::HunterMonkey: {
        auto m = std::make_unique<Monster>(x, y, 'H', 8, 80);
        m->setAggroRange(50.0f);  // global tracking
        m->setAttackDamage(20);
        m->setRanged(true);
        m->setRangedDamage(12);
        return m;
      }
      case MonsterType::HealerMonkey: {
        auto m = std::make_unique<Monster>(x, y, 'h', 3, 35);
        m->isHealer = true;
        m->healTimer = 3.0f;
        m->setAggroRange(8.0f);
        return m;
      }
      case MonsterType::SummonerMonkey: {
        auto m = std::make_unique<Monster>(x, y, 'S', 6, 50);
        m->isSummoner = true;
        m->summonTimer = 4.0f;
        m->setAggroRange(6.0f);
        m->setAttackDamage(8);
        return m;
      }
      case MonsterType::ChargerMonkey: {
        auto m = std::make_unique<Monster>(x, y, 'C', 4, 40);
        m->isCharger = true;
        m->setAggroRange(18.0f);
        m->setAttackDamage(25);
        return m;
      }
      case MonsterType::SplitterMonkey: {
        auto m = std::make_unique<Monster>(x, y, 's', 3, 50);
        m->isSplitter = true;
        m->splitCount = 2;
        m->origSymbol = 's';
        m->origColor = 3;
        m->origMaxHp = 50;
        m->setAttackDamage(10);
        return m;
      }
      default:
        return nullptr;
    }
  }
};
}  // namespace rogue
