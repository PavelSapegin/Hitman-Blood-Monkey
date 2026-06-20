#pragma once
#include <ncurses.h>

#include "Renderer.h"

namespace rogue {
class NcursesRenderer : public IRenderer {
public:
  void initialize() override;
  void shutdown() override;
  void clear() override;
  void refresh() override;
  void drawChar(float x, float y, char ch, int color) override;
};
}  // namespace rogue
