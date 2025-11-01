#include "cell.hpp"
void cell_t::set(u8 i) {
  _c = i;
  if (i == 0) {
    _col = {0, 0, 0, 255};
  } else if (i == 1) {
    _col = {0, 128, 0, 255};
  } else if (i == 2) {
    _col = {0, 0, 128, 255};
  } else if (i == 3) {
    _col = {255, 255, 255, 255};
  } else {
    _col = {128, 0, 0, 255};
  }
}
void cell_t::set(Color col) { _col = col; }
void cell_t::set() {
  if (_c == 0) {
    _col = {0, 0, 0, 255};
  } else if (_c == 1) {
    _col = {0, 128, 0, 255};
  } else if (_c == 2) {
    _col = {0, 0, 128, 255};
  } else if (_c == 3) {
    _col = {255, 255, 255, 255};
  } else {
    _col = {128, 0, 0, 255};
  }
  _c++;
  if (_c >= 4)
    _c = 0;
}
void cell_t::move(u8 dir) {}
char cell_t::info() { return _c; }
void cell_t::draw() {
  set(_c);
  DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
}