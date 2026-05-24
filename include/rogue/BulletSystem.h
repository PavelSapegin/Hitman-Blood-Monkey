#pragma once
#include "Bullet.h"
#include "world/Map.h"
#include "entities/Monster.h"
#include "ParticleSystem.h"
#include <vector>
#include <memory>

namespace rogue
{
    class BulletSystem
    {
        public:
        void spawn(float x, float y, float tx, float ty, float damage);
        void update(
            float dt, Map& map,
            std::vector<std::unique_ptr<Monster>>& monsters,
            ParticleSystem& particles
        );
        void render() const;
        void clear() {bullets.clear();}

        private:
            std::vector<Bullet> bullets;
            static constexpr float BULLET_SPEED = 20.0f;
            static constexpr float TILE_SIZE = 40.0f;
    };
}
