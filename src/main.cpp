#include "rogue/Raylib_renderer.h"
#include "rogue/core/Engine.h"

int main() {
  rogue::Engine engine(std::make_unique<rogue::RaylibRenderer>(800, 600));
  engine.run();
  return 0;
}
