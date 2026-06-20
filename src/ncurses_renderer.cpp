#include "rogue/Ncurses_renderer.h"

#include <ncurses.h>

namespace rogue {
void NcursesRenderer::initialize() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
  }
}

void NcursesRenderer::clear() {
  ::clear();
}

void NcursesRenderer::refresh() {
  ::refresh();
}
void NcursesRenderer::drawChar(float x, float y, char ch, int color) {
  attron(COLOR_PAIR(color));
  mvaddch(static_cast<int>(y), static_cast<int>(x), ch);
  attroff(COLOR_PAIR(color));
}
void NcursesRenderer::shutdown() {
  endwin();
}
}  // namespace rogue
