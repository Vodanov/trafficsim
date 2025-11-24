#pragma once
#include <cstdint>
#include <raylib.h>
#include <set>
#include <unordered_map>
#include <vector>
using u32 = uint32_t;
using i32 = int32_t;
using u16 = uint16_t;
using u8 = uint8_t;
constexpr const static u16 TARGET_FPS = 144;
constexpr const static double SIGNAL_TIME = 2.5;
constexpr const static double ENTITY_UPDATE_TIME = 0.02;
constexpr const static u32 screenWidth = 1280;
constexpr const static u32 screenHeight = 720;
constexpr const static u32 boardWidth = 4000;
constexpr const static u32 boardHeight = 4000;
constexpr const static double ENTITY_TIMEOUT = 1;
constexpr const static u32 cellSizeX = 10;
constexpr const static u32 cellSizeY = 10;
constexpr const u32 static tableWidth = boardWidth / cellSizeX;
constexpr const u32 static tableHeight = boardHeight / cellSizeY;
constexpr const u8 static CHAR_OFFSET = 70;
enum class Direction : u8 {
  NOWHERE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  DIAGONAL_UP_RIGHT,
  DIAGONAL_UP_LEFT,
  DIAGONAL_DOWN_RIGHT,
  DIAGONAL_DOWN_LEFT,
};
inline bool operator==(const Vector2 &a, const Vector2 &b) {
  return a.x == b.x && a.y == b.y;
}
inline bool operator==(const Camera2D &a, const Camera2D &b) {
  return (a.offset == b.offset && a.rotation == b.rotation &&
          a.target == b.target && a.zoom == b.zoom);
}
inline bool operator!=(const Vector2 &a, const Vector2 &b) {
  return a.x != b.x || a.y != b.y;
}
inline bool operator<(const Vector2 &a, const Vector2 &b) {
  return (a.x < b.x || (a.x == b.x && a.y < b.y));
}
inline Vector2 operator-(const Vector2 &a, const Vector2 &b) {
  return {a.x - b.x, a.y - b.y};
}
inline Vector2 operator+(const Vector2 &a, const Vector2 &b) {
  return {a.x + b.x, a.y + b.y};
}
inline bool operator==(const Color &a, const Color &b) {
  return (a.r == b.r && a.g == b.g && a.b == b.b);
}
inline bool operator!=(const Color &a, const Color &b) { return !(a == b); }
inline bool operator==(Direction a, Direction b) {
  return static_cast<u8>(a) == static_cast<u8>(b);
}

inline bool operator!=(Direction a, Direction b) { return !(a == b); }
inline Direction operator|(Direction a, Direction b) {
  return static_cast<Direction>(static_cast<u8>(a) | static_cast<u8>(b));
}
inline Direction operator+(Direction &a, Direction &b) {
  return static_cast<Direction>(static_cast<u8>(a) + static_cast<u8>(b));
}
inline Direction operator+(Direction &a, u8 b) {
  return static_cast<Direction>(static_cast<u8>(a) + b);
}
enum class TileType : u8 {
  BASE_ROAD,

  ROAD_UP,
  ROAD_RIGHT,
  ROAD_DOWN,
  ROAD_LEFT,

  // multi directional
  ROAD_UP_DOWN,
  ROAD_LEFT_RIGHT,
  ROAD_LEFT_UP,
  ROAD_RIGHT_UP,
  ROAD_LEFT_DOWN,
  ROAD_RIGHT_DOWN,
  ROAD_UP_LEFT_RIGHT,
  ROAD_UP_LEFT_DOWN,
  ROAD_UP_RIGHT_DOWN,
  ROAD_LEFT_DOWN_RIGHT,

  ROAD_CROSS,

  GRASS,
  ROAD_GRASS,
  BUILDING,

  SIGNAL_UP_RED,
  SIGNAL_RIGHT_RED,
  SIGNAL_DOWN_RED,
  SIGNAL_LEFT_RED,
  SIGNAL_UP_YELLOW,
  SIGNAL_RIGHT_YELLOW,
  SIGNAL_DOWN_YELLOW,
  SIGNAL_LEFT_YELLOW,
  SIGNAL_UP_GREEN,
  SIGNAL_RIGHT_GREEN,
  SIGNAL_DOWN_GREEN,
  SIGNAL_LEFT_GREEN,

