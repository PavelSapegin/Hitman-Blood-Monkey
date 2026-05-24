#include "rogue/ParticleSystem.h"
#include <gtest/gtest.h>

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

  // Simulate enough time passing for all particles to die
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
