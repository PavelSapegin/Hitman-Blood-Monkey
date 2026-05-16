#include "rogue/Raylib_renderer.h"
#include <raylib.h>

namespace rogue {

RaylibRenderer::RaylibRenderer(int sw, int sh)
    : screenWidth(sw), screenHeight(sh) {
    camera.target = {0.0f, 0.0f};
    camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

RaylibRenderer::~RaylibRenderer() {}

void RaylibRenderer::initialize() {
    InitWindow(screenWidth, screenHeight, "Hitman Blood Monkey - Top-Down");
    SetTargetFPS(60);
}

void RaylibRenderer::shutdown() { CloseWindow(); }
void RaylibRenderer::clear() { BeginDrawing(); ClearBackground(BLACK); }
void RaylibRenderer::refresh() { EndDrawing(); }

Vector2 RaylibRenderer::worldToScreen(float x, float y) {
    return { x * TILE_SIZE + TILE_SIZE / 2.0f, 
             y * TILE_SIZE + TILE_SIZE / 2.0f };
}

void RaylibRenderer::renderMap(const Map &map, const std::vector<Entity*>& entities) {
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            Vector2 screenPos = worldToScreen((float)x, (float)y);
            const Tile& tile = map.getTile(x, y);

            DrawRectangle((int)screenPos.x, (int)screenPos.y, (int)TILE_SIZE, (int)TILE_SIZE,
                          (tile.symbol == '%') ? MAROON : LIGHTGRAY);
            if (tile.symbol == '#') {
                DrawRectangle((int)screenPos.x, (int)screenPos.y, (int)TILE_SIZE, (int)TILE_SIZE, DARKGRAY);
            }
        }
    }

    for (auto* entity : entities) {
        Vector2 pos = worldToScreen(entity->getX(), entity->getY());
        Color eColor = (entity->getColor() == COLOR_PLAYER) ? BLUE : RED;
        DrawCircle((int)pos.x, (int)pos.y, 15, eColor);
    }
}

void RaylibRenderer::setCameraTarget(float x, float y) {
    camera.target = { x * TILE_SIZE, y * TILE_SIZE };
}

void RaylibRenderer::beginScene() {
    BeginMode2D(camera);
}

void RaylibRenderer::endScene() {
    EndMode2D();
}

void RaylibRenderer::drawChar(float x, float y, char ch, int color) {
    Vector2 screenPos = worldToScreen(x, y);
    DrawText(TextFormat("%c", ch), (int)screenPos.x, (int)screenPos.y, 20,
             (color == COLOR_PLAYER) ? BLUE : WHITE);
}

} // namespace rogue
