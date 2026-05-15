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
    void drawChar(float x, float y, char ch, int color) override;
    Vector2 worldToIsometric(float x, float y);

private:
    const float TILE_SIZE = 20.0f;
    const int screenWidth = 800;
    const int screenHeight = 600;
};
} // namespace rogue
