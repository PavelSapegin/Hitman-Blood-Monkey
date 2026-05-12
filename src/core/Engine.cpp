#include "../include/rogue/core/Engine.h"
#include <ncurses.h>
#include <algorithm>

namespace rogue {
    Engine::Engine(int mapWidth, int mapHeight) : isRunning(true), map(mapWidth, mapHeight), player(1, 1, '@', COLOR_PLAYER, 100) {
        // Initialize ncurses
        initscr();
        cbreak();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        start_color();
        init_pair(COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_PLAYER, COLOR_CYAN, COLOR_BLACK);
        init_pair(COLOR_MONSTER, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_WALL, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_BLOOD, COLOR_MAGENTA, COLOR_BLACK);

        // Add some monsters for testing
        monsters.push_back(std::make_unique<Monster>(5, 5, 'M', COLOR_MONSTER, 50));
        monsters.push_back(std::make_unique<Monster>(10, 10, 'M', COLOR_MONSTER, 50));
    }

    Engine::~Engine() {
        endwin(); // End ncurses mode
    }

    void Engine::handleInput() {
        int ch = getch();
        int dx = 0, dy = 0;
        switch (ch) {
            case 'q':
                isRunning = false;
                return;
            case KEY_UP:
                dx = 0;
                dy = -1;
                break;
            case KEY_DOWN:
                dx = 0;
                dy = 1;
                break;
            case KEY_LEFT:
                dx = -1;
                dy = 0;
                break;
            case KEY_RIGHT:
                dx = 1;
                dy = 0;
                break;
        }

        if (dx != 0 || dy != 0) {
            int newX = player.getX() + dx;
            int newY = player.getY() + dy;
            
            auto it = std::find_if(monsters.begin(), monsters.end(), [&](const std::unique_ptr<Monster>& monster) {
                return monster->getX() == newX && monster->getY() == newY;
            });

            if (it != monsters.end()) {
                (*it)->takeDamage(10); // Example damage value
                if ((*it)->isDead()) {
                    map.spillBlood((*it)->getX(), (*it)->getY());
                    monsters.erase(it);
                }
            } else if (map.isWalkable(newX, newY)) {
                player.move(dx, dy);
            }
        }


    }


    void Engine::render()
    {
        clear();
        map.render();
        for (const auto& entity: monsters)
        {
            attron(COLOR_PAIR(entity->getColor()));
            mvaddch(entity->getY(), entity->getX(), entity->getSymbol());
            attroff(COLOR_PAIR(entity->getColor()));   
        }

        attron(COLOR_PAIR(player.getColor()));
        mvaddch(player.getY(), player.getX(), player.getSymbol());
        attroff(COLOR_PAIR(player.getColor()));
        refresh();
    }

    void Engine::run()
    {
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

} // namespace rogue
