#include "rogue/Ncurses_renderer.h"
#include "rogue/core/Engine.h"

int main() {
  rogue::Engine engine(std::make_unique<rogue::NcursesRenderer>());
  engine.run();
  return 0;
}
