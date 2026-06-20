#pragma once

namespace rogue {

enum class WeaponType { Fists, Minigun, Shotgun, RocketLauncher };

struct Weapon {
  WeaponType type;

  float cooldown = 0.0f;
  float damage = 0.0f;
  float range = 0.0f;

  float currentFireRate = 0.0f;
  float minFireRate = 0.05f;
  float maxFireRate = 0.5f;
  float spinupRate = 0.4f;
  bool isFiring = false;

  static Weapon makeFists() {
    Weapon w;
    w.type = WeaponType::Fists;
    w.damage = 999.0f;
    w.range = 1.5f;
    w.cooldown = 0.3f;
    w.currentFireRate = 0.3f;
    return w;
  }

  static Weapon makeMinigun() {
    Weapon w;
    w.type = WeaponType::Minigun;
    w.damage = 10.0f;
    w.range = 15.0f;
    w.currentFireRate = 0.5f;
    return w;
  }

  static Weapon makeShotgun() {
    Weapon w;
    w.type = WeaponType::Shotgun;
    w.damage = 25.0f;
    w.range = 8.0f;
    w.cooldown = 0.8f;
    w.currentFireRate = 0.8f;
    return w;
  }

  static Weapon makeRocketLauncher() {
    Weapon w;
    w.type = WeaponType::RocketLauncher;
    w.damage = 80.0f;
    w.range = 20.0f;
    w.cooldown = 1.5f;
    w.currentFireRate = 1.5f;
    return w;
  }
};

}  // namespace rogue
