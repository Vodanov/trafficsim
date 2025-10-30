#pragma once
#include "gameSettings.hpp"
#include "includes.hpp"
#include <ctime>
#include <raylib.h>
static u16 __counter;
class entity_t {
public:
  std::vector<Vector2> _positions{{0, 0}};
  std::vector<Vector2> _path;
  Vector2 _dest{0, 0};
  Color _col{0, 0, 0, 0};
  time_t _time;
  u8 _dir{0}, _speed{0}, _madness{0}, _type{0};
  void draw() {
    move();
    for (auto &pos : _positions)
      DrawRectangle(pos.x, pos.y, cellSizeX, cellSizeY, _col);
  }
  void move() {
    Vector2 curr = _positions.front();
    switch (_dir) {
    case 0:
      break;
    case 1:
      _positions.front().x -= cellSizeX;
      break;
    case 2:
      _positions.front().x += cellSizeX;
      break;
    case 3:
      _positions.front().y += cellSizeY;
      break;
    case 4:
      _positions.front().y -= cellSizeY;
      break;
    default:
      break;
    }
    for (u32 i = 1; i < _positions.size(); i++) {
      Vector2 tmp = _positions.at(i);
      _positions.at(i) = curr;
      curr = tmp;
    }
  }
  void update() {}
  entity_t() { get_id(); }
  entity_t(u16 x, u16 y) {
    _positions.front().x = x * cellSizeX;
    _positions.front().y = y * cellSizeY;
    get_id();
  }
  entity_t(u16 x, u16 y, Color col) {
    _positions.front().x = x * cellSizeX;
    _positions.front().y = y * cellSizeY;
    _col = col;
    get_id();
  }

private:
  u16 id;
  void get_id() {
    id = __counter;
    __counter++;
  }
};
