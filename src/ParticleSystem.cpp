#include "rogue/Constants.h"
#include "rogue/ParticleSystem.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace rogue {

static float randF(float min, float max) {
  return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

void ParticleSystem::spawnBlood(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * PI;
    float speed = 2.0f + ((float)rand() / (float)RAND_MAX) * 8.0f;

    Particle p;
    p.x = x + randF(-0.2f, 0.2f);
    p.y = y + randF(-0.2f, 0.2f);
    p.vx = std::cos(angle) * speed;
    p.vy = std::sin(angle) * speed;
    p.lifetime = randF(2.0f, 5.0f);
    p.maxLifetime = p.lifetime;
    p.size = randF(2.0f, 7.0f);
    p.maxSize = p.size;
    p.type = ParticleType::Blood;

    static const Color bloodColors[] = {
        RED,         MAROON,   Color{(unsigned char)180, (unsigned char)0, (unsigned char)0, (unsigned char)255},
        Color{(unsigned char)200, (unsigned char)50, (unsigned char)50, (unsigned char)255}, Color{(unsigned char)120, (unsigned char)0, (unsigned char)0, (unsigned char)255}};
    p.color = bloodColors[rand() % 5];

    particles.push_back(p);
  }
}

void ParticleSystem::spawnExplosion(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    float angle = randF(0, 2.0f * PI);
    float speed = randF(3.0f, 12.0f);
    float t = (float)i / (float)count;

    Particle p;
    p.x = x + randF(-0.3f, 0.3f);
    p.y = y + randF(-0.3f, 0.3f);
    p.vx = std::cos(angle) * speed;
    p.vy = std::sin(angle) * speed;

    if (t < 0.4f) {
      p.type = ParticleType::ExplosionDebris;
      p.lifetime = randF(0.5f, 1.5f);
      p.size = randF(2.0f, 4.0f);
      p.color = Color{(unsigned char)(80 + rand() % 40), (unsigned char)(30 + rand() % 20), (unsigned char)10, (unsigned char)255};
    } else if (t < 0.7f) {
      p.type = ParticleType::Fire;
      p.lifetime = randF(0.3f, 0.8f);
      p.size = randF(3.0f, 8.0f);
      p.color = (rand() % 2) ? ORANGE : YELLOW;
    } else {
      p.type = ParticleType::Smoke;
      p.lifetime = randF(2.0f, 4.0f);
      p.size = randF(3.0f, 6.0f);
      p.maxSize = p.size + randF(5.0f, 12.0f);
      p.color = Color{(unsigned char)(100 + rand() % 60), (unsigned char)(100 + rand() % 60), (unsigned char)(100 + rand() % 60),
                      (unsigned char)100};
    }

    p.maxLifetime = p.lifetime;
    particles.push_back(p);
  }
}

void ParticleSystem::spawnFire(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    Particle p;
    p.x = x + randF(-0.2f, 0.2f);
    p.y = y + randF(-0.2f, 0.2f);
    p.vx = randF(-2.0f, 2.0f);
    p.vy = -randF(1.0f, 5.0f);
    p.lifetime = randF(0.3f, 0.8f);
    p.maxLifetime = p.lifetime;
    p.size = randF(2.0f, 6.0f);
    p.maxSize = p.size;
    p.color = (rand() % 3 == 0) ? YELLOW : ORANGE;
    p.type = ParticleType::Fire;
    particles.push_back(p);
  }
}

void ParticleSystem::spawnSmoke(float x, float y, int count) {
  for (int i = 0; i < count; ++i) {
    Particle p;
    p.x = x + randF(-0.3f, 0.3f);
    p.y = y + randF(-0.3f, 0.3f);
    p.vx = randF(-1.5f, 1.5f);
    p.vy = -randF(0.5f, 2.5f);
    p.lifetime = randF(2.5f, 5.0f);
    p.maxLifetime = p.lifetime;
    p.size = randF(3.0f, 6.0f);
    p.maxSize = p.size + randF(6.0f, 14.0f);
    int v = 80 + rand() % 80;
    p.color = Color{(unsigned char)v, (unsigned char)v, (unsigned char)v, (unsigned char)120};
    p.type = ParticleType::Smoke;
    particles.push_back(p);
  }
}

void ParticleSystem::spawnMuzzleFlash(float x, float y) {
  for (int i = 0; i < 6; ++i) {
    Particle p;
    p.x = x + randF(-0.1f, 0.1f);
    p.y = y + randF(-0.1f, 0.1f);
    float angle = randF(0, 2.0f * PI);
    float speed = randF(1.0f, 4.0f);
    p.vx = std::cos(angle) * speed;
    p.vy = std::sin(angle) * speed;
    p.lifetime = randF(0.05f, 0.2f);
    p.maxLifetime = p.lifetime;
    p.size = randF(2.0f, 5.0f);
    p.maxSize = p.size;
    p.color = (rand() % 2) ? YELLOW : ORANGE;
    p.type = ParticleType::Fire;
    particles.push_back(p);
  }
}

void ParticleSystem::update(float deltaTime) {
  for (auto &p : particles) {
    p.x += p.vx * deltaTime;
    p.y += p.vy * deltaTime;
    p.lifetime -= deltaTime;

    switch (p.type) {
    case ParticleType::Blood:
      p.vy += 0.5f * deltaTime;
      p.vx *= 0.94f;
      break;
    case ParticleType::Fire:
      p.vy -= 0.8f * deltaTime;
      p.vx *= 0.95f;
      break;
    case ParticleType::Smoke:
      p.vy -= 0.2f * deltaTime;
      p.vx *= 0.98f;
      p.size += 2.0f * deltaTime;
      if (p.size > p.maxSize)
        p.size = p.maxSize;
      break;
    case ParticleType::ExplosionDebris:
      p.vy += 3.0f * deltaTime;
      p.vx *= 0.88f;
      break;
    }
  }

  particles.erase(
      std::remove_if(particles.begin(), particles.end(),
                     [](const Particle &p) { return p.lifetime <= 0; }),
      particles.end());
}

void ParticleSystem::render() const {
  for (const auto &p : particles) {
    float alpha = p.lifetime / p.maxLifetime;
    Color c = p.color;
    c.a = static_cast<unsigned char>(alpha * c.a);

    float screenX = p.x * TILE_SIZE;
    float screenY = p.y * TILE_SIZE;

    switch (p.type) {
    case ParticleType::Blood:
      DrawCircle((int)screenX, (int)screenY, p.size * alpha, c);
      break;
    case ParticleType::Fire: {
      float flicker = 0.7f + 0.3f * std::sin(p.lifetime * 30.0f);
      DrawCircle((int)screenX, (int)screenY, p.size * flicker, c);
      break;
    }
    case ParticleType::Smoke:
      c.a = static_cast<unsigned char>((1.0f - (1.0f - alpha) * (1.0f - alpha)) * c.a);
      DrawCircle((int)screenX, (int)screenY, p.size, c);
      break;
    case ParticleType::ExplosionDebris:
      DrawCircle((int)screenX, (int)screenY, p.size * (0.5f + 0.5f * alpha), c);
      break;
    }
  }
}

} // namespace rogue
