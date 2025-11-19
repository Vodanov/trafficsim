#include "board.hpp"
#include "definitions.hpp"
#include <iostream>
#include <print>
#include <queue>
#include <raylib.h>
#include <stack>
#include <unordered_map>
#include <vector>
void board_t::draw_cells(u8 &pause) {
  for (auto &row : boardBG)
    for (auto &cell : row)
      cell.draw(GetTime(), pause);
}
void board_t::create_entity(float x, float y, Color col, Vector2 dest) {
  entity_t new_ent(x, y, col);
  std::stack<Vector2> paths = find_path({x, y}, dest);
  new_ent._path = std::move(paths);
  entities.push_back(new_ent);
}
void board_t::draw_entities(u8 &pause) {
  for (auto &entity : entities) {
    entity.draw(pause);
  }
}
void board_t::draw_board(u8 &pause) {
  draw_cells(pause);
  draw_entities(pause);
}
cell_t &board_t::at(u32 y, u32 x) {
  return boardBG.at(y).at(x);
}
const u32 static tableWidth = screenWidth / cellSizeX;
const u32 static tableHeight = screenHeight / cellSizeY;
namespace std {
template <> struct hash<Vector2> {
  size_t operator()(Vector2 const &v) const noexcept {
    return (std::hash<int>()(v.x) << 1) ^ std::hash<int>()(v.y);
  }
};
} // namespace std

bool board_t::cant_move(i32 x, i32 y, Vector2 prev) {
  if (x < 0 || y < 0 || x >= tableWidth || y >= tableHeight)
    return 1;
  if (boardBG.at(y).at(x)._t == BASE_ROAD)
    return 1;
  i32 movX = x - prev.x, movY = y - prev.y;
  auto type = boardBG[prev.y][prev.x]._c;
  if(type == ROAD_CROSS) return 0;
  if (movX == 0 && movY == 1) {
  return !(type == ROAD_DOWN || type == ROAD_LEFT_DOWN || type == ROAD_RIGHT_DOWN ||
           type == ROAD_UP_DOWN || type == ROAD_UP_LEFT_DOWN || type == ROAD_UP_RIGHT_DOWN || type == ROAD_LEFT_DOWN_RIGHT ||
           type == ROAD_CROSS ||
           type == SIGNAL_DOWN_RED || type == SIGNAL_DOWN_YELLOW || type == SIGNAL_DOWN_GREEN);
} else if (movX == 0 && movY == -1) {
  return !(type == ROAD_UP || type == ROAD_LEFT_UP || type == ROAD_RIGHT_UP ||
           type == ROAD_UP_DOWN || type == ROAD_UP_LEFT_RIGHT || type == ROAD_UP_LEFT_DOWN || type == ROAD_UP_RIGHT_DOWN ||
           type == ROAD_CROSS ||
           type == SIGNAL_UP_RED || type == SIGNAL_UP_YELLOW || type == SIGNAL_UP_GREEN);
} else if (movX == 1 && movY == 0) {
  return !(type == ROAD_RIGHT || type == ROAD_RIGHT_UP || type == ROAD_RIGHT_DOWN ||
           type == ROAD_LEFT_RIGHT || type == ROAD_UP_LEFT_RIGHT || type == ROAD_UP_RIGHT_DOWN || type == ROAD_LEFT_DOWN_RIGHT ||
           type == ROAD_CROSS ||
           type == SIGNAL_RIGHT_RED || type == SIGNAL_RIGHT_YELLOW || type == SIGNAL_RIGHT_GREEN);
} else if (movX == -1 && movY == 0) {
  return !(type == ROAD_LEFT || type == ROAD_LEFT_UP || type == ROAD_LEFT_DOWN ||
           type == ROAD_LEFT_RIGHT || type == ROAD_UP_LEFT_RIGHT || type == ROAD_UP_LEFT_DOWN || type == ROAD_LEFT_DOWN_RIGHT ||
           type == ROAD_CROSS ||
           type == SIGNAL_LEFT_RED || type == SIGNAL_LEFT_YELLOW || type == SIGNAL_LEFT_GREEN);
}
  return 1;
}
bool operator==(const Vector2 &a, const Vector2 &b) {
  return a.x == b.x && a.y == b.y;
}
bool operator!=(const Vector2 &a, const Vector2 &b) {
  return a.x != b.x || a.y != b.y;
}
void board_t::bfs(std::vector<u8> &table, Vector2 &end, Vector2 &start,
                  std::stack<Vector2> &path) {
  std::queue<Vector2> paths;
  std::unordered_map<Vector2, Vector2> parents;
  paths.push(start);
  Vector2 diffs[] = {
      {1, 0}, {-1, 0}, {0, 1}, {0, -1}}; //, {1,1}, {-1,1}, {1,-1}, {-1,-1}};
  while (!paths.empty()) {
    BeginDrawing();
    ClearBackground({0, 0, 0, 255});
    WaitTime(0.1);
    auto curr = paths.front();
    std::cout << curr.x << ' ' << curr.y << '\n';
    ClearBackground({0, 0, 0, 255});
    
    if (curr.x == end.x && curr.y == end.y) {
      path.push(end);
      curr = parents[end];
      while (curr != start) {
        path.push(curr);
        curr = parents[curr];
      }
      return;
    }
    paths.pop();
    for (auto [x, y] : diffs) {
      float nx = curr.x + x;
      float ny = curr.y + y;
      if (table[nx + ny * tableWidth] || cant_move((i32)nx, (i32)ny, curr))
        continue;
      parents[{nx, ny}] = curr;
      table[nx + ny * tableWidth] = 1;
      paths.push({nx, ny});
      DrawRectangle(curr.x * cellSizeX, curr.y * cellSizeX, cellSizeX, cellSizeX,
                  RED);
    }
    EndDrawing();
  }
}
std::stack<Vector2> board_t::find_path(Vector2 start, Vector2 end) {
  std::stack<Vector2> path;
  std::vector<u8> table(screenWidth / cellSizeX * screenHeight / cellSizeY, 0);
  u8 found = 0;
  bfs(table, end, start, path);
  if (path.size() == 1)
    path.pop();
  return path;
}
