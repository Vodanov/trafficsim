#include "cell.hpp"
#include "definitions.hpp"
#include <cstddef>
#include <raylib.h>
#include <unordered_map>
std::unordered_map<TileType, Color> typeToColor = {
    {TileType::BASE_ROAD, {110, 110, 130, 255}},
    {TileType::ROAD_UP, {120, 120, 140, 255}},
    {TileType::ROAD_RIGHT, {130, 130, 150, 255}},
    {TileType::ROAD_DOWN, {140, 140, 160, 255}},
    {TileType::ROAD_LEFT, {150, 150, 170, 255}},

    {TileType::ROAD_UP_DOWN, {125, 125, 155, 255}},
    {TileType::ROAD_LEFT_RIGHT, {135, 135, 165, 255}},

    {TileType::ROAD_LEFT_UP, {145, 145, 175, 255}},
    {TileType::ROAD_RIGHT_UP, {155, 155, 185, 255}},
    {TileType::ROAD_LEFT_DOWN, {135, 135, 170, 255}},
    {TileType::ROAD_RIGHT_DOWN, {145, 145, 180, 255}},

    {TileType::ROAD_UP_LEFT_RIGHT, {160, 160, 195, 255}},
    {TileType::ROAD_UP_LEFT_DOWN, {170, 170, 205, 255}},
    {TileType::ROAD_UP_RIGHT_DOWN, {180, 180, 215, 255}},
    {TileType::ROAD_LEFT_DOWN_RIGHT, {165, 165, 200, 255}},

    // Slightly lighter + slightly bluer so intersections stand out
    {TileType::ROAD_CROSS, {95, 95, 120, 255}},

    {TileType::GRASS, {0, 120, 0, 255}},
    {TileType::BUILDING, {100, 50, 0, 255}},

    {TileType::SIGNAL_UP_RED, {255, 0, 0, 255}},
    {TileType::SIGNAL_RIGHT_RED, {255, 0, 0, 255}},
    {TileType::SIGNAL_DOWN_RED, {255, 0, 0, 255}},
    {TileType::SIGNAL_LEFT_RED, {255, 0, 0, 255}},

    {TileType::SIGNAL_UP_YELLOW, {255, 255, 0, 255}},
    {TileType::SIGNAL_RIGHT_YELLOW, {255, 255, 0, 255}},
    {TileType::SIGNAL_DOWN_YELLOW, {255, 255, 0, 255}},
    {TileType::SIGNAL_LEFT_YELLOW, {255, 255, 0, 255}},

    {TileType::SIGNAL_UP_GREEN, {0, 255, 0, 255}},
    {TileType::SIGNAL_RIGHT_GREEN, {0, 255, 0, 255}},
    {TileType::SIGNAL_DOWN_GREEN, {0, 255, 0, 255}},
    {TileType::SIGNAL_LEFT_GREEN, {0, 255, 0, 255}},

    {TileType::ROAD_GRASS, {76, 63, 47, 255}},
    {TileType::FOREST, {25, 70, 10, 255}},
    {TileType::WATER, {25, 50, 150, 255}},
    {TileType::PEDESTRIAN_ROAD, {170, 170, 170, 255}},
    {TileType::BUILDING_CITY, {70, 70, 70, 255}},
  {TileType::TARGET, {10,50,10,255}}
};
std::unordered_map<TileType, i32> typeToCost = {
    {TileType::BASE_ROAD, 0},
    {TileType::ROAD_UP, 0},
    {TileType::ROAD_RIGHT, 0},
    {TileType::ROAD_DOWN, 0},
    {TileType::ROAD_LEFT, 0},
    {TileType::ROAD_UP_DOWN, 0},
    {TileType::ROAD_LEFT_RIGHT, 0},
    {TileType::ROAD_LEFT_UP, 1},
    {TileType::ROAD_RIGHT_UP, 1},
    {TileType::ROAD_LEFT_DOWN, 1},
    {TileType::ROAD_RIGHT_DOWN, 1},
    {TileType::ROAD_UP_LEFT_RIGHT, 0},
    {TileType::ROAD_UP_LEFT_DOWN, 0},
    {TileType::ROAD_UP_RIGHT_DOWN, 0},
    {TileType::ROAD_LEFT_DOWN_RIGHT, 0},
    {TileType::ROAD_CROSS, 0},

    {TileType::SIGNAL_UP_RED, 4},
    {TileType::SIGNAL_RIGHT_RED, 4},
    {TileType::SIGNAL_DOWN_RED, 4},
    {TileType::SIGNAL_LEFT_RED, 4},
    {TileType::SIGNAL_UP_YELLOW, 4},
    {TileType::SIGNAL_RIGHT_YELLOW, 4},
    {TileType::SIGNAL_DOWN_YELLOW, 4},
    {TileType::SIGNAL_LEFT_YELLOW, 4},
    {TileType::SIGNAL_UP_GREEN, 4},
    {TileType::SIGNAL_RIGHT_GREEN, 4},
    {TileType::SIGNAL_DOWN_GREEN, 4},
    {TileType::SIGNAL_LEFT_GREEN, 4},

    {TileType::GRASS, 100},
    {TileType::ROAD_GRASS, 50},
    {TileType::BUILDING, 10000},
    {TileType::FOREST, 10000},
    {TileType::WATER, 10000},
    {TileType::PEDESTRIAN_ROAD, 255},
    {TileType::BUILDING_CITY, 10000},
    {TileType::TARGET, 0},
};
// tbd
// std::unordered_map<TileType, Texture2D> textureMap{
//   {TileType::GRASS,LoadTexture("/textures/grass.png")},
//   {0,LoadTexture("/textures/empty.png")}
// };
void cell_t::set(TileType i) {
  _t = i;
  _cost = typeToCost[i];
  _col = typeToColor[_t];
}

