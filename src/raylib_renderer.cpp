#include "rogue/Raylib_renderer.h"
#include <raylib.h>
#include <string>

namespace rogue {
void RaylibRenderer::initialize() {
    InitWindow(screenWidth, screenHeight, "Hitman Blood Monkey");
    SetTargetFPS(60);
}

void RaylibRenderer::shutdown() {
    CloseWindow();
}

void RaylibRenderer::clear() {
    BeginDrawing();
    ClearBackground(BLACK);
}

void RaylibRenderer::refresh() {
    EndDrawing();
}

void RaylibRenderer::drawChar(float x, float y, char ch, int color) {
    // Map color index to Raylib Colors
    Color tileColor = LIGHTGRAY; // Floor/Default
    if (color == 4) tileColor = GRAY;      // Walls
    else if (color == 2) tileColor = BLUE; // Player
    else if (color == 3) tileColor = RED;  // Monsters
    else if (color == 5) tileColor = MAROON; // Blood

    // Draw tile block
    DrawRectangle(static_cast<int>(x * TILE_SIZE), static_cast<int>(y * TILE_SIZE), static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE), tileColor);
    
    // Draw outline for tiles
    DrawRectangleLines(static_cast<int>(x * TILE_SIZE), static_cast<int>(y * TILE_SIZE), static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE), DARKGRAY);

    // Draw symbol on top for debugging/classic feel
    if (ch != ' ') {
        std::string s(1, ch);
        DrawText(s.c_str(), static_cast<int>(x * TILE_SIZE + TILE_SIZE / 4), static_cast<int>(y * TILE_SIZE + TILE_SIZE / 4), static_cast<int>(TILE_SIZE), BLACK);
    }
}
} // namespace rogue
