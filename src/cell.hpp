#include "definitions.hpp"
class cell_t {
public:
  void set(u8 i);
  void set(Color col);
  void set();
  void move(u8 dir);
  char info();
  void draw(double time, u8& pause);
  cell_t(i32 posX, i32 posY);
  u16 _x, _y;
  Color _col{0, 0, 0, 255};
  u8 _c{GRASS};
  u8 _t{GRASS};
  double _time{0};
};