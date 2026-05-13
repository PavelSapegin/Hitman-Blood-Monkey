# Project Archive: Hitman-Blood-Monkey
**Path:** `/home/bober/Desktop/mywork/Hitman-Blood-Monkey`

---

### File: LICENSE
```
MIT License

Copyright (c) 2026 Pavel Sapegin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

```

---

### File: CMakeLists.txt
```
cmake_minimum_required(VERSION 3.14)
project(TestGame CXX)

# Включаем современный стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)

include_directories(include)

add_executable(testgame 
    src/main.cpp
    src/core/Engine.cpp
    src/ncurses_renderer.cpp
    include/rogue/entities/Entity.cpp
    include/rogue/entities/Monster.cpp
    include/rogue/entities/Player.cpp
    include/rogue/world/Map.cpp
    include/rogue/core/Command.cpp
    
)


target_link_libraries(testgame ${CURSES_LIBRARIES})

```

---

### File: src/main.cpp
```cpp
#include "rogue/core/Engine.h"
#include "rogue/Ncurses_renderer.h"


int main() {
    rogue::Engine engine(std::make_unique<rogue::NcursesRenderer>());
    engine.run();
    return 0;
}

```

---

### File: src/ncurses_renderer.cpp
```cpp
#include "rogue/Ncurses_renderer.h"
#include <ncurses.h>

namespace rogue
{
    void NcursesRenderer::initialize()
    {
        initscr();
        cbreak();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            init_pair(2, COLOR_RED, COLOR_BLACK);
            init_pair(3, COLOR_GREEN, COLOR_BLACK);
        }
    }

    void NcursesRenderer::clear()
    {
        ::clear();
    }

    void NcursesRenderer::refresh()
    {
        ::refresh();
    }
    void NcursesRenderer::drawChar(int x, int y, char ch,int color)
    {
        attron(COLOR_PAIR(color));
        mvaddch(y, x, ch);
        attroff(COLOR_PAIR(color));
    }
    void NcursesRenderer::shutdown()
    {
        endwin();
    }
}

```

---

### File: src/core/Engine.cpp
```cpp
#include "../include/rogue/core/Engine.h"
#include "../include/rogue/entities/MonsterFactory.h"
#include "../include/rogue/core/Command.h"
#include "../include/rogue/Exceptions.h"
#include <stdexcept>
#include <ncurses.h>
#include <algorithm>
#include <iostream>

namespace rogue {
    Engine::Engine(std::unique_ptr<IRenderer> r) : isRunning(true), map(80, 25), player(1, 1, '@', COLOR_PLAYER, 100) {
        
        if (!r) {
            throw InitializationException("Renderer cannot be null");
        }
        renderer = std::move(r);
        renderer->initialize();
       
        // Add some monsters for testing
        monsters.push_back(MonsterFactory::createMonster(MonsterType::SceletonMonkey, 5, 5));
        monsters.push_back(MonsterFactory::createMonster(MonsterType::GoblinMonkey, 10, 10));
        monsters.push_back(MonsterFactory::createMonster(MonsterType::MonkeyBoss, 15, 15));
    }

    Engine::~Engine() {
        if (renderer) {
            renderer->shutdown(); // End ncurses mode
        }
    }

    void Engine::handleInput() {
        int ch = getch();
        int dx = 0, dy = 0;
        std::unique_ptr<Command> command = nullptr;
        switch (ch) {
            case 'q':
                isRunning = false;
                break;
            case KEY_UP:
                command = std::make_unique<MoveCommand>(0, -1);
                break;
                case KEY_DOWN:
                command = std::make_unique<MoveCommand>(0, 1);
                break;
            case KEY_LEFT:
                command = std::make_unique<MoveCommand>(-1, 0);
                break;
            case KEY_RIGHT:
                command = std::make_unique<MoveCommand>(1, 0);
                break;
        }

        int targetX = player.getX() + dx;
        int targetY = player.getY() + dy;

        auto targetMonster = std::find_if(monsters.begin(), monsters.end(), [&](const std::unique_ptr<Monster>& monster) {
            return monster->getX() == targetX && monster->getY() == targetY;
        });
        if (targetMonster != monsters.end()) {
            command = std::make_unique<AttackCommand>(targetMonster->get());
        } else {
         command = std::make_unique<MoveCommand>(dx, dy);
        }
        if (command) {
            command->execute(player, map);
        }

    }


    void Engine::render()
    {
        renderer->clear();
        map.render(*renderer);
        renderer->drawChar(player.getX(), player.getY(), player.getSymbol(), player.getColor());
        for (const auto& monster : monsters) {
            renderer->drawChar(monster->getX(), monster->getY(), monster->getSymbol(), monster->getColor());
        }
        renderer->refresh();
    }

    void Engine::run()
    {
        try{
        while(isRunning)
        {
            render();
            handleInput();

            player.update();
            for (auto& monster : monsters)
            {
                monster->update();
            }

            monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [](const std::unique_ptr<Monster>& monster) {
                return monster->isDead();
            }), monsters.end());
        }
    
        }
        catch(const GameException& e)
        {
            renderer->shutdown();
            std::cout << "Game error: " << e.what() << std::endl;
        }
        catch(const std::exception& e)
        {
            renderer->shutdown();
            std::cout << "Unexpected error: " << e.what() << std::endl;
        }
    }

} // namespace rogue

```

