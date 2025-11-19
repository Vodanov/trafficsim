#include "cell.hpp"
#include "definitions.hpp"
#include <iostream>
#include <raylib.h>
std::unordered_map<u8, Color> typeToColor = {
    {BASE_ROAD, {0, 0, 0, 255}},
    {ROAD_UP, {100, 100, 100, 255}},
    {ROAD_RIGHT, {120, 120, 120, 255}},
    {ROAD_DOWN, {140, 140, 140, 255}},
    {ROAD_LEFT, {160, 160, 160, 255}},
    {ROAD_UP_DOWN, {80, 80, 200, 255}},
    {ROAD_LEFT_RIGHT, {200, 80, 80, 255}},
    {ROAD_LEFT_UP, {80, 200, 80, 255}},
    {ROAD_RIGHT_UP, {200, 200, 80, 255}},
    {ROAD_LEFT_DOWN, {200, 80, 200, 255}},
    {ROAD_RIGHT_DOWN, {80, 200, 200, 255}},
    {ROAD_UP_LEFT_RIGHT, {150, 100, 50, 255}},
    {ROAD_UP_LEFT_DOWN, {50, 150, 100, 255}},
    {ROAD_UP_RIGHT_DOWN, {100, 50, 150, 255}},
    {ROAD_LEFT_DOWN_RIGHT, {150, 150, 50, 255}},
    {ROAD_CROSS, {255, 255, 255, 255}},
    {GRASS, {0, 200, 0, 255}},
    {BUILDING, {100, 50, 0, 255}},
    {SIGNAL_UP_RED, {255, 0, 0, 255}},
    {SIGNAL_RIGHT_RED, {255, 0, 0, 255}},
    {SIGNAL_DOWN_RED, {255, 0, 0, 255}},
    {SIGNAL_LEFT_RED, {255, 0, 0, 255}},
    {SIGNAL_UP_YELLOW, {255, 255, 0, 255}},
    {SIGNAL_RIGHT_YELLOW, {255, 255, 0, 255}},
    {SIGNAL_DOWN_YELLOW, {255, 255, 0, 255}},
    {SIGNAL_LEFT_YELLOW, {255, 255, 0, 255}},
    {SIGNAL_UP_GREEN, {0, 255, 0, 255}},
    {SIGNAL_RIGHT_GREEN, {0, 255, 0, 255}},
    {SIGNAL_DOWN_GREEN, {0, 255, 0, 255}},
    {SIGNAL_LEFT_GREEN, {0, 255, 0, 255}}
};
void cell_t::set(u8 i) {
  _t = i;
  _c = i;
  _col = typeToColor[_t];
}

void cell_t::set(Color col) { _col = col; }

void cell_t::set() {
  switch (_t) {
  case SIGNAL_UP_RED:
    _t = SIGNAL_UP_YELLOW;
    break;
  case SIGNAL_UP_YELLOW:
    _t = SIGNAL_UP_GREEN;
    break;
  case SIGNAL_UP_GREEN:
    _t = SIGNAL_UP_RED;
    break;
  case SIGNAL_RIGHT_RED:
    _t = SIGNAL_RIGHT_YELLOW;
    break;
  case SIGNAL_RIGHT_YELLOW:
    _t = SIGNAL_RIGHT_GREEN;
    break;
  case SIGNAL_RIGHT_GREEN:
    _t = SIGNAL_RIGHT_RED;
    break;
  case SIGNAL_DOWN_RED:
    _t = SIGNAL_DOWN_YELLOW;
    break;
  case SIGNAL_DOWN_YELLOW:
    _t = SIGNAL_DOWN_GREEN;
    break;
  case SIGNAL_DOWN_GREEN:
    _t = SIGNAL_DOWN_RED;
    break;
  case SIGNAL_LEFT_RED:
    _t = SIGNAL_LEFT_YELLOW;
    break;
  case SIGNAL_LEFT_YELLOW:
    _t = SIGNAL_LEFT_GREEN;
    break;
  case SIGNAL_LEFT_GREEN:
    _t = SIGNAL_LEFT_RED;
    break;
  }
  _col = typeToColor[_t];
}

void cell_t::move(u8 dir) {}
char cell_t::info() { return _c; }
void cell_t::draw(double time, u8 &pause) {
  if (pause) {
    DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
    return;
  }
  // enums for signal
  if (time - _time >= 5 && (_t >= 18 && _t <= 29)) {
    std::cout << time << ' ' << _time << '\n';
    set();
    _time = time;
  }
  // DrawTexture(Texture2D texture, int posX, int posY, Color tint)
  DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
}