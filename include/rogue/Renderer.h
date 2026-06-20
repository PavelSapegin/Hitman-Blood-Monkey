#pragma once
#include <vector>

namespace rogue {
class Map;
class Entity;

class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual void initialize() = 0;
  virtual void shutdown() = 0;
  virtual void clear() = 0;
  virtual void refresh() = 0;
  virtual void drawChar(float x, float y, char ch, int color) = 0;
  virtual void renderMap(const Map &map, const std::vector<Entity *> &entities) = 0;
  virtual void setCameraTarget(float x, float y) = 0;
  virtual void beginScene() = 0;
  virtual void endScene() = 0;
};
}  // namespace rogue