---

### File: include/rogue/Renderer.h
```cpp
#pragma once

namespace rogue
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;
        virtual void initialize() = 0;
        virtual void shutdown() = 0;
        virtual void clear() = 0;
        virtual void refresh() = 0;
        virtual void drawChar(int x, int y, char ch,int color) = 0;
    };
}

```

---

### File: include/rogue/game.h
```cpp

```

---

### File: include/rogue/Exceptions.h
```cpp
#pragma once
#include <stdexcept>
#include <string>

namespace rogue
{
    class GameException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error; // Inherit constructors
    };

    class RendererException : public GameException
    {
    public:
        explicit RendererException(const std::string& message) : GameException(message) {}
    };

    class MapException : public GameException
    {
    public:
        explicit MapException(const std::string& message) : GameException(message) {}
    };

    class InitializationException : public GameException
    {
    public:        explicit InitializationException(const std::string& message) : GameException(message) {}
    };
}

```

---

### File: include/rogue/Ncurses_renderer.h
```cpp
#pragma once
#include "Renderer.h"
#include <ncurses.h>

namespace rogue
{
    class NcursesRenderer : public IRenderer
    {
    public:
        void initialize() override;
        void shutdown() override;
        void clear() override;
        void refresh() override;
        void drawChar(int x, int y, char ch,int color) override;
    };
}

```

---

### File: include/rogue/world/Map.h
```cpp
#pragma once
#include "../Renderer.h"
#include <vector>

namespace rogue {

    enum ColorPairs {
        COLOR_DEFAULT = 1,
        COLOR_PLAYER = 2,
        COLOR_MONSTER = 3,
        COLOR_WALL = 4,
        COLOR_BLOOD = 5

    };

    struct Tile {
        char symbol;
        int colorPair;

    };

    class Map {
    public:
        Map(int width, int height);
        ~Map() = default;

        int getWidth() const;
        int getHeight() const;

        bool isWalkable(int x, int y) const;
        void render(IRenderer& renderer) const;
        void spillBlood(int x, int y);
    private:
        int width;
        int height;
        std::vector<std::vector<Tile>> tiles; // Example tile representation
    };
} // namespace rogue

```

---

### File: include/rogue/world/Map.cpp
```cpp
#include "Map.h"
#include "../Renderer.h"
#include <ncurses.h>

namespace rogue {
Map::Map(int width, int height) : width(width), height(height) {
    // Initialize the map with default tiles (for simplicity)
    tiles.resize(height, std::vector<Tile>(width, {' ', COLOR_DEFAULT}));    

    for (int x = 0; x < width; ++x) {
        tiles[0][x] = {'#', COLOR_WALL}; // Top wall
        tiles[height - 1][x] = {'#', COLOR_WALL}; // Bottom wall
    }
    for (int y = 0; y < height; ++y) {
        tiles[y][0] = {'#', COLOR_WALL}; // Left wall
        tiles[y][width - 1] = {'#', COLOR_WALL}; // Right wall
    }
    };


bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false; // Out of bounds
    }
    return tiles[y][x].symbol != '#'; // Walkable if not a wall
}


void Map::spillBlood(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return; // Out of bounds
    }
    tiles[y][x] = {'%', COLOR_BLOOD}; // Example blood tile
}

int Map::getHeight() const {
    return height;
    } 

int Map::getWidth() const {
    return width;
}

void Map::render(IRenderer& renderer) const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Tile& tile = tiles[y][x];
            renderer.drawChar(x, y, tile.symbol, tile.colorPair);
            attroff(COLOR_PAIR(tile.colorPair));
        }
    }

}
}// namespace rogue

```

---

### File: include/rogue/core/Command.cpp
```cpp
#include "rogue/core/Command.h"

namespace rogue
{
    void MoveCommand::execute(Entity& entity, Map& map)
    {
        int newX = entity.getX() + dx;
        int newY = entity.getY() + dy;
        if (map.isWalkable(newX, newY)) {
            entity.setPosition(newX, newY);
        }
    }

    void AttackCommand::execute(Entity& entity, Map& map)
    {
        if (!target) return; // No target to attack

        // Simple attack logic: reduce target's health by a fixed amount
        int damage = 10; // Example damage value
        target->takeDamage(damage);
        if (target->isDead()) {
            map.spillBlood(target->getX(), target->getY());
        }
    }
}

```

---

