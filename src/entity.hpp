#pragma once
#include "includes.hpp"
class entity_t {
public:
  void draw() { DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col); }
  entity_t() { get_id(); }
  entity_t(u16 x, u16 y) : _x(x), _y(y) { get_id(); }
  entity_t(u16 x, u16 y, Color col) : _x(x), _y(y), _col(col) { get_id(); }

  u16 _x{0}, _y{0};
  Color _col{0, 0, 0, 0};
  u8 dir{1}, _speed{0}, _madness{0}, _type{0};

private:
  static u16 counter;
  u16 id;
  void get_id() {
    id = counter;
    counter++;
  }
};
