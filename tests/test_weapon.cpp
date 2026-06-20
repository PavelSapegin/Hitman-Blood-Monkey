#include "rogue/Weapon.h"
#include <gtest/gtest.h>

using namespace rogue;

TEST(WeaponTest, FistsDefaults) {
  Weapon w = Weapon::makeFists();
  EXPECT_EQ(w.type, WeaponType::Fists);
  EXPECT_FLOAT_EQ(w.damage, 999.0f);
  EXPECT_FLOAT_EQ(w.range, 1.5f);
  EXPECT_FLOAT_EQ(w.cooldown, 0.3f);
  EXPECT_FLOAT_EQ(w.currentFireRate, 0.3f);
}

TEST(WeaponTest, MinigunDefaults) {
  Weapon w = Weapon::makeMinigun();
  EXPECT_EQ(w.type, WeaponType::Minigun);
  EXPECT_FLOAT_EQ(w.damage, 10.0f);
  EXPECT_FLOAT_EQ(w.range, 15.0f);
  EXPECT_FLOAT_EQ(w.currentFireRate, 0.5f);
}

TEST(WeaponTest, MinigunSpinupDecreasesFireRate) {
  Weapon w = Weapon::makeMinigun();
  float initial = w.currentFireRate;
  w.isFiring = true;

  w.currentFireRate -= w.spinupRate * 0.5f;
  EXPECT_LT(w.currentFireRate, initial);
}

TEST(WeaponTest, MinigunFireRateClamped) {
  Weapon w = Weapon::makeMinigun();
  w.currentFireRate = w.minFireRate;
  w.currentFireRate -= w.spinupRate * 1.0f;
  if (w.currentFireRate < w.minFireRate)
    w.currentFireRate = w.minFireRate;
  EXPECT_FLOAT_EQ(w.currentFireRate, w.minFireRate);
}

TEST(WeaponTest, FistsNotFiringByDefault) {
  Weapon w = Weapon::makeFists();
  EXPECT_FALSE(w.isFiring);
}
