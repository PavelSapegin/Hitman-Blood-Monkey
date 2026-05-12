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
