#include "rogue/world/Map.h"
#include <gtest/gtest.h>

using namespace rogue;

TEST(MapTest, WallsAreNotWalkable) {
  Map map(10, 10);
  EXPECT_FALSE(map.isWalkable(0, 0));
  EXPECT_FALSE(map.isWalkable(9, 9));
  EXPECT_FALSE(map.isWalkable(5, 0));
  EXPECT_FALSE(map.isWalkable(0, 5));
}

TEST(MapTest, FloorIsWalkable) {
  Map map(10, 10);
  EXPECT_TRUE(map.isWalkable(1, 1));
  EXPECT_TRUE(map.isWalkable(5, 5));
  EXPECT_TRUE(map.isWalkable(8, 8));
}

TEST(MapTest, OutOfBoundsNotWalkable) {
  Map map(10, 10);
  EXPECT_FALSE(map.isWalkable(-1, 0));
  EXPECT_FALSE(map.isWalkable(0, -1));
  EXPECT_FALSE(map.isWalkable(10, 0));
  EXPECT_FALSE(map.isWalkable(0, 10));
}

TEST(MapTest, SpillBloodChangesTile) {
  Map map(10, 10);
  map.spillBlood(5, 5);
  EXPECT_EQ(map.getTile(5, 5).symbol, '%');
  EXPECT_EQ(map.getTile(5, 5).colorPair, 5);
}

TEST(MapTest, SpillBloodOutOfBoundsDoesNotCrash) {
  Map map(10, 10);
  EXPECT_NO_THROW(map.spillBlood(-1, -1));
  EXPECT_NO_THROW(map.spillBlood(100, 100));
}

TEST(MapTest, GetTileOutOfRangeThrows) {
  Map map(10, 10);
  EXPECT_THROW(map.getTile(-1, 0), std::out_of_range);
  EXPECT_THROW(map.getTile(0, -1), std::out_of_range);
  EXPECT_THROW(map.getTile(10, 0), std::out_of_range);
}

TEST(MapTest, Dimensions) {
  Map map(20, 15);
  EXPECT_EQ(map.getWidth(), 20);
  EXPECT_EQ(map.getHeight(), 15);
}
