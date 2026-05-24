#include "rogue/entities/Player.h"
#include "rogue/world/Map.h"
#include <cmath>
#include <gtest/gtest.h>

using namespace rogue;

class PlayerTest : public ::testing::Test {
protected:
  Map map{40, 40, 42}; // фиксированный seed
  Player player{(float)map.getRooms()[0].centerX(),
                (float)map.getRooms()[0].centerY(), '@', 2, 100};
};

TEST_F(PlayerTest, MovesOnOpenFloor) {
  std::vector<Entity *> entities;
  float startX = player.getX();
  player.setContext(map, entities, 1.0f);
  player.processInput(1.0f, 0.0f, 0.016f);
  player.update();
  EXPECT_GT(player.getX(), startX);
}

TEST_F(PlayerTest, InitialPosition) {
  EXPECT_FLOAT_EQ(player.getX(), (float)map.getRooms()[0].centerX());
  EXPECT_FLOAT_EQ(player.getY(), (float)map.getRooms()[0].centerY());
}

TEST_F(PlayerTest, InitialHp) {
  EXPECT_EQ(player.getHp(), 100);
  EXPECT_FALSE(player.isDead());
}

TEST_F(PlayerTest, TakeDamage) {
  player.takeDamage(30);
  EXPECT_EQ(player.getHp(), 70);
}

TEST_F(PlayerTest, TakeDamageDoesNotGoBelowZero) {
  player.takeDamage(999);
  EXPECT_EQ(player.getHp(), 0);
  EXPECT_TRUE(player.isDead());
}

TEST_F(PlayerTest, BlockedByWall) {
  // Place player near the top wall
  Player p{5.0f, 1.0f, '@', 2, 100};
  std::vector<Entity *> entities;
  p.setContext(map, entities, 1.0f);
  p.processInput(0.0f, -1.0f, 0.016f); // Attempt to move up into the wall
  p.update();
  EXPECT_GE(p.getY(), 0.5f); // Should not move into the wall
}

TEST_F(PlayerTest, DiagonalMovementNormalized) {
  std::vector<Entity *> entities;
  player.setContext(map, entities, 1.0f);

  // Create another player to compare diagonal speed
  Player p1{5.0f, 5.0f, '@', 2, 100};
  Player p2{5.0f, 5.0f, '@', 2, 100};

  p1.setContext(map, entities, 0.016f);
  p2.setContext(map, entities, 0.016f);

  p1.processInput(1.0f, 0.0f, 0.016f);
  p2.processInput(1.0f, 1.0f, 0.016f);

  p1.update();
  p2.update();

  float distX = p1.getX() - 5.0f;
  float distDiag = std::sqrt((p2.getX() - 5.0f) * (p2.getX() - 5.0f) +
                             (p2.getY() - 5.0f) * (p2.getY() - 5.0f));

  // Diagonal distance should be approximately the same as straight distance due
  // to normalization
  EXPECT_NEAR(distX, distDiag, 0.001f);
}
