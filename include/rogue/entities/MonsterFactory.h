#pragma once
#include "Monster.h"
#include <memory>

namespace rogue
{
    enum class MonsterType
    {
        SceletonMonkey,
        GoblinMonkey,
        MonkeyBoss
    };

    class MonsterFactory
    {
        public:
        static std::unique_ptr<Monster> createMonster(MonsterType type, int x, int y)
        {
            switch (type)
            {
                case MonsterType::SceletonMonkey:
                    return std::make_unique<Monster>(x, y, 's', COLOR_MONSTER, 30);
                case MonsterType::GoblinMonkey:
                    return std::make_unique<Monster>(x, y, 'g', COLOR_MONSTER, 50);
                case MonsterType::MonkeyBoss:
                    return std::make_unique<Monster>(x, y, 'M', COLOR_MONSTER, 100);
                default:
                    return nullptr;
            }
        }
    };
}