  ROAD_DIAGONAL_DOWN_LEFT,
  ROAD_DIAGONAL_DOWN_RIGHT,
  ROAD_DIAGONAL_UP_LEFT,
  ROAD_DIAGONAL_UP_RIGHT,
};
static std::unordered_map<u8, TileType> directionToTileType = {
    {0, TileType::ROAD_RIGHT},      // dir 0: nx >= 1
    {1, TileType::ROAD_LEFT},        // dir 1: nx <= -1
    {2, TileType::ROAD_DOWN},        // dir 2: ny >= 1
    {3, TileType::ROAD_UP},            // dir 3: ny <= -1
    {4, TileType::ROAD_DIAGONAL_DOWN_RIGHT},  // dir 4: nx>=1 && ny>=1
    {5, TileType::ROAD_DIAGONAL_DOWN_LEFT},    // dir 5: nx<=-1 && ny>=1
    {6, TileType::ROAD_DIAGONAL_UP_RIGHT},      // dir 6: nx>=1 && ny<=-1
    {7, TileType::ROAD_DIAGONAL_UP_LEFT},         // dir 7: nx<=-1 && ny<=-1
};
inline TileType operator+(TileType &a, TileType &b) {
  return static_cast<TileType>(static_cast<u8>(a) + static_cast<u8>(b));
}

namespace std {
template <> struct hash<Vector2> {
  size_t operator()(Vector2 const &v) const noexcept {
    return (std::hash<int>()(v.x) << 1) ^ std::hash<int>()(v.y);
  }
};
} // namespace std
static std::unordered_map<TileType, Direction> tileToValue{
    {TileType::BASE_ROAD, Direction::NOWHERE},
    {TileType::ROAD_UP, Direction::UP},
    {TileType::ROAD_RIGHT, Direction::RIGHT},
    {TileType::ROAD_DOWN, Direction::DOWN},
    {TileType::ROAD_LEFT, Direction::LEFT},
    {TileType::ROAD_UP_DOWN, Direction::UP | Direction::DOWN},
    {TileType::ROAD_LEFT_RIGHT, Direction::LEFT | Direction::RIGHT},
    {TileType::ROAD_LEFT_UP, Direction::LEFT | Direction::UP},
    {TileType::ROAD_RIGHT_UP, Direction::RIGHT | Direction::UP},
    {TileType::ROAD_LEFT_DOWN, Direction::LEFT | Direction::DOWN},
    {TileType::ROAD_RIGHT_DOWN, Direction::RIGHT | Direction::DOWN},
    {TileType::ROAD_UP_LEFT_RIGHT,
     Direction::UP | Direction::LEFT | Direction::RIGHT},
    {TileType::ROAD_UP_LEFT_DOWN,
     Direction::UP | Direction::LEFT | Direction::DOWN},
    {TileType::ROAD_UP_RIGHT_DOWN,
     Direction::UP | Direction::RIGHT | Direction::DOWN},
    {TileType::ROAD_LEFT_DOWN_RIGHT,
     Direction::LEFT | Direction::DOWN | Direction::RIGHT},
    {TileType::ROAD_CROSS,
     Direction::UP | Direction::DOWN | Direction::LEFT | Direction::RIGHT},
    {TileType::GRASS, Direction::NOWHERE},
    {TileType::BUILDING, Direction::NOWHERE},
    {TileType::SIGNAL_UP_RED, Direction::UP},
    {TileType::SIGNAL_RIGHT_RED, Direction::RIGHT},
    {TileType::SIGNAL_DOWN_RED, Direction::DOWN},
    {TileType::SIGNAL_LEFT_RED, Direction::LEFT},
    {TileType::SIGNAL_UP_YELLOW, Direction::UP},
    {TileType::SIGNAL_RIGHT_YELLOW, Direction::RIGHT},
    {TileType::SIGNAL_DOWN_YELLOW, Direction::DOWN},
    {TileType::SIGNAL_LEFT_YELLOW, Direction::LEFT},
    {TileType::SIGNAL_UP_GREEN, Direction::UP},
    {TileType::SIGNAL_RIGHT_GREEN, Direction::RIGHT},
    {TileType::SIGNAL_DOWN_GREEN, Direction::DOWN},
    {TileType::SIGNAL_LEFT_GREEN, Direction::LEFT}};
