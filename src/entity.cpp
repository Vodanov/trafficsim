#include "entity.hpp"
#include "definitions.hpp"
#include <array>
#include <print>
#include <raylib.h>
void entity_t::move(u8 &dir, u8& moved) {
  if (_path.empty())
    return;
  if (_speed < _maxSpeed)
    _speed += _acceleration;
  else
    _speed = _maxSpeed;
  if (dir != _dir) {
    _dir = dir;
    _speed /= 1.5;
  }
  static std::array<Vector2, 8> movs = {
      Vector2{_acceleration, 0.f},  {-1 * (_acceleration), 0},        {0, _acceleration},
      {0, _acceleration * -1},      {_acceleration, _acceleration*-1},          {_acceleration * -1, _acceleration*-1},
      {_acceleration, _acceleration}, {_acceleration * -1, _acceleration}};
  _relative_position += movs.at(dir);
  if (_relative_position.x >= 1.0f || _relative_position.x <= -1.0f ||
      _relative_position.y >= 1.0f || _relative_position.y <= -1.0f) {
    float nx = 0, ny = 0;
    moved = 1;
    if (_relative_position.x >= 1.0f) {
      nx = _relative_position.x - 1.0f;
      _position.x = _path.top().x;
      _path.pop();
    } else if (_relative_position.x <= -1.0f) {
      nx = _relative_position.x + 1.0f;
      _position.x = _path.top().x;
      _path.pop();
    }

    if (_relative_position.y >= 1.0f) {
      ny = _relative_position.y - 1.0f;
      _position.y = _path.top().y;
      _path.pop();
    } else if (_relative_position.y <= -1.0f) {
      ny = _relative_position.y + 1.0f;
      _position.y = _path.top().y;
      _path.pop();
    }
    _relative_position = {nx, ny};
  }
}
Vector2 entity_t::next_pos() {
  if (_path.empty())
    return {0, 0};
  return _path.top();
}
Vector2 entity_t::next_next_pos() {
  if (_path.size() < 2)
    return {};
  Vector2 next = _path.top();
  _path.pop();
  Vector2 result = _path.empty() ? Vector2{0, 0} : _path.top();
  _path.push(next);
  return result;
}
void entity_t::draw() {
  DrawRectangle(((_position.x + _relative_position.x) * cellSizeX) + CAR_SIZE * _lane,
                ((_position.y + _relative_position.y) * cellSizeX) + CAR_SIZE * _lane, CAR_SIZE,
                CAR_SIZE, _col);
}
entity_t::entity_t(u16 x, u16 y, Color col) {
  _start = {(float)x, (float)y};
  _position.x = x;
  _position.y = y;
  _col = col;
}
entity_t::entity_t(u16 x, u16 y) {
  _start = {(float)x, (float)y};
  _position.x = x;
  _position.y = y;
}