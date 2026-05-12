#pragma once
#include <vector>
#include <memory>
#include "../world/Map.h"
#include "../entities/Player.h"
#include "../entities/Monster.h"

namespace rogue {

    class Engine {
    public:
        Engine(int mapWidth, int mapHeight);
        ~Engine();

        void run(); // Main game loop
        
    private:

    void handleInput();
    void render();

        bool isRunning;
        Map map;
        Player player;
        std::vector<std::unique_ptr<Monster>> monsters; // Using unique_ptr for automatic memory management
    };

} // namespace rogue
