#include "rogue/Raylib_renderer.h"
#include "rogue/TextureManager.h"
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
void RaylibRenderer::clear() {
  BeginDrawing();
  ClearBackground(BLACK);
}
void RaylibRenderer::refresh() { EndDrawing(); }

Vector2 RaylibRenderer::worldToScreen(float x, float y) {
  return {x * TILE_SIZE, y * TILE_SIZE};
}

void RaylibRenderer::renderMap(const Map &map,
                               const std::vector<Entity *> &entities) {
  auto &tm = TextureManager::getInstance();

  for (int y = 0; y < map.getHeight(); ++y) {
    for (int x = 0; x < map.getWidth(); ++x) {
      const Tile &tile = map.getTile(x, y);
      Vector2 screenPos = worldToScreen((float)x, (float)y);

      // Position for drawing the tile texture
      Vector2 tileOrigin = {screenPos.x, screenPos.y};

      Texture2D *tex = nullptr;
      if (tile.symbol == '#')
        tex = &tm.get("wall");
      else if (tile.symbol == '%') {
        // Blood tile - draw floor texture with red overlay
        DrawTexturePro(tm.get("floor"), {0, 0, 16, 16},
                       {tileOrigin.x, tileOrigin.y, TILE_SIZE, TILE_SIZE},
                       {0, 0}, 0.0f, WHITE);
        DrawRectangle((int)tileOrigin.x, (int)tileOrigin.y, (int)TILE_SIZE,
                      (int)TILE_SIZE, {150, 0, 0, 180});
        continue;
      } else if (tile.symbol == '.' || tile.symbol == ' ')
        tex = &tm.get("floor");

      DrawTexturePro(*tex, {0, 0, 16, 16},
                     {tileOrigin.x, tileOrigin.y, TILE_SIZE, TILE_SIZE}, {0, 0},
                     0.0f, WHITE);
    }
  }

  for (auto *entity : entities) {
    Vector2 pos = worldToScreen(entity->getX(), entity->getY());
    Vector2 tileOrigin = {pos.x - TILE_SIZE / 2.0f, pos.y - TILE_SIZE / 2.0f};

    Texture2D *tex = nullptr;
    int color = entity->getColor();

    if (color == COLOR_PLAYER)
      tex = &tm.get("player");
    else if (color == 3)
      tex = &tm.get("monster1");
    else if (color == 4)
      tex = &tm.get("monster2");
    else if (color == 5)
      tex = &tm.get("boss");

    if (tex) {
      DrawTexturePro(*tex, {0, 0, 16, 16},
                     {tileOrigin.x, tileOrigin.y, TILE_SIZE, TILE_SIZE}, {0, 0},
                     0.0f, WHITE);
    }
  }
}

void RaylibRenderer::setCameraTarget(float x, float y) {
  float targetX = x * TILE_SIZE;
  float targetY = y * TILE_SIZE;

  float speed = 5.0f; // чем меньше — тем плавнее
  float dt = GetFrameTime();

  camera.target.x += (targetX - camera.target.x) * speed * dt;
  camera.target.y += (targetY - camera.target.y) * speed * dt;
}

void RaylibRenderer::beginScene() { BeginMode2D(camera); }

void RaylibRenderer::endScene() { EndMode2D(); }

void RaylibRenderer::drawChar(float x, float y, char ch, int color) {
  Vector2 screenPos = worldToScreen(x, y);
  DrawText(TextFormat("%c", ch), (int)screenPos.x, (int)screenPos.y, 20,
           (color == COLOR_PLAYER) ? BLUE : WHITE);
}

} // namespace rogue
