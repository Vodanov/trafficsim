#pragma once
#include <cstdint>
#include <raylib.h>
#define u32 uint32_t
#define i32 int32_t
#define u16 uint16_t
#define u8 uint8_t
constexpr const static u16 TARGET_FPS = 144;
constexpr const static double SIGNAL_TIME = 2.5;
constexpr const static double ENTITY_UPDATE_TIME = 0;
constexpr const static u32 screenWidth = 1280;
constexpr const static u32 screenHeight = 720;
constexpr const static u32 boardWidth = 4000;
constexpr const static u32 boardHeight = 4000;
constexpr const static double ENTITY_TIMEOUT = 1;
constexpr const static u32 cellSizeX = 10;
constexpr const static u32 cellSizeY = 10;
constexpr const u32 static tableWidth = boardWidth / cellSizeX;
constexpr const u32 static tableHeight = boardHeight / cellSizeY;
enum Direction {
  NOWHERE,
  UP,
  DOWN,
  LEFT,
  RIGHT
};
enum tileType {
  BASE_ROAD,
  
  ROAD_UP,
  ROAD_RIGHT, 
  ROAD_DOWN,
  ROAD_LEFT,
  
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
};