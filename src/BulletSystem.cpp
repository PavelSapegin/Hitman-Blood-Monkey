#include "rogue/BulletSystem.h"
#include <cmath>
#include <algorithm>
#include <raylib.h>

namespace rogue {
    void BulletSystem::spawn(float x, float y, float tx, float ty, float damage)
    {
        float dx = tx - x;
        float dy = ty - y;
        float len = std::sqrt(dx * dx + dy * dy);
        if (len == 0) return;

        Bullet b;
        b.x = x;
        b.y = y;
        b.vx = (dx / len) * BULLET_SPEED;
        b.vy = (dy / len) * BULLET_SPEED;
        b.damage = damage;
        b.lifetime = 1.2f;
        bullets.push_back(b);
    }

    void BulletSystem::update(float dt, Map& map,
    std::vector<std::unique_ptr<Monster>>& monsters, ParticleSystem& particles)
    {
        for (auto& b : bullets)
        {
            if (b.dead) continue;
            b.x += b.vx * dt;
            b.y += b.vy * dt;
            b.lifetime -= dt;

            if (b.lifetime <= 0 || !map.isWalkable(b.x, b.y))
            {
                b.dead = true;
                continue;
            }

            for (auto& m: monsters)
            {
                if (m->isDead()) continue;
                float dx = m->getX() - b.x;
                float dy = m->getY() - b.y;
                float dist = std::sqrt(dx * dx + dy * dy);
                if (dist < 0.5f)
                {
                    m->takeDamage(static_cast<int>(b.damage));
                    b.dead = true;
                    particles.spawnBlood(m->getX(), m->getY(),8);
                    if (m->isDead())
                    {
                        particles.spawnBlood(m->getX(),m->getY(),25);
                        map.spillBlood(m->getX(),m->getY());
                    }
                    break;
                }
            }
        }

        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) {return b.dead;}),
            bullets.end());
    }


    void BulletSystem::render() const 
    {
        for (const auto& b : bullets)
        {
            float screenX = b.x * TILE_SIZE;
            float screenY = b.y * TILE_SIZE;
            DrawCircle((int)screenX, (int)screenY, 4, YELLOW);
            DrawCircle((int)(screenX - b.vx * 0.02f * TILE_SIZE),
        (int)(screenY - b.vy * 0.02f * TILE_SIZE),2, ORANGE);
        }
    }
} // namespace rogue
