#include "board.hpp"
#include "definitions.hpp"
#include <iostream>
#include <raylib.h>
void check_inputs(u8 &type) {
  bool w = IsKeyDown(KEY_W);
  bool a = IsKeyDown(KEY_A);
  bool s = IsKeyDown(KEY_S);
  bool d = IsKeyDown(KEY_D);

  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    if (w) {
      if (IsKeyDown(KEY_LEFT_CONTROL)) {
        type = SIGNAL_UP_YELLOW;
      } else if (IsKeyDown(KEY_LEFT_ALT)) {
        type = SIGNAL_UP_GREEN;
      } else {
        type = SIGNAL_UP_RED;
      }
      return;
    }
    if (d) {
      if (IsKeyDown(KEY_LEFT_CONTROL)) {
        type = SIGNAL_RIGHT_YELLOW;
      } else if (IsKeyDown(KEY_LEFT_ALT)) {
        type = SIGNAL_RIGHT_GREEN;
      } else {
        type = SIGNAL_RIGHT_RED;
      }
      return;
    }
    if (s) {
      if (IsKeyDown(KEY_LEFT_CONTROL)) {
        type = SIGNAL_DOWN_YELLOW;
      } else if (IsKeyDown(KEY_LEFT_ALT)) {
        type = SIGNAL_DOWN_GREEN;
      } else {
        type = SIGNAL_DOWN_RED;
      }
      return;
    }
    if (a) {
      if (IsKeyDown(KEY_LEFT_CONTROL)) {
        type = SIGNAL_LEFT_YELLOW;
      } else if (IsKeyDown(KEY_LEFT_ALT)) {
        type = SIGNAL_LEFT_GREEN;
      } else {
        type = SIGNAL_LEFT_RED;
      }
      return;
    }
  }
  int keyCount = (w ? 1 : 0) + (a ? 1 : 0) + (s ? 1 : 0) + (d ? 1 : 0);
  if (keyCount >= 2) {
    if (w && a && s && d) {
      type = ROAD_CROSS;
      return;
    }
    if (w && a && d) {
      type = ROAD_UP_LEFT_RIGHT;
      return;
    }
    if (w && s && d) {
      type = ROAD_UP_RIGHT_DOWN;
      return;
    }
    if (w && a && s) {
      type = ROAD_UP_LEFT_DOWN;
      return;
    }
    if (a && s && d) {
      type = ROAD_LEFT_DOWN_RIGHT;
      return;
    }
    if (w && a) {
      type = ROAD_LEFT_UP;
      return;
    }
    if (w && d) {
      type = ROAD_RIGHT_UP;
      return;
    }
    if (a && s) {
      type = ROAD_LEFT_DOWN;
      return;
    }
    if (s && d) {
      type = ROAD_RIGHT_DOWN;
      return;
    }
    if (w && s) {
      type = ROAD_UP_DOWN;
      return;
    }
    if (a && d) {
      type = ROAD_LEFT_RIGHT;
      return;
    }
  }
  if (w)
    type = ROAD_UP;
  if (d)
    type = ROAD_RIGHT;
  if (s)
    type = ROAD_DOWN;
  if (a)
    type = ROAD_LEFT;

  if (IsKeyPressed(KEY_Z))
    type = BASE_ROAD;
  if (IsKeyPressed(KEY_X))
    type = GRASS;
  if (IsKeyPressed(KEY_C))
    type = BUILDING;
  if (IsKeyPressed(KEY_V))
    type = 99;
}
int main() {
  InitWindow(screenWidth, screenHeight, "Traffic sim");
  u8 type = BASE_ROAD, pause = 1;
  board_t board;
  board.size();
  std::clock_t curr = std::clock();
  SetTargetFPS(60);
  type = 0;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground({0, 0, 0, 255});
    if (IsKeyPressed(KEY_SPACE))
      pause = !pause;
    board.draw_board(pause);
    Vector2 mousePos = GetMousePosition();
    u32 cellX = mousePos.x / cellSizeX;
    u32 cellY = mousePos.y / cellSizeX;
    check_inputs(type);
    if (cellX <= screenWidth / cellSizeX && cellY <= screenHeight / cellSizeY) {
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        board.at(cellY, cellX).set(type);
        std::cout << "[CELL] y:" << cellY << " x:" << cellX
                  << " type:" << (char)type + 80 << '\n';
      }
      if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        board.create_entity(cellX, cellY, {255, 0, 0, 255}, {0, 0});
        std::cout << "[Entity] -> " << cellY << ' ' << cellX << '\n';
      }
    } else {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        board.at(cellY, cellX).set();
        std::cout << "[CELL] -> " << cellX << ' ' << cellY << '\n';
      }
      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        board.create_entity(cellX, cellY, {255, 0, 0, 255}, {0, 0});
        std::cout << "[Entity] -> " << cellX << ' ' << cellY << '\n';
      }
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
