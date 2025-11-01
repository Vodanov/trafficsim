/*
    _c -> type of terrain
    0 -> road
    1 -> pedestrian
    2 -> building
    3 -> stop signal
*/
#include "definitions.hpp"
#include <raylib.h>
class cell_t {
public:
  void set(u8 i);
  void set(Color col);
  void set();
  void move(u8 dir);
  char info();
  void draw();
  cell_t(u32 posX, u32 posY) : _x(posX), _y(posY) {}
  u16 _x, _y;
  Color _col{0, 0, 0, 255};
  u8 _c{0};
};