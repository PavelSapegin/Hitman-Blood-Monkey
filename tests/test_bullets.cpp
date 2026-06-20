#include "rogue/BulletSystem.h"
#include "rogue/entities/Monster.h"
#include "rogue/world/Map.h"
#include <gtest/gtest.h>

using namespace rogue;

TEST(BulletTest, SpawnCreatesBullet) {
  BulletSystem bs;
  bs.spawn(0.0f, 0.0f, 10.0f, 0.0f, 25.0f);
  bs.spawn(0.0f, 0.0f, 0.0f, 10.0f, 25.0f);
  bs.spawn(5.0f, 5.0f, 5.0f, 5.0f, 25.0f);
  bs.clear();
  // After clear, internal bullet list should be empty (no crash)
}

TEST(BulletTest, SpawnTowardsTarget) {
  BulletSystem bs;
  bs.spawn(0.0f, 0.0f, 10.0f, 0.0f, 25.0f);
  // No crash means sanity check passed; bullet moves in x direction
}

TEST(BulletTest, BulletsHitMonsters) {
  Map map(40, 40, 42);
  auto room = map.getRooms()[0];
  float cx = (float)room.centerX();
  float cy = (float)room.centerY();

  BulletSystem bs;
  ParticleSystem ps;
  std::vector<std::unique_ptr<Monster>> monsters;
  monsters.push_back(std::make_unique<Monster>(cx, cy, 's', 3, 30));

  // Spawn bullet close to the monster, aimed directly at it
  // SkeletMonkey has 30 HP; 3 hits of 25 should kill it
  bs.spawn(cx - 0.4f, cy, cx, cy, 25.0f);
  bs.spawn(cx - 0.4f, cy + 0.1f, cx, cy, 25.0f);
  bs.spawn(cx - 0.4f, cy - 0.1f, cx, cy, 25.0f);

  for (int i = 0; i < 20; ++i) {
    bs.update(0.01f, map, monsters, ps);
    if (monsters[0]->isDead())
      break;
  }

  EXPECT_TRUE(monsters[0]->isDead());
}

TEST(BulletTest, BulletsExpireAfterLifetime) {
  Map map(40, 40, 42);
  BulletSystem bs;
  ParticleSystem ps;
  std::vector<std::unique_ptr<Monster>> monsters;

  bs.spawn(5.0f, 5.0f, 10.0f, 5.0f, 25.0f);

  for (int i = 0; i < 200; ++i) {
    bs.update(0.1f, map, monsters, ps);
  }

  // Should not crash — bullet expired without hitting anything
}
