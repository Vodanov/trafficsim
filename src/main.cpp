#include "board.hpp"
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
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        uint32_t cellX = mousePos.x / cellSizeX;
        uint32_t cellY = mousePos.y / cellSizeX;
        board.at(cellY).at(cellX)->set();
        std::cout << "[CELL] -> " << cellX << ' ' << cellY << '\n';
      }
      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        uint32_t cellX = mousePos.x / cellSizeX;
        uint32_t cellY = mousePos.y / cellSizeX;
        board.create_entity(cellX, cellY, (Color){255, 0, 0, 255});
        std::cout << "[Entity] -> " << cellX << ' ' << cellY << '\n';
      }
      EndDrawing();
    }
  } else {
    for (uint32_t i = 0; i < screenHeight / cellSizeY; i++) {
      for (uint32_t j = 0; j < screenWidth / cellSizeX; j++) {
        if ((j + i) % 2 == 0)
          continue;
        board.at(i).at(j)->set(1);
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
