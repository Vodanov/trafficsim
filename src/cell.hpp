#include "definitions.hpp"
#include "entity.hpp"
#include <array>
#include <bitset>
#include <vector>
class cell_t {
public:
  void set(TileType i);
  void set(Color col);
  void set();
  void move(u8 dir);
  char info();
  void draw();
  cell_t(i32 posX, i32 posY, TileType cost = TileType::GRASS);
  u16 _x, _y;
  Color _col{0, 0, 0, 255};
  u8 _cost{0};
  u8 _maxCapacity{2};
  std::array<u8, 256> _sections;
  float _maxSpeed{DEFAULT_SPEED};
  TileType _t{TileType::GRASS};
  double _time{0};
  void update(double& time, u8 &pause);
};