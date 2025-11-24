#include "definitions.hpp"
class cell_t {
public:
  void set(TileType i);
  void set(Color col);
  void set();
  void move(u8 dir);
  char info();
  void draw(double time, u8& pause);
  cell_t(i32 posX, i32 posY, TileType cost = TileType::GRASS);
  u16 _x, _y;
  Color _col{0, 0, 0, 255};
  u8 _cost{0};
  TileType _t{TileType::GRASS};
  double _time{0};
};