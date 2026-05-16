#include "rogue/ParticleSystem.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

namespace rogue {

static const char BLOOD_SYMBOLS[] = {'%', '*', 'x', 'o', '#'};
static const Color BLOOD_COLORS[] = {RED, MAROON, DARKGRAY, RED, MAROON};
static constexpr float TILE_SIZE = 40.0f;

void ParticleSystem::spawnBlood(float x, float y, int count) {
    for (int i = 0; i < count; ++i) {
        float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
        float speed = 1.5f + ((float)rand() / (float)RAND_MAX) * 5.0f;
        

        Particle p;
        p.x = x;
        p.y = y;
        p.vx = std::cos(angle) * speed;
        p.vy = std::sin(angle) * speed;
        p.lifetime = 0.4f + ((float)rand() / (float)RAND_MAX) * 0.8f;
        p.maxLifetime = p.lifetime;
        p.symbol = BLOOD_SYMBOLS[rand() % 5];
        p.color = BLOOD_COLORS[rand() % 5];

        particles.push_back(p);
    }
}

void ParticleSystem::update(float deltaTime) {
    for (auto& p : particles) {
        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;

        // Гравитация и затухание скорости
        p.vy += 2.0f * deltaTime;
        p.vx *= 0.95f;

        p.lifetime -= deltaTime;
    }

    // Удаляем мёртвые частицы
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const rogue::Particle& p) { return p.lifetime <= 0; }),
        particles.end());
}

void ParticleSystem::render() const {
    for (const auto& p : particles) {
        // Прозрачность зависит от оставшегося времени жизни
        float alpha = p.lifetime / p.maxLifetime;
        Color c = p.color;
        c.a = static_cast<unsigned char>(alpha * 255);

        float screenX = p.x * TILE_SIZE;
        float screenY = p.y * TILE_SIZE;

        DrawText(TextFormat("%c", p.symbol), 
                 (int)screenX, (int)screenY, 20, c);
    }
}

} // namespace rogue
