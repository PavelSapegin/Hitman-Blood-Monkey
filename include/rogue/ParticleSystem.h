#pragma once
#include <vector>
#include <raylib.h>

namespace rogue {

struct Particle {
    float x, y;
    float vx, vy;       // скорость
    float lifetime;     // сколько осталось жить
    float maxLifetime;
    char symbol;        // '%', '*', 'x', 'o'
    Color color;
};

class ParticleSystem {
public:
    void spawnBlood(float x, float y, int count = 20);
    void update(float deltaTime);
    void render() const;
    bool isEmpty() const { return particles.empty(); }

private:
    std::vector<Particle> particles;
};

} // namespace rogue
