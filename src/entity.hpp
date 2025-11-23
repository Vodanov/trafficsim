#pragma once
#include "definitions.hpp"
#include <raylib.h>
#include <stack>
#include <future>
class entity_t {
public:
  Vector2 _position{0, 0};
  std::stack<Vector2> _path;
  Vector2 _dest{0, 0}, _start;
  Color _col{0, 0, 0, 0};
  double _time{0};
  u8 _dir{0};
  float _speed{0.0f}, _maxSpeed{0.76f}, _acceleration{0.03f};
  u8 _has_path{0};
  void draw();
  void move(u8& dir);
  Vector2 next_pos();
  entity_t(u16 x, u16 y);
  entity_t(u16 x, u16 y, Color col);
  Vector2 next_next_pos();
private:
  Vector2 _relative_position{0.0f,0.0f};
};