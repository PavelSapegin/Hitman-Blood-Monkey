#include "rogue/core/Command.h"
#include "rogue/entities/Monster.h"
#include "rogue/world/Map.h"
#include <gtest/gtest.h>

using namespace rogue;

class CommandTest : public ::testing::Test {
protected:
  Map map{40, 40, 42};
  Monster monster{(float)map.getRooms()[0].centerX(),
                  (float)map.getRooms()[0].centerY(), 's', 3, 30};
};

TEST_F(CommandTest, MoveCommandMovesEntity) {
  float startX = monster.getX();
  MoveCommand cmd(1, 0);
  cmd.execute(monster, map);
  EXPECT_FLOAT_EQ(monster.getX(), startX + 1.0f);
}

TEST_F(CommandTest, MoveCommandBlockedByWall) {
  // Place a wall to the left of the monster
  Monster m{1.0f, 1.0f, 's', 3, 30};
  MoveCommand cmd(-1, 0); // Attempt to move left into the wall
  cmd.execute(m, map);
  EXPECT_FLOAT_EQ(m.getX(), 1.0f); // Position should not change
}

TEST_F(CommandTest, AttackCommandDamagesTarget) {
  Monster target{6.0f, 5.0f, 'g', 3, 50};
  AttackCommand cmd(&target);
  cmd.execute(monster, map);
  EXPECT_EQ(target.getHp(), 40);
}

TEST_F(CommandTest, AttackCommandKillsTarget) {
  Monster target{6.0f, 5.0f, 'g', 3, 10};
  AttackCommand cmd(&target);
  cmd.execute(monster, map);
  EXPECT_TRUE(target.isDead());
}

TEST_F(CommandTest, AttackCommandNullTargetDoesNotCrash) {
  AttackCommand cmd(nullptr);
  EXPECT_NO_THROW(cmd.execute(monster, map));
}
