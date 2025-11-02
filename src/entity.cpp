#include "entity.hpp"
#include "definitions.hpp"
void entity_t::move() {
  if (_path.empty())
    return;
  Vector2 curr = _positions.front();
  _positions.front().x = _path.top().x * cellSizeX;
  _positions.front().y = _path.top().y * cellSizeX;
  _path.pop();
  /*switch (_dir) {
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
  }*/
}
void entity_t::get_id() {
  id = __counter;
  __counter++;
}
void entity_t::draw() {
  move();
  for (auto &pos : _positions)
    DrawRectangle(pos.x, pos.y, cellSizeX, cellSizeY, _col);
}