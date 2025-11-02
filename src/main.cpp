#include "board.hpp"
#include "definitions.hpp"
#include <iostream>
#include <raylib.h>
int main() {
  InitWindow(screenWidth, screenHeight, "Traffic sim");
  u8 type = 2;
  board_t board;
  if (type >= 1) {
    std::clock_t curr = std::clock();
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      board.draw_board();
      Vector2 mousePos = GetMousePosition();
      u32 cellX = mousePos.x / cellSizeX;
      u32 cellY = mousePos.y / cellSizeX;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        board.at(cellY, cellX)->set();
        std::cout << "[CELL] -> " << cellX << ' ' << cellY << '\n';
      }
      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        board.create_entity(cellX, cellY, {255, 0, 0, 255}, {0, 0});
        std::cout << "[Entity] -> " << cellX << ' ' << cellY << '\n';
      }
      EndDrawing();
    }
  } else {
    for (u32 i = 0; i < screenHeight / cellSizeY; i++) {
      for (u32 j = 0; j < screenWidth / cellSizeX; j++) {
        if ((j + i) % 2 == 0)
          continue;
        board.at(i, j)->set(1);
      }
    }
    std::clock_t curr = std::clock();
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      board.draw_board();
      EndDrawing();
    }
  }
  CloseWindow();
  return 0;
}
