#pragma once

namespace rogue {
class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual void initialize() = 0;
  virtual void shutdown() = 0;
  virtual void clear() = 0;
  virtual void refresh() = 0;
  virtual void drawChar(int x, int y, char ch, int color) = 0;
};
} // namespace rogue
