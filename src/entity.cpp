#include "entity.hpp"
#include "definitions.hpp"
#include <raylib.h>
void entity_t::move(u8 &dir) {
  if (_speed <= _maxSpeed)
    _speed += _acceleration;
  if (dir != _dir) {
    _dir = dir;
    _speed /= 2;
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
  }
  if (_relative_position.x >= 1 || _relative_position.x <= -1 ||
      _relative_position.y >= 1 || _relative_position.y <= -1) {
    _positions.front().x = _path.top().x;
    _positions.front().y = _path.top().y;
    _path.pop();
    _relative_position = {0, 0};
  }
}
Vector2 entity_t::next_pos() { return {_path.top().x, _path.top().y}; }
Vector2 entity_t::next_next_pos() {
  if (_path.empty())
    return {};
  Vector2 tmp = _path.top(), res = {};
  _path.pop();
  if (!_path.empty())
    res = _path.top();
  _path.push(tmp);
  return res;
}
void entity_t::draw(u8 &pause) {
  for (auto &pos : _positions)
    DrawRectangle((pos.x + _relative_position.x) * cellSizeX,
                  (pos.y + _relative_position.y) * cellSizeX, cellSizeX,
                  cellSizeY, _col);
}
entity_t::entity_t(u16 x, u16 y, Color col) {
  _start = {(float)x,(float)y};
  _positions.front().x = x;
  _positions.front().y = y;
  _col = col;
}
entity_t::entity_t(u16 x, u16 y){
  _start = {(float)x,(float)y};
  _positions.front().x = x;
  _positions.front().y = y;
}