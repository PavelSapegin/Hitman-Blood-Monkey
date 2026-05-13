#pragma once
#include "Renderer.h"
#include <raylib.h>

namespace rogue {
class RaylibRenderer : public IRenderer {
public:
    void initialize() override;
    void shutdown() override;
    void clear() override;
    void refresh() override;
    void drawChar(int x, int y, char ch, int color) override;

private:
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int fontSize = 20;
};
} // namespace rogue
