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
    Vector2 isoPos = worldToIsometric(x, y);

    // Map color index to Raylib Colors
    Color tileColor = LIGHTGRAY; // Floor/Default
    if (color == 4) tileColor = GRAY;      // Walls
    else if (color == 2) tileColor = BLUE; // Player
    else if (color == 3) tileColor = RED;  // Monsters
    else if (color == 5) tileColor = MAROON; // Blood

    // Draw isometric tile (diamond shape)
    DrawPoly(isoPos, 4, 25.0f, 45.0f, tileColor);
    DrawPolyLines(isoPos, 4, 25.0f, 45.0f, DARKGRAY);

    // Draw symbol on top
    if (ch != ' ') {
        std::string s(1, ch);
        DrawText(s.c_str(), static_cast<int>(isoPos.x - 5), static_cast<int>(isoPos.y - 5), 20, BLACK);
    }
}

Vector2 RaylibRenderer::worldToIsometric(float x, float y) {
    float tileWidth = 40.0f;
    float tileHeight = 20.0f;
    
    float isoX = (x - y) * (tileWidth / 2.0f);
    float isoY = (x + y) * (tileHeight / 2.0f);
    
    return { isoX + screenWidth / 2.0f, isoY + 100.0f };
}
} // namespace rogue
