#pragma once
#include "Renderer.h"
#include <ncurses.h>

namespace rogue {
class NcursesRenderer : public IRenderer {
public:
  void initialize() override;
  void shutdown() override;
  void clear() override;
  void refresh() override;
  void drawChar(int x, int y, char ch, int color) override;
};
} // namespace rogue
