#pragma once

#include <raylib.h>

#include <vector>

#include "Constants.h"
#include "Renderer.h"
#include "entities/Entity.h"
#include "world/Map.h"

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

  void renderMap(const Map &map, const std::vector<Entity *> &entities) override;

  void setCameraTarget(float x, float y) override;
  void beginScene() override;
  void endScene() override;
  Camera2D getCamera() const { return camera; }
  void setZoom(float zoom);
  float getZoom() const { return camera.zoom; }
  void addShake(float intensity);

private:
  Vector2 worldToScreen(float x, float y);
  int screenWidth;
  int screenHeight;
  Camera2D camera;
  float shakeIntensity = 0.0f;
};

}  // namespace rogue
