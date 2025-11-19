#include "entity.hpp"
#include "definitions.hpp"
void entity_t::move() {
  if (_path.empty())
    return;
  Vector2 curr = _positions.front();
  _positions.front().x = _path.top().x * cellSizeX;
  _positions.front().y = _path.top().y * cellSizeX;
  _path.pop();
}
void entity_t::get_id() {
  id = __counter;
  __counter++;
}
void entity_t::draw(u8 &pause) {
  if (!pause)
    move();
  for (auto &pos : _positions)
    DrawRectangle(pos.x, pos.y, cellSizeX, cellSizeY, _col);
}
entity_t::entity_t(u16 x, u16 y, Color col) {
  _positions.front().x = x * cellSizeX;
  _positions.front().y = y * cellSizeY;
  _col = col;
  get_id();
}
entity_t::entity_t(u16 x, u16 y) {
  _positions.front().x = x * cellSizeX;
  _positions.front().y = y * cellSizeY;
  get_id();
}