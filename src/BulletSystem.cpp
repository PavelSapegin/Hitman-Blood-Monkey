#include "rogue/BulletSystem.h"

#include <raylib.h>

#include <algorithm>
#include <cmath>

namespace rogue {
void BulletSystem::spawn(float x, float y, float tx, float ty, float damage, bool isRocket) {
  float dx = tx - x;
  float dy = ty - y;
  float len = std::sqrt(dx * dx + dy * dy);
  if (len == 0)
    return;

  Bullet b;
  b.x = x;
  b.y = y;
  b.vx = (dx / len) * BULLET_SPEED;
  b.vy = (dy / len) * BULLET_SPEED;
  b.damage = damage;
  b.lifetime = 1.2f;
  b.isRocket = isRocket;
  if (piercingMod)
    b.pierceLeft = 3;
  if (ricochetMod)
    b.bounceLeft = 2;
  bullets.push_back(b);
}

void BulletSystem::update(float dt, Map &map, std::vector<std::unique_ptr<Monster>> &monsters,
                          ParticleSystem &particles) {
  rocketExplosions.clear();

  for (auto &b : bullets) {
    if (b.dead)
      continue;
    b.x += b.vx * dt;
    b.y += b.vy * dt;
    b.lifetime -= dt;

    if (b.lifetime <= 0) {
      b.dead = true;
      continue;
    }

    if (!map.isWalkable(b.x, b.y)) {
      if (b.bounceLeft > 0) {
        --b.bounceLeft;
        if (map.isWalkable(b.x - b.vx * dt * 2, b.y + b.vy * dt * 2)) {
          b.vy = -b.vy;
        } else if (map.isWalkable(b.x + b.vx * dt * 2, b.y - b.vy * dt * 2)) {
          b.vx = -b.vx;
        } else {
          b.vx = -b.vx;
          b.vy = -b.vy;
        }
        particles.spawnFire(b.x, b.y, 3);
        continue;
      }
      particles.spawnFire(b.x, b.y, 5);
      b.dead = true;
      if (b.isRocket) {
        rocketExplosions.push_back({b.x, b.y});
      }
      if (explosiveMod && onExplosion) {
        onExplosion(b.x, b.y, explosiveRadius, explosiveDamage);
      }
      continue;
    }

    for (auto &m : monsters) {
      if (m->isDead())
        continue;
      float dx = m->getX() - b.x;
      float dy = m->getY() - b.y;
      float dist = std::sqrt(dx * dx + dy * dy);
      if (dist < 0.5f) {
        m->takeDamage(static_cast<int>(b.damage));
        m->onHit();
        particles.spawnBlood(m->getX(), m->getY(), 15);
        if (explosiveMod && onExplosion) {
          onExplosion(b.x, b.y, explosiveRadius, explosiveDamage);
        }
        if (b.pierceLeft > 0) {
          --b.pierceLeft;
        } else {
          b.dead = true;
        }
        if (b.isRocket) {
          rocketExplosions.push_back({b.x, b.y});
          b.dead = true;
        }
        if (m->isDead()) {
          particles.spawnBlood(m->getX(), m->getY(), 40);
          particles.spawnExplosion(m->getX(), m->getY(), 30);
          particles.spawnSmoke(m->getX(), m->getY(), 12);
          map.spillBloodArea((int)m->getX(), (int)m->getY(), 3);
          if (onMonsterKilled) {
            onMonsterKilled(m->getX(), m->getY());
          }
        } else {
          map.spillBloodArea((int)m->getX(), (int)m->getY(), 1);
        }
        break;
      }
    }
  }

  bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(), [](const Bullet &b) { return b.dead; }),
    bullets.end());
}

std::vector<std::pair<float, float>> BulletSystem::getRocketExplosions() {
  auto result = std::move(rocketExplosions);
  rocketExplosions.clear();
  return result;
}

void BulletSystem::clear() {
  bullets.clear();
  rocketExplosions.clear();
}

void BulletSystem::render() const {
  for (const auto &b : bullets) {
    float screenX = b.x * TILE_SIZE;
    float screenY = b.y * TILE_SIZE;
    DrawCircle((int)screenX, (int)screenY, 4, YELLOW);
    DrawCircle((int)(screenX - b.vx * 0.02f * TILE_SIZE), (int)(screenY - b.vy * 0.02f * TILE_SIZE),
               2, ORANGE);
    if (b.isRocket) {
      DrawCircle((int)screenX, (int)screenY, 7, Color{255, 100, 0, 100});
    }
    if (b.pierceLeft > 0) {
      DrawCircle((int)screenX, (int)screenY, 6, Color{100, 100, 255, 80});
    }
  }
}
}  // namespace rogue
