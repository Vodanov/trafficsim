#include "entity.hpp"
#include "definitions.hpp"
#include <print>
#include <raylib.h>
void entity_t::move(u8 &dir) {
  if (_speed <= _maxSpeed || _acceleration <= 0)
    _speed += _acceleration;
  if (dir != _dir) {
    _dir = dir;
    _speed /= 1.5;
  }
  switch (dir) {
  case 1:
    _relative_position.x += _speed;
    break;
  case 2:
    _relative_position.x -= _speed;
    break;
  case 3:
    _relative_position.y += _speed;
    break;
  case 4:
    _relative_position.y -= _speed;
    break;
  case 5:
    _relative_position.y += _speed;
    _relative_position.x += _speed;
    break;
  case 6:
    _relative_position.y += _speed;
    _relative_position.x -= _speed;
    break;
  case 7:
    _relative_position.y -= _speed;
    _relative_position.x += _speed;
    break;
  case 8:
    _relative_position.y -= _speed;
    _relative_position.x -= _speed;
    break;
  }
  if (_relative_position.x >= 1.0f || _relative_position.x <= -1.0f ||
      _relative_position.y >= 1.0f || _relative_position.y <= -1.0f) {
    _position = _path.top();
    _path.pop();
    _relative_position = {0, 0};
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
  DrawRectangle((_position.x + _relative_position.x) * cellSizeX,
                (_position.y + _relative_position.y) * cellSizeX, cellSizeX,
                cellSizeY, _col);
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