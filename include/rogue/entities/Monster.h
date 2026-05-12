#pragma once
#include "Entity.h"

namespace rogue {

    class Monster : public Entity {
    public:
        Monster(int x, int y, char symbol, int color, int maxHp);
        void update() override; // Implement monster-specific behavior
    };

} // namespace rogue
