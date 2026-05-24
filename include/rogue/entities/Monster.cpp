#include "Monster.h"
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <queue>

namespace rogue {

Monster::Monster(float x, float y, char symbol, int color, int maxHp)
    : Entity(x, y, symbol, color, maxHp) {}

void Monster::update() {}

// Эвристика — манхэттенское расстояние
static float heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<std::pair<int,int>> Monster::findPath(
    const Map& map, int startX, int startY, int goalX, int goalY)
{
    // Ключ для unordered_map
    auto encode = [&](int x, int y) { return y * map.getWidth() + x; };

    struct ANode {
        int x, y;
        float f;
        bool operator>(const ANode& o) const { return f > o.f; }
    };

    std::priority_queue<ANode, std::vector<ANode>, std::greater<ANode>> open;
    std::unordered_map<int, float> gScore;
    std::unordered_map<int, int> parent; // encode(x,y) -> encode(parentX,parentY)

    int startKey = encode(startX, startY);
    gScore[startKey] = 0;
    open.push({startX, startY, heuristic(startX, startY, goalX, goalY)});
    parent[startKey] = -1;

    const int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    while (!open.empty()) {
        auto cur = open.top(); open.pop();
        int cx = cur.x, cy = cur.y;

        if (cx == goalX && cy == goalY) {
            // Восстанавливаем путь
            std::vector<std::pair<int,int>> path;
            int key = encode(goalX, goalY);
            while (key != -1) {
                int px = key % map.getWidth();
                int py = key / map.getWidth();
                path.push_back({px, py});
                key = parent.count(key) ? parent[key] : -1;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        float curG = gScore[encode(cx, cy)];
        for (auto& d : dirs) {
            int nx = cx + d[0];
            int ny = cy + d[1];
            if (!map.isWalkable(nx, ny)) continue;

            int nkey = encode(nx, ny);
            float ng = curG + 1.0f;
            if (!gScore.count(nkey) || ng < gScore[nkey]) {
                gScore[nkey] = ng;
                parent[nkey] = encode(cx, cy);
                open.push({nx, ny, ng + heuristic(nx, ny, goalX, goalY)});
            }
        }
    }

    return {}; // путь не найден
}

void Monster::updateAI(const Map& map, float playerX, float playerY, float dt)
{
    float dx = playerX - x;
    float dy = playerY - y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > aggroRange) return;

    tickCooldown(dt);
    if (dist <= ATTACK_RANGE) return; // стоим — атака в Engine

    // Пересчитываем путь раз в PATH_UPDATE_INTERVAL секунд
    pathUpdateTimer -= dt;
    if (pathUpdateTimer <= 0.0f) {
        pathUpdateTimer = PATH_UPDATE_INTERVAL;
        currentPath = findPath(map,
            static_cast<int>(x), static_cast<int>(y),
            static_cast<int>(playerX), static_cast<int>(playerY));
    }

    // Идём по пути
    if (currentPath.size() > 1) {
        auto& next = currentPath[1]; // [0] это текущая позиция
        float ndx = next.first  - x;
        float ndy = next.second - y;
        float nlen = std::sqrt(ndx * ndx + ndy * ndy);

        if (nlen > 0) {
            float speed = 3.0f * dt;
            x += (ndx / nlen) * speed;
            y += (ndy / nlen) * speed;
        }

        if (nlen < 0.2f) currentPath.erase(currentPath.begin());
    }
}

} // namespace rogue
