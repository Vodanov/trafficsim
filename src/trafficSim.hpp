#pragma once
#include "board.hpp"
#include "definitions.hpp"
#include "visible_area.hpp"
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <raylib.h>
#include <string>
#include <thread>
#include <utility>
class trafficSim {
  Camera2D _camera = {0, 0, 0, 0, 0, 1.f};
  board_t board;
  TileType type = TileType::GRASS;
  u8 pause = 1;
  VisibleArea area;
  u32 cellX, cellY;
  u8 road_size{2};
  std::thread entity_process_thread;
  i32 layer{0};
  float maxSpeed{DEFAULT_SPEED};
  // 0 - base layer
public:
  trafficSim() {
    InitWindow(screenWidth, screenHeight, "TrafficSim");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGET_FPS);
    area = VisibleArea(_camera);
    board._camera = &_camera;
  }
  void run() {
    entity_process_thread = std::thread([this]() {
      while (!WindowShouldClose()) {
        if (!pause) {
          double time = GetTime();
          board.process_entities(time, pause);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    });
    entity_process_thread.detach();
    while (!WindowShouldClose()) {
      _camera.zoom = expf(logf(_camera.zoom) + (GetMouseWheelMove() * 0.1f));
      camera_checks();
      input_checks();
      BeginDrawing();
      BeginMode2D(_camera);
      ClearBackground({0, 0, 0, 255});
      if (IsKeyPressed(KEY_SPACE))
        pause = !pause;
      if (IsKeyPressed(KEY_G)) {
        std::thread entity_thread([&]() {
          board.create_entity(104, 25, {0,125,255,255});
          board.create_entity(220, 25, {0,125,255,255});
          board.create_entity(104, 26, {0,125,255,255});
        });
        std::thread second_thread([&]() {
          board.create_entity(220, 26, {0,125,255,255});
          board.create_entity(177, 1, {0,125,255,255});
          board.create_entity(178, 67, {0,125,255,255});
          
        });
        entity_thread.detach();
        second_thread.detach();
      }
      Vector2 mousePos = GetMousePosition();

      cellX = (mousePos.x / cellSizeX - _camera.offset.x / cellSizeX) /
              _camera.zoom;
      cellY = (mousePos.y / cellSizeX - _camera.offset.y / cellSizeY) /
              _camera.zoom;
      std::pair<u32, u32> mouse = {cellX, cellY};
      board.draw_board(pause, VisibleArea(_camera), mouse);
      if (board.bfs_search_res.size() != 0) {
        for (auto position : board.bfs_search_res) {
          float screenX = position.x * 10;
          float screenY = position.y * 10;
          float size = 10.0f / _camera.zoom;

          DrawRectangle(screenX, screenY, size, size, {0, 0, 255, 20});
        }
      }
      // std::string text = std::to_string(_camera.offset.x);
      // DrawText(std::to_string(board.entity_count()).c_str(),
      //          _camera.offset.x / _camera.zoom * -1,
      //          _camera.offset.y / _camera.zoom * -1, 25 / _camera.zoom,
      //          {0, 255, 0, 255});
      // DrawText(std::to_string(maxSpeed).c_str(),
      //          _camera.offset.x / _camera.zoom * -1,
      //          _camera.offset.y / _camera.zoom * -1 + 25 / _camera.zoom,
      //          25 / _camera.zoom, {0, 255, 0, 255});
      // DrawText(std::to_string(road_size).c_str(),
      //          _camera.offset.x / _camera.zoom * -1,
      //          _camera.offset.y / _camera.zoom * -1 + 50 / _camera.zoom,
      //          25 / _camera.zoom, {0, 255, 0, 255});
      // DrawText(tileTypeNames[type].c_str(),
      //          _camera.offset.x / _camera.zoom * -1,
      //          _camera.offset.y / _camera.zoom * -1 + 75 / _camera.zoom,
      //          25 / _camera.zoom, {0, 255, 0, 255});

      if (cellX <= boardWidth / cellSizeX - 1 &&
          cellY <= boardHeight / cellSizeY - 1) {
        if (IsKeyDown(KEY_R))
          board.at(cellY, cellX)._maxSpeed = maxSpeed;
        if (IsKeyDown(KEY_T)) {
          board.at(cellY, cellX)._maxCapacity = road_size;
        }
        if (IsKeyPressed(KEY_F)) {
          board.at(cellY, cellX).set(TileType::TARGET);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
          board.at(cellY, cellX).set(type);
          board.at(cellY, cellX)._maxCapacity = road_size;
          board.at(cellY, cellX)._maxSpeed = maxSpeed;
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
          std::thread entity_thread(
              [x = cellX, y = cellY, &board = this->board]() {
                board.create_entity(x, y, random_color());
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
    board.create_entity(x, y, random_color());
  }
  static Color random_color() {
    return Color{static_cast<u8>(GetRandomValue(0, 255)),
                 static_cast<u8>(GetRandomValue(0, 255)),
                 static_cast<u8>(GetRandomValue(0, 255)), 255};
  }
  void camera_checks() {
    if (IsKeyPressed(KEY_BACKSPACE))
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
    area = VisibleArea(_camera);
  }
  void input_checks() {
    bool w = IsKeyDown(KEY_W);
    bool a = IsKeyDown(KEY_A);
    bool s = IsKeyDown(KEY_S);
    bool d = IsKeyDown(KEY_D);
    float speedIncrement = DEFAULT_SPEED / 10;
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_C))
      type = TileType::PEDESTRIAN_ROAD;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
      if (IsKeyPressed(KEY_V))
        type = TileType::WATER;
      if (IsKeyPressed(KEY_C))
        type = TileType::BUILDING_CITY;
      if (road_size < 8 && IsKeyPressed(KEY_NINE))
        road_size++;
      if (road_size > 0 && IsKeyPressed(KEY_EIGHT))
        road_size--;
      if (w) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
          type = TileType::SIGNAL_UP_YELLOW;
        } else if (IsKeyDown(KEY_LEFT_ALT)) {
          type = TileType::SIGNAL_UP_GREEN;
        } else {
          type = TileType::SIGNAL_UP_RED;
        }
        return;
      }
      if (d) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
          type = TileType::SIGNAL_RIGHT_YELLOW;
        } else if (IsKeyDown(KEY_LEFT_ALT)) {
          type = TileType::SIGNAL_RIGHT_GREEN;
        } else {
          type = TileType::SIGNAL_RIGHT_RED;
        }
        return;
      }
      if (s) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
          type = TileType::SIGNAL_DOWN_YELLOW;
        } else if (IsKeyDown(KEY_LEFT_ALT)) {
          type = TileType::SIGNAL_DOWN_GREEN;
        } else {
          type = TileType::SIGNAL_DOWN_RED;
        }
        return;
      }
      if (a) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
          type = TileType::SIGNAL_LEFT_YELLOW;
        } else if (IsKeyDown(KEY_LEFT_ALT)) {
          type = TileType::SIGNAL_LEFT_GREEN;
        } else {
          type = TileType::SIGNAL_LEFT_RED;
        }
        return;
      }
    } else {
      if (IsKeyPressed(KEY_V))
        type = TileType::FOREST;
      if (IsKeyPressed(KEY_NINE))
        maxSpeed += speedIncrement;
      if (IsKeyPressed(KEY_EIGHT))
        maxSpeed -= speedIncrement;
    }
    i32 keyCount = (w ? 1 : 0) + (a ? 1 : 0) + (s ? 1 : 0) + (d ? 1 : 0);
    if (keyCount >= 2) {
      if (w && a && s && d) {
        type = TileType::ROAD_CROSS;
        return;
      }
      if (w && a && d) {
        type = TileType::ROAD_UP_LEFT_RIGHT;
        return;
      }
      if (w && s && d) {
        type = TileType::ROAD_UP_RIGHT_DOWN;
        return;
      }
      if (w && a && s) {
        type = TileType::ROAD_UP_LEFT_DOWN;
        return;
      }
      if (a && s && d) {
        type = TileType::ROAD_LEFT_DOWN_RIGHT;
        return;
      }
      if (w && a) {
        type = TileType::ROAD_LEFT_UP;
        return;
      }
      if (w && d) {
        type = TileType::ROAD_RIGHT_UP;
        return;
      }
      if (a && s) {
        type = TileType::ROAD_LEFT_DOWN;
        return;
      }
      if (s && d) {
        type = TileType::ROAD_RIGHT_DOWN;
        return;
      }
      if (w && s) {
        type = TileType::ROAD_UP_DOWN;
        return;
      }
      if (a && d) {
        type = TileType::ROAD_LEFT_RIGHT;
        return;
      }
    }
    if (w)
      type = TileType::ROAD_UP;
    if (d)
      type = TileType::ROAD_RIGHT;
    if (s)
      type = TileType::ROAD_DOWN;
    if (a)
      type = TileType::ROAD_LEFT;

    if (IsKeyPressed(KEY_Z))
      type = TileType::BASE_ROAD;
    if (IsKeyPressed(KEY_X))
      type = TileType::GRASS;
  }
};