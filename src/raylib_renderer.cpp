#include "rogue/Raylib_renderer.h"
#include <raylib.h>
#include <string>

namespace rogue {
void RaylibRenderer::initialize() {
  InitWindow(screenWidth, screenHeight, "Hitman Blood Monkey");
  SetTargetFPS(60);
  mapTexture = LoadRenderTexture(screenWidth, screenHeight);
}

void RaylibRenderer::shutdown() {
  UnloadRenderTexture(mapTexture);
  CloseWindow();
}

void RaylibRenderer::clear() {
  BeginDrawing();
  ClearBackground(BLACK);
}

void RaylibRenderer::refresh() { EndDrawing(); }

void RaylibRenderer::drawChar(float x, float y, char ch, int color) {
  Vector2 isoPos = worldToIsometric(x, y);

  // Map color index to Raylib Colors
  Color tileColor = LIGHTGRAY; // Floor/Default
  if (color == 4)
    tileColor = GRAY; // Walls
  else if (color == 2)
    tileColor = BLUE; // Player
  else if (color == 3)
    tileColor = RED; // Monsters
  else if (color == 5)
    tileColor = MAROON; // Blood

  // Draw isometric tile (diamond shape)
  if (color == 4) { // Walls
    // Draw base tile
    DrawPoly(isoPos, 4, 25.0f, 45.0f, DARKGRAY);
    // Draw the wall volume by shifting upward
    Vector2 topPos = {isoPos.x, isoPos.y - 40.0f};
    DrawRectangle(static_cast<int>(topPos.x - 15),
                  static_cast<int>(topPos.y - 30), 30, 40, tileColor);
    DrawPolyLines(isoPos, 4, 25.0f, 45.0f, DARKGRAY);
  } else { // Floor
    DrawPoly(isoPos, 4, 25.0f, 45.0f, tileColor);
  }

  // Draw symbol on top
  if (ch != ' ') {
    std::string s(1, ch);
    DrawText(s.c_str(), static_cast<int>(isoPos.x - 5),
             static_cast<int>(isoPos.y - 5), 20, BLACK);
  }
}

Vector2 RaylibRenderer::worldToIsometric(float x, float y) {
  float tileWidth = 40.0f;
  float tileHeight = 20.0f;

  float isoX = (x - y) * (tileWidth / 2.0f);
  float isoY = (x + y) * (tileHeight / 2.0f);

  return {isoX + screenWidth / 2.0f, isoY + 100.0f};
}

void RaylibRenderer::beginMapRendering() {
  BeginTextureMode(mapTexture);
  ClearBackground(BLACK);
}

void RaylibRenderer::endMapRendering() {
  EndTextureMode();
  isMapRendered = true;
}

void RaylibRenderer::drawMapTexture() {
  if (isMapRendered) {
    DrawTextureRec(mapTexture.texture,
                   (Rectangle){0, 0, (float)mapTexture.texture.width,
                               (float)-mapTexture.texture.height},
                   (Vector2){0, 0}, WHITE);
  }
}
} // namespace rogue
