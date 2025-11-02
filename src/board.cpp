#include "board.hpp"
#include "definitions.hpp"
#include <algorithm>
#include <iostream>
#include <print>
#include <queue>
#include <raylib.h>
#include <stack>
#include <unordered_set>
#include <vector>
void board_t::draw_cells() {
  for (auto &row : boardBG)
    for (auto &cell : row)
      cell->draw();
}
void board_t::create_entity(float x, float y, Color col, Vector2 dest) {
  std::cout << "Creating entity:\n";
  entity_t new_ent(x, y, col);
  std::cout << "Searching for path\n";
  std::stack<Vector2> paths = find_path({x, y}, dest);
  std::cout << "Adding to the list\n";
  new_ent._path = std::move(paths);
  entities.push_back(new_ent);
}
void board_t::draw_entities() {
  for (auto &entity : entities) {
    entity.draw();
  }
}
void board_t::draw_board() {
  draw_cells();
  draw_entities();
}
std::unique_ptr<cell_t> &board_t::at(u32 idx, u32 idx2) {
  return boardBG.at(idx).at(idx2);
}
const u32 static tableWidth = screenWidth / cellSizeX;
const u32 static tableHeight = screenHeight / cellSizeY;

void dfs(u8 &found, std::stack<Vector2> &path, std::vector<u8> &table,
         Vector2 &end) {
  if (found)
    return;

  if (path.empty()) {
    return;
  }

  Vector2 top = path.top();
  u32 x = top.x;
  u32 y = top.y;

  if (x >= tableWidth || y >= tableHeight) {
    if (!path.empty())
      path.pop();
    return;
  }

  if (x == end.x && y == end.y) {
    found = 1;
    return;
  }

  if (table[x + y * tableWidth] == 1) {
    return;
  }

  table[x + y * tableWidth] = 1;

  if (x + 1 < tableWidth) {
    u32 nx = x + 1;
    if (!table[nx + y * tableWidth]) {
      path.push({(float)nx, (float)y});
      dfs(found, path, table, end);
      if (found)
        return;
      if (!path.empty())
        path.pop();
    }
  }
  if (x > 0) {
    u32 nx = x - 1;
    if (!table[nx + y * tableWidth]) {
      path.push({(float)nx, (float)y});
      dfs(found, path, table, end);
      if (found)
        return;
      if (!path.empty())
        path.pop();
    }
  }
  if (y + 1 < tableHeight) {
    u32 ny = y + 1;
    if (!table[x + ny * tableWidth]) {
      path.push({(float)x, (float)ny});
      dfs(found, path, table, end);
      if (found)
        return;
      if (!path.empty())
        path.pop();
    }
  }
  if (y > 0) {
    u32 ny = y - 1;
    if (!table[x + ny * tableWidth]) {
      path.push({(float)x, (float)ny});
      dfs(found, path, table, end);
      if (found)
        return;
      if (!path.empty())
        path.pop();
    }
  }
}
std::stack<Vector2> board_t::find_path(Vector2 start, Vector2 end) {
  std::stack<Vector2> path;
  std::vector<u8> table(screenWidth / cellSizeX * screenHeight / cellSizeY, 0);
  u8 found = 0;
  path.push(end);
  dfs(found, path, table, start);
  return path;
}
