#include <gtest/gtest.h>

#include "rogue/core/Command.h"
#include "rogue/entities/Monster.h"
#include "rogue/world/Map.h"

using namespace rogue;

class CommandTest : public ::testing::Test {
protected:
  Map map{40, 40, 42};
  Monster monster{(float)map.getRooms()[0].centerX(), (float)map.getRooms()[0].centerY(), 's', 3,
                  30};
};

TEST_F(CommandTest, MoveCommandMovesEntity) {
  float startX = monster.getX();
  float startY = monster.getY();
  int dx = 0, dy = 0;
  if (map.isWalkable(startX + 1, startY))
    dx = 1;
  else if (map.isWalkable(startX - 1, startY))
    dx = -1;
  else if (map.isWalkable(startX, startY + 1))
    dy = 1;
  else if (map.isWalkable(startX, startY - 1))
    dy = -1;
  ASSERT_TRUE(dx != 0 || dy != 0) << "No walkable direction from spawn";
  MoveCommand cmd(dx, dy);
  cmd.execute(monster, map);
  EXPECT_FLOAT_EQ(monster.getX(), startX + dx);
  EXPECT_FLOAT_EQ(monster.getY(), startY + dy);
}

TEST_F(CommandTest, MoveCommandBlockedByWall) {
  // Place a wall to the left of the monster
  Monster m{1.0f, 1.0f, 's', 3, 30};
  MoveCommand cmd(-1, 0);  // Attempt to move left into the wall
  cmd.execute(m, map);
  EXPECT_FLOAT_EQ(m.getX(), 1.0f);  // Position should not change
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
