#pragma once
#include "definitions.hpp"
#include <ctime>
#include <queue>
#include <raylib.h>
#include <stack>
#include <vector>
static u16 __counter;
class entity_t {
public:
  std::vector<Vector2> _positions{{0, 0}};
  std::stack<Vector2> _path;
  Vector2 _dest{0, 0};
  Color _col{0, 0, 0, 0};
  time_t _time;
  u8 _dir{0}, _madness{0}, _type{0};
  float _speed{40.0f};
  void draw(u8& pause);
  void move();
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
  void get_id();
};
