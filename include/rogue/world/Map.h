#pragma once
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
        void render() const;
        void spillBlood(int x, int y);
    private:
        int width;
        int height;
        std::vector<std::vector<Tile>> tiles; // Example tile representation
    };
} // namespace rogue