### File: include/rogue/core/Engine.h
```cpp
#pragma once
#include <vector>
#include <memory>
#include "../Renderer.h"
#include "../world/Map.h"
#include "../entities/Player.h"
#include "../entities/Monster.h"

namespace rogue {

    class Engine {
    public:
        Engine(std::unique_ptr<IRenderer> renderer);
        ~Engine();

        void run(); // Main game loop
        
    private:
    
    std::unique_ptr<IRenderer> renderer; // Using unique_ptr for automatic memory management
    void handleInput();
    void render();

        bool isRunning;
        Map map;
        Player player;
        std::vector<std::unique_ptr<Monster>> monsters; // Using unique_ptr for automatic memory management
    };

} // namespace rogue

```

---

### File: include/rogue/core/Command.h
```cpp
#pragma once
#include "rogue/entities/Entity.h"
#include "rogue/world/Map.h"
#include "rogue/entities/Monster.h"

namespace rogue
{
    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void execute(Entity& entity, Map& map) = 0;
    };

    class MoveCommand : public Command
    {
        int dx, dy;
        public:
        MoveCommand(int dx, int dy) : dx(dx), dy(dy) {}
        void execute(Entity& entity, Map& map) override;
    };

    class AttackCommand: public Command
    {

        Monster* target;
        public:
        AttackCommand(Monster* target) : target(target) {}
        void execute(Entity& entity, Map& map) override;
    };
}

```

---

### File: include/rogue/entities/Entity.cpp
```cpp
#include "Entity.h"

namespace rogue {

    Entity::Entity(int x, int y, char symbol, int color, int maxHp) : x(x), y(y), symbol(symbol), color(color), hp(maxHp), maxHp(maxHp) {}

    int Entity::getX() const {
        return x;
    }

    int Entity::getY() const {
        return y;
    }

    char Entity::getSymbol() const {
        return symbol;
    }

    int Entity::getColor() const {
        return color;
    }

    int Entity::getHp() const {
        return hp;
    }

    bool Entity::isDead() const {
        return hp <= 0;
    }

    void Entity::takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) {
            hp = 0;
        }
    }

    void Entity::setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    void Entity::move(int dx, int dy) {
        x += dx;
        y += dy;
    }

} // namespace rogue

```

---

### File: include/rogue/entities/MonsterFactory.h
```cpp
#pragma once
#include "Monster.h"
#include <memory>

namespace rogue
{
    enum class MonsterType
    {
        SceletonMonkey,
        GoblinMonkey,
        MonkeyBoss
    };

    class MonsterFactory
    {
        public:
        static std::unique_ptr<Monster> createMonster(MonsterType type, int x, int y)
        {
            switch (type)
            {
                case MonsterType::SceletonMonkey:
                    return std::make_unique<Monster>(x, y, 's', COLOR_MONSTER, 30);
                case MonsterType::GoblinMonkey:
                    return std::make_unique<Monster>(x, y, 'g', COLOR_MONSTER, 50);
                case MonsterType::MonkeyBoss:
                    return std::make_unique<Monster>(x, y, 'M', COLOR_MONSTER, 100);
                default:
                    return nullptr;
            }
        }
    };
}

```

---

### File: include/rogue/entities/Player.h
```cpp
#pragma once
#include "Entity.h"

namespace rogue {

    class Player : public Entity {
    public:
        Player(int x, int y, char symbol,int color, int maxHp);
        void update() override; // Implement player-specific behavior
        void move(int dx, int dy);
    };
} // namespace rogue

```

---

### File: include/rogue/entities/Entity.h
```cpp
#pragma once

namespace rogue {

    class Entity {
    public:
        Entity(int x, int y, char symbol, int color, int maxHp);
        virtual ~Entity()=default;

        int getX() const;
        int getY() const;
        char getSymbol() const;
        int getColor() const;
        int getHp() const;
        bool isDead() const;
        void takeDamage(int damage);
        void setPosition(int newX, int newY);
        virtual void update() = 0; // Pure virtual function for updating entity state
        void move(int dx, int dy);
    protected:
        int x;
        int y;
        char symbol;
        int color;
        int hp;
        int maxHp;
    };

} // namespace rogue

```

---

### File: include/rogue/entities/Monster.cpp
```cpp
#include "Monster.h"

namespace rogue {

    Monster::Monster(int x, int y, char symbol, int color, int maxHp) : Entity(x, y, symbol, color, maxHp) {}

    void Monster::update() {
        // Implement simple monster behavior (e.g., random movement)
        // For now, this is just a placeholder
    }

} // namespace rogue


```

---

### File: include/rogue/entities/Player.cpp
```cpp
#include "Player.h"

namespace rogue {

    Player::Player(int x, int y, char symbol, int color, int maxHp) : Entity(x, y, symbol, color, maxHp) {}

    void Player::update() {
        // Implement player-specific behavior (e.g., handle input)
    }

    void Player::move(int dx, int dy) {
       x += dx;
       y += dy;
    }

} // namespace rogue

```

---

### File: include/rogue/entities/Monster.h
```cpp
#pragma once
#include "Entity.h"

namespace rogue {

    class Monster : public Entity {
    public:
        Monster(int x, int y, char symbol, int color, int maxHp);
        void update() override; // Implement monster-specific behavior
    };

} // namespace rogue

```

---