static std::unordered_map<TileType, std::set<Vector2>> allowedMovements = {
    {TileType::ROAD_UP, {{0, -1}}},
    {TileType::ROAD_DOWN, {{0, 1}}},
    {TileType::ROAD_LEFT, {{-1, 0}}},
    {TileType::ROAD_RIGHT, {{1, 0}}},

    {TileType::ROAD_UP_DOWN, {{0, 1}, {0, -1}}},
    {TileType::ROAD_LEFT_RIGHT, {{1, 0}, {-1, 0}}},

    {TileType::ROAD_LEFT_UP, {{-1, 0}, {0, -1}}},
    {TileType::ROAD_RIGHT_UP, {{1, 0}, {0, -1}}},
    {TileType::ROAD_LEFT_DOWN, {{-1, 0}, {0, 1}}},
    {TileType::ROAD_RIGHT_DOWN, {{1, 0}, {0, 1}}},

    {TileType::ROAD_UP_LEFT_RIGHT, {{0, -1}, {-1, 0}, {1, 0}}},
    {TileType::ROAD_UP_LEFT_DOWN, {{0, -1}, {-1, 0}, {0, 1}}},
    {TileType::ROAD_UP_RIGHT_DOWN, {{0, -1}, {1, 0}, {0, 1}}},
    {TileType::ROAD_LEFT_DOWN_RIGHT, {Vector2{-1, 0}, {0, 1}, {1, 0}}},

    {TileType::SIGNAL_DOWN_RED, {Vector2{0, 1}}},
    {TileType::SIGNAL_DOWN_YELLOW, {{0, 1}}},
    {TileType::SIGNAL_DOWN_GREEN, {{0, 1}}},

    {TileType::SIGNAL_UP_RED, {{0, -1}}},
    {TileType::SIGNAL_UP_YELLOW, {{0, -1}}},
    {TileType::SIGNAL_UP_GREEN, {{0, -1}}},

    {TileType::SIGNAL_RIGHT_RED, {{1, 0}}},
    {TileType::SIGNAL_RIGHT_YELLOW, {{1, 0}}},
    {TileType::SIGNAL_RIGHT_GREEN, {{1, 0}}},

    {TileType::SIGNAL_LEFT_RED, {{-1, 0}}},
    {TileType::SIGNAL_LEFT_YELLOW, {{-1, 0}}},
    {TileType::SIGNAL_LEFT_GREEN, {{-1, 0}}},
    {TileType::GRASS, {{-1, 0}, {0, 1}, {1, 0}}},
    {TileType::ROAD_GRASS, {{-1, 0}, {0, 1}, {1, 0}}},
};
static std::unordered_map<TileType, std::vector<Vector2>> movementOnTile = {
    {TileType::ROAD_UP, {{0, 1}}},
    {TileType::ROAD_DOWN, {{0, -1}}},
    {TileType::ROAD_LEFT, {{-1, 0}}},
    {TileType::ROAD_RIGHT, {{1, 0}}},

    {TileType::ROAD_UP_DOWN, {{0, 1}, {0, -1}}},
    {TileType::ROAD_LEFT_RIGHT, {{-1, 0}, {1, 0}}},

    {TileType::ROAD_LEFT_UP, {{-1, 0}, {0, 1}}},
    {TileType::ROAD_RIGHT_UP, {{1, 0}, {0, 1}}},
    {TileType::ROAD_LEFT_DOWN, {{-1, 0}, {0, -1}}},
    {TileType::ROAD_RIGHT_DOWN, {{1, 0}, {0, -1}}},

    {TileType::ROAD_UP_LEFT_RIGHT, {{0, 1}, {-1, 0}, {1, 0}}},
    {TileType::ROAD_UP_LEFT_DOWN, {{0, 1}, {-1, 0}, {0, -1}}},
    {TileType::ROAD_UP_RIGHT_DOWN, {{0, 1}, {1, 0}, {0, -1}}},
    {TileType::ROAD_LEFT_DOWN_RIGHT, {{-1, 0}, {0, -1}, {1, 0}}},

    {TileType::SIGNAL_DOWN_RED, {{0, -1}}},
    {TileType::SIGNAL_DOWN_YELLOW, {{0, -1}}},
    {TileType::SIGNAL_DOWN_GREEN, {{0, -1}}},

    {TileType::SIGNAL_UP_RED, {{0, 1}}},
    {TileType::SIGNAL_UP_YELLOW, {{0, 1}}},
    {TileType::SIGNAL_UP_GREEN, {{0, 1}}},

    {TileType::SIGNAL_RIGHT_RED, {{1, 0}}},
    {TileType::SIGNAL_RIGHT_YELLOW, {{1, 0}}},
    {TileType::SIGNAL_RIGHT_GREEN, {{1, 0}}},

    {TileType::SIGNAL_LEFT_RED, {{-1, 0}}},
    {TileType::SIGNAL_LEFT_YELLOW, {{-1, 0}}},
    {TileType::SIGNAL_LEFT_GREEN, {{-1, 0}}},

    {TileType::GRASS,
     {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}}},
    {TileType::ROAD_GRASS,
     {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}}},
    {TileType::ROAD_DIAGONAL_DOWN_LEFT, {{-1, -1}}},
    {TileType::ROAD_DIAGONAL_DOWN_RIGHT, {{1, -1}}},
    {TileType::ROAD_DIAGONAL_UP_RIGHT, {{1, 1}}},
    {TileType::ROAD_DIAGONAL_UP_LEFT, {{-1, 1}}},
    {TileType::ROAD_CROSS,
     {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}}},
};