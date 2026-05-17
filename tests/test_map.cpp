#include <gtest/gtest.h>
#include "rogue/world/Map.h"

using namespace rogue;

class MapTest : public ::testing::Test {
protected:
    // Фиксированный seed — карта всегда одинаковая
    Map map{80, 50, 42};
};

TEST_F(MapTest, Dimensions) {
    EXPECT_EQ(map.getWidth(), 80);
    EXPECT_EQ(map.getHeight(), 50);
}

TEST_F(MapTest, OutOfBoundsNotWalkable) {
    EXPECT_FALSE(map.isWalkable(-1, 0));
    EXPECT_FALSE(map.isWalkable(0, -1));
    EXPECT_FALSE(map.isWalkable(80, 0));
    EXPECT_FALSE(map.isWalkable(0, 50));
}

TEST_F(MapTest, GetTileOutOfRangeThrows) {
    EXPECT_THROW(map.getTile(-1, 0), std::out_of_range);
    EXPECT_THROW(map.getTile(0, -1), std::out_of_range);
    EXPECT_THROW(map.getTile(80, 0), std::out_of_range);
}

TEST_F(MapTest, RoomsGenerated) {
    // После генерации должны быть комнаты
    EXPECT_FALSE(map.getRooms().empty());
}

TEST_F(MapTest, RoomCentersAreWalkable) {
    // Центры всех комнат должны быть проходимы
    for (const auto& room : map.getRooms()) {
        EXPECT_TRUE(map.isWalkable(room.centerX(), room.centerY()))
            << "Room center " << room.centerX() << "," << room.centerY() 
            << " is not walkable";
    }
}

TEST_F(MapTest, SpillBloodChangesTile) {
    // Берём центр первой комнаты — гарантированно walkable
    const auto& room = map.getRooms()[0];
    int cx = room.centerX();
    int cy = room.centerY();
    map.spillBlood(cx, cy);
    EXPECT_EQ(map.getTile(cx, cy).symbol, '%');
    EXPECT_EQ(map.getTile(cx, cy).colorPair, 5);
}

TEST_F(MapTest, SpillBloodOutOfBoundsDoesNotCrash) {
    EXPECT_NO_THROW(map.spillBlood(-1, -1));
    EXPECT_NO_THROW(map.spillBlood(100, 100));
}
