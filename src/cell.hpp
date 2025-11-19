#include "definitions.hpp"
#include <ctime>
#include <raylib.h>

class cell_t {
public:
  void set(u8 i);
  void set(Color col);
  void set();
  void move(u8 dir);
  char info();
  void draw(double time, u8& pause);
  cell_t(u32 posX, u32 posY);
  u16 _x, _y;
  Color _col{0, 0, 0, 255};
  u8 _c{0};
  u8 _t{BASE_ROAD};
  double _time{0};
};