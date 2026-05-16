#pragma once

#include "Renderer.h"
#include "entities/Entity.h"
#include "world/Map.h"
#include <raylib.h>
#include <string>
#include <vector>

namespace rogue {

class RaylibRenderer : public IRenderer {
public:
  RaylibRenderer(int sw, int sh);
  ~RaylibRenderer() override;

  void initialize() override;
  void shutdown() override;
  void clear() override;
  void refresh() override;
  void drawChar(float x, float y, char ch, int color) override;

  void renderMap(const Map &map,
                 const std::vector<Entity *> &entities) override;

  void setCameraTarget(float x, float y) override;
  void beginScene() override;
  void endScene() override;

private:
  Vector2 worldToScreen(float x, float y);
  int screenWidth;
  int screenHeight;
  Camera2D camera;
  static constexpr float TILE_SIZE = 40.0f;
};

} // namespace rogue
