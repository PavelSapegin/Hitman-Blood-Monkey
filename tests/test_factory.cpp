#include <gtest/gtest.h>

#include "rogue/entities/MonsterFactory.h"

using namespace rogue;

TEST(MonsterFactoryTest, CreateSceletonMonkey) {
  auto m = MonsterFactory::createMonster(MonsterType::SceletonMonkey, 1.0f, 2.0f);
  ASSERT_NE(m, nullptr);
  EXPECT_FLOAT_EQ(m->getX(), 1.0f);
  EXPECT_FLOAT_EQ(m->getY(), 2.0f);
  EXPECT_EQ(m->getSymbol(), 's');
  EXPECT_EQ(m->getHp(), 30);
  EXPECT_FALSE(m->isDead());
}

TEST(MonsterFactoryTest, CreateGoblinMonkey) {
  auto m = MonsterFactory::createMonster(MonsterType::GoblinMonkey, 0.0f, 0.0f);
  ASSERT_NE(m, nullptr);
  EXPECT_EQ(m->getSymbol(), 'g');
  EXPECT_EQ(m->getHp(), 50);
}

TEST(MonsterFactoryTest, CreateMonkeyBoss) {
  auto m = MonsterFactory::createMonster(MonsterType::MonkeyBoss, 0.0f, 0.0f);
  ASSERT_NE(m, nullptr);
  EXPECT_EQ(m->getSymbol(), 'M');
  EXPECT_EQ(m->getHp(), 100);
}

TEST(MonsterFactoryTest, MonsterTakesDamage) {
  auto m = MonsterFactory::createMonster(MonsterType::SceletonMonkey, 0.0f, 0.0f);
  m->takeDamage(20);
  EXPECT_EQ(m->getHp(), 10);
}

TEST(MonsterFactoryTest, MonsterDiesFromDamage) {
  auto m = MonsterFactory::createMonster(MonsterType::SceletonMonkey, 0.0f, 0.0f);
  m->takeDamage(999);
  EXPECT_TRUE(m->isDead());
  EXPECT_EQ(m->getHp(), 0);
}
