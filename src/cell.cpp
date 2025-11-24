#include "cell.hpp"
#include "definitions.hpp"
#include <raylib.h>
#include <unordered_map>
std::unordered_map<TileType, Color> typeToColor = {
    {TileType::BASE_ROAD, {50, 50, 50, 255}},
    {TileType::ROAD_UP, {80, 80, 80, 255}},
    {TileType::ROAD_RIGHT, {100, 100, 100, 255}},
    {TileType::ROAD_DOWN, {120, 120, 120, 255}},
    {TileType::ROAD_LEFT, {140, 140, 140, 255}},
    {TileType::ROAD_UP_DOWN, {80, 80, 200, 255}},
    {TileType::ROAD_LEFT_RIGHT, {200, 80, 80, 255}},
    {TileType::ROAD_LEFT_UP, {80, 200, 80, 255}},
    {TileType::ROAD_RIGHT_UP, {200, 200, 80, 255}},
    {TileType::ROAD_LEFT_DOWN, {200, 80, 200, 255}},
    {TileType::ROAD_RIGHT_DOWN, {80, 200, 200, 255}},
    {TileType::ROAD_UP_LEFT_RIGHT, {150, 100, 50, 255}},
    {TileType::ROAD_UP_LEFT_DOWN, {50, 150, 100, 255}},
    {TileType::ROAD_UP_RIGHT_DOWN, {100, 50, 150, 255}},
    {TileType::ROAD_LEFT_DOWN_RIGHT, {150, 150, 50, 255}},
    {TileType::ROAD_CROSS, {180, 180, 180, 255}},
    
    // Diagonal roads - using distinct brownish tones
    {TileType::ROAD_DIAGONAL_DOWN_LEFT, {120, 100, 80, 255}},
    {TileType::ROAD_DIAGONAL_DOWN_RIGHT, {140, 120, 100, 255}},
    {TileType::ROAD_DIAGONAL_UP_RIGHT, {160, 140, 120, 255}},
    {TileType::ROAD_DIAGONAL_UP_LEFT, {180, 160, 140, 255}},
    
    {TileType::GRASS, {40, 160, 40, 255}},
    {TileType::BUILDING, {120, 60, 20, 255}},
    
    // Traffic signals - keeping original colors but making them more vibrant
    {TileType::SIGNAL_UP_RED, {255, 40, 40, 255}},
    {TileType::SIGNAL_RIGHT_RED, {255, 40, 40, 255}},
    {TileType::SIGNAL_DOWN_RED, {255, 40, 40, 255}},
    {TileType::SIGNAL_LEFT_RED, {255, 40, 40, 255}},
    {TileType::SIGNAL_UP_YELLOW, {255, 255, 50, 255}},
    {TileType::SIGNAL_RIGHT_YELLOW, {255, 255, 50, 255}},
    {TileType::SIGNAL_DOWN_YELLOW, {255, 255, 50, 255}},
    {TileType::SIGNAL_LEFT_YELLOW, {255, 255, 50, 255}},
    {TileType::SIGNAL_UP_GREEN, {50, 255, 50, 255}},
    {TileType::SIGNAL_RIGHT_GREEN, {50, 255, 50, 255}},
    {TileType::SIGNAL_DOWN_GREEN, {50, 255, 50, 255}},
    {TileType::SIGNAL_LEFT_GREEN, {50, 255, 50, 255}},
    
    {TileType::ROAD_GRASS, {90, 110, 70, 255}},
};

std::unordered_map<TileType, u8> typeToCost = {
    {TileType::BASE_ROAD, 1},
    {TileType::ROAD_UP, 1},
    {TileType::ROAD_RIGHT, 1},
    {TileType::ROAD_DOWN, 1},
    {TileType::ROAD_LEFT, 1},
    {TileType::ROAD_UP_DOWN, 1},
    {TileType::ROAD_LEFT_RIGHT, 1},
    {TileType::ROAD_LEFT_UP, 1},
    {TileType::ROAD_RIGHT_UP, 1},
    {TileType::ROAD_LEFT_DOWN, 1},
    {TileType::ROAD_RIGHT_DOWN, 1},
    {TileType::ROAD_UP_LEFT_RIGHT, 1},
    {TileType::ROAD_UP_LEFT_DOWN, 1},
    {TileType::ROAD_UP_RIGHT_DOWN, 1},
    {TileType::ROAD_LEFT_DOWN_RIGHT, 1},
    {TileType::ROAD_CROSS, 1},
    
    // Diagonal roads - slightly higher cost than straight roads but less than grass
    {TileType::ROAD_DIAGONAL_DOWN_LEFT, 2},
    {TileType::ROAD_DIAGONAL_DOWN_RIGHT, 2},
    {TileType::ROAD_DIAGONAL_UP_RIGHT, 2},
    {TileType::ROAD_DIAGONAL_UP_LEFT, 2},
    
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
    
    {TileType::GRASS, 10},
    {TileType::ROAD_GRASS, 5},
    {TileType::BUILDING, 255},
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
    _t = TileType::SIGNAL_UP_YELLOW;
    break;
  case TileType::SIGNAL_UP_YELLOW:
    _t = TileType::SIGNAL_UP_GREEN;
    break;
  case TileType::SIGNAL_UP_GREEN:
    _t = TileType::SIGNAL_UP_RED;
    break;
  case TileType::SIGNAL_RIGHT_RED:
    _t = TileType::SIGNAL_RIGHT_YELLOW;
    break;
  case TileType::SIGNAL_RIGHT_YELLOW:
    _t = TileType::SIGNAL_RIGHT_GREEN;
    break;
  case TileType::SIGNAL_RIGHT_GREEN:
    _t = TileType::SIGNAL_RIGHT_RED;
    break;
  case TileType::SIGNAL_DOWN_RED:
    _t = TileType::SIGNAL_DOWN_YELLOW;
    break;
  case TileType::SIGNAL_DOWN_YELLOW:
    _t = TileType::SIGNAL_DOWN_GREEN;
    break;
  case TileType::SIGNAL_DOWN_GREEN:
    _t = TileType::SIGNAL_DOWN_RED;
    break;
  case TileType::SIGNAL_LEFT_RED:
    _t = TileType::SIGNAL_LEFT_YELLOW;
    break;
  case TileType::SIGNAL_LEFT_YELLOW:
    _t = TileType::SIGNAL_LEFT_GREEN;
    break;
  case TileType::SIGNAL_LEFT_GREEN:
    _t = TileType::SIGNAL_LEFT_RED;
    break;
  }
  _col = typeToColor[_t];
}
cell_t::cell_t(i32 posX, i32 posY, TileType cost) : _x(posX), _y(posY){ set(cost); }
void cell_t::move(u8 dir) {}
char cell_t::info() { return static_cast<u8>(_t); }
void cell_t::draw(double time, u8 &pause) {
  if (pause) {
    _time = time;
    DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
    return;
  }
  if (time - _time >= SIGNAL_TIME && (static_cast<u8>(_t) >= static_cast<u8>(TileType::SIGNAL_UP_RED) && static_cast<u8>(_t) <= static_cast<u8>(TileType::SIGNAL_LEFT_GREEN))) {
    set();
    _time = time;
  }
  //DrawTexture(textureMap[_t], _x, _y, {0,0,0,0});
  DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
}