void cell_t::set(Color col) { _col = col; }

void cell_t::set() {
  switch (_t) {
  case TileType::SIGNAL_UP_RED:
    _t = TileType::SIGNAL_UP_GREEN;
    break;
  case TileType::SIGNAL_UP_YELLOW:
    _t = TileType::SIGNAL_UP_RED;
    break;
  case TileType::SIGNAL_UP_GREEN:
    _t = TileType::SIGNAL_UP_YELLOW;
    break;
  case TileType::SIGNAL_RIGHT_RED:
    _t = TileType::SIGNAL_RIGHT_GREEN;
    break;
  case TileType::SIGNAL_RIGHT_YELLOW:
    _t = TileType::SIGNAL_RIGHT_RED;
    break;
  case TileType::SIGNAL_RIGHT_GREEN:
    _t = TileType::SIGNAL_RIGHT_YELLOW;
    break;
  case TileType::SIGNAL_DOWN_RED:
    _t = TileType::SIGNAL_DOWN_GREEN;
    break;
  case TileType::SIGNAL_DOWN_YELLOW:
    _t = TileType::SIGNAL_DOWN_RED;
    break;
  case TileType::SIGNAL_DOWN_GREEN:
    _t = TileType::SIGNAL_DOWN_YELLOW;
    break;
  case TileType::SIGNAL_LEFT_RED:
    _t = TileType::SIGNAL_LEFT_GREEN;
    break;
  case TileType::SIGNAL_LEFT_YELLOW:
    _t = TileType::SIGNAL_LEFT_RED;
    break;
  case TileType::SIGNAL_LEFT_GREEN:
    _t = TileType::SIGNAL_LEFT_YELLOW;
    break;
  }
  _col = typeToColor[_t];
}
cell_t::cell_t(i32 posX, i32 posY, TileType cost) : _x(posX), _y(posY) {
  _sections.fill(0);
  set(cost);
}
void cell_t::move(u8 dir) {}
char cell_t::info() { return static_cast<u8>(_t); }

void cell_t::update(double& time, u8& pause){
  if(pause){
    _time = time;
    return;
  }
      const u8 type = static_cast<u8>(_t);

    const u8 SIGNAL_FIRST  = static_cast<u8>(TileType::SIGNAL_UP_RED);
    const u8 SIGNAL_LAST   = static_cast<u8>(TileType::SIGNAL_LEFT_GREEN);
    const u8 YELLOW_FIRST  = static_cast<u8>(TileType::SIGNAL_UP_YELLOW);

    if (type >= SIGNAL_FIRST && type <= SIGNAL_LAST) {
        double dt = time - _time;

        if (type >= YELLOW_FIRST) {
            if (dt >= MOVEMENT_SIGNAL_TIME) {
                _time = time;
                set();
            }
        } else {
            if (dt >= SIGNAL_TIME) {
                _time = time;
                set();
            }
        }
    }
}
void cell_t::draw() {
  DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
}