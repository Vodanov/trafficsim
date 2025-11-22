#pragma once
#include "board.hpp"
#include "definitions.hpp"
#include "operators.hpp"
#include "visible_area.hpp"
#include <math.h>
#include <raylib.h>
#include <string>
#include <thread>
class trafficSim {
  Camera2D _camera = {0, 0, 0, 0, 0, 1.f};
  board_t board;
  u8 type = BASE_ROAD, pause = 1;
  VisibleArea area;
  u32 cellX, cellY;

public:
  trafficSim() {
    InitWindow(screenWidth, screenHeight, "TrafficSim");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGET_FPS);
    area = VisibleArea(_camera);
    board._camera = &_camera;
  }
  void run() {
    while (!WindowShouldClose() || IsKeyPressed(KEY_CAPS_LOCK)) {
      _camera.zoom = expf(logf(_camera.zoom) + (GetMouseWheelMove() * 0.1f));
      camera_checks();
      input_checks();
      BeginDrawing();
      BeginMode2D(_camera);
      ClearBackground({0, 0, 0, 255});
      if (IsKeyPressed(KEY_SPACE))
        pause = !pause;
      board.draw_board(pause, VisibleArea(_camera));
      std::string text = std::to_string(_camera.offset.x);
      DrawText(std::to_string(board.entity_count()).c_str(),
               _camera.offset.x / _camera.zoom * -1,
               _camera.offset.y / _camera.zoom * -1, 25 / _camera.zoom,
               {0, 255, 0, 255});
      Vector2 mousePos = GetMousePosition();
      cellX = (mousePos.x / cellSizeX - _camera.offset.x / cellSizeX) /
              _camera.zoom;
      cellY = (mousePos.y / cellSizeX - _camera.offset.y / cellSizeY) /
              _camera.zoom;
      if (cellX <= boardWidth / cellSizeX - 1 &&
          cellY <= boardHeight / cellSizeY - 1) {
        if (IsKeyPressed(KEY_F)) {
          board.create_desitation(cellX, cellY);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
          board.at(cellY, cellX).set(type);
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
          std::thread entity_thread(
              [x = cellX, y = cellY, &board = this->board]() {
                board.create_entity(x, y, {255, 0, 0, 255});
              });
          entity_thread.detach();
        }
      }
      EndDrawing();
    }
  }
  ~trafficSim() { CloseWindow(); }

private:
  static void create_thread(u32 x, u32 y, board_t &board) {
    board.create_entity(x, y, {255, 0, 0, 255});
  }
  void camera_checks() {
    auto old = _camera;
    if (IsKeyPressed(KEY_R))
      _camera.zoom = 1.0f;
    if (IsKeyDown(KEY_UP))
      _camera.offset.y += cellSizeY / _camera.zoom / 2;
    if (IsKeyDown(KEY_RIGHT))
      _camera.offset.x -= cellSizeX / _camera.zoom / 2;
    if (IsKeyDown(KEY_DOWN))
      _camera.offset.y -= cellSizeY / _camera.zoom / 2;
    if (IsKeyDown(KEY_LEFT))
      _camera.offset.x += cellSizeX / _camera.zoom / 2;
    if (_camera.zoom > 3.0f)
      _camera.zoom = 3.0f;
    else if (_camera.zoom < 0.1f)
      _camera.zoom = 0.1f;
    if (old == _camera)
      return;
    area = VisibleArea(_camera);
  }
  void input_checks() {
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
    i32 keyCount = (w ? 1 : 0) + (a ? 1 : 0) + (s ? 1 : 0) + (d ? 1 : 0);
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
};