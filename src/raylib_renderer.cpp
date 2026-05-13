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

void RaylibRenderer::drawChar(int x, int y, char ch, int color) {
    // Map color index to Raylib Colors
    Color tileColor = LIGHTGRAY; // Floor/Default
    if (color == 4) tileColor = GRAY;      // Walls
    else if (color == 2) tileColor = BLUE; // Player
    else if (color == 3) tileColor = RED;  // Monsters
    else if (color == 5) tileColor = MAROON; // Blood

    // Draw tile block
    DrawRectangle(x * fontSize, y * fontSize, fontSize, fontSize, tileColor);
    
    // Draw outline for tiles
    DrawRectangleLines(x * fontSize, y * fontSize, fontSize, fontSize, DARKGRAY);

    // Draw symbol on top for debugging/classic feel
    if (ch != ' ') {
        std::string s(1, ch);
        DrawText(s.c_str(), x * fontSize + fontSize / 4, y * fontSize + fontSize / 4, fontSize, BLACK);
    }
}
} // namespace rogue
