#include <gtest/gtest.h>

#include "rogue/ParticleSystem.h"

using namespace rogue;

TEST(ParticleTest, SpawnCreatesParticles) {
  ParticleSystem ps;
  EXPECT_TRUE(ps.isEmpty());
  ps.spawnBlood(5.0f, 5.0f, 20);
  EXPECT_FALSE(ps.isEmpty());
}

TEST(ParticleTest, ParticlesDieOverTime) {
  ParticleSystem ps;
  ps.spawnBlood(5.0f, 5.0f, 10);

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, ParticlesAliveShortTime) {
  ParticleSystem ps;
  ps.spawnBlood(5.0f, 5.0f, 10);
  ps.update(0.01f);
  EXPECT_FALSE(ps.isEmpty());
}

TEST(ParticleTest, SpawnZeroParticles) {
  ParticleSystem ps;
  ps.spawnBlood(5.0f, 5.0f, 0);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnExplosion) {
  ParticleSystem ps;
  ps.spawnExplosion(5.0f, 5.0f, 30);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnFire) {
  ParticleSystem ps;
  ps.spawnFire(5.0f, 5.0f, 10);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(5.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnSmoke) {
  ParticleSystem ps;
  ps.spawnSmoke(5.0f, 5.0f, 8);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, SpawnMuzzleFlash) {
  ParticleSystem ps;
  ps.spawnMuzzleFlash(5.0f, 5.0f);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(2.0f);
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, Clear) {
  ParticleSystem ps;
  ps.spawnBlood(0, 0, 20);
  ps.spawnExplosion(0, 0, 30);
  ps.clear();
  EXPECT_TRUE(ps.isEmpty());
}

TEST(ParticleTest, MultipleTypesCoexist) {
  ParticleSystem ps;
  ps.spawnBlood(0, 0, 10);
  ps.spawnExplosion(0, 0, 15);
  ps.spawnFire(0, 0, 5);
  ps.spawnSmoke(0, 0, 5);
  EXPECT_FALSE(ps.isEmpty());

  ps.update(10.0f);
  EXPECT_TRUE(ps.isEmpty());
}
