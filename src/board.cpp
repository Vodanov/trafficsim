#include "board.hpp"
#include "definitions.hpp"
#include <iostream>
#include <print>
#include <queue>
#include <raylib.h>
#include <stack>
#include <unordered_map>
#include <vector>
void board_t::draw_cells() {
  for (auto &row : boardBG)
    for (auto &cell : row)
      cell->draw();
}
void board_t::create_entity(float x, float y, Color col, Vector2 dest) {
  entity_t new_ent(x, y, col);
  std::stack<Vector2> paths = find_path({x, y}, dest);
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
namespace std {
template<>
struct hash<Vector2> {
    size_t operator()(const Vector2& v) const noexcept {
        // convert to integers first if necessary
        auto x = static_cast<int>(v.x);
        auto y = static_cast<int>(v.y);
        // simple, fast hash combo
        return (static_cast<size_t>(x) * 73856093) ^ (static_cast<size_t>(y) * 19349663);
    }
};
}

bool operator==(const Vector2& a, const Vector2& b) {
    return a.x == b.x && a.y == b.y;
}
bool operator!=(const Vector2& a, const Vector2& b) {
    return a.x != b.x || a.y != b.y;
}
void board_t::bfs(std::vector<u8> &table,
         Vector2 &end, Vector2& start, std::stack<Vector2>& path) {
  std::queue<Vector2> paths;
  std::unordered_map<Vector2, Vector2> parents;
  paths.push(start);
  Vector2 diffs[] = {{1,0}, {-1,0}, {0,1},{0,-1}};
  while (!paths.empty()){
    auto curr = paths.front();
    std::cout << curr.x << ' ' << curr.y << '\n';
    if (curr.x == end.x && curr.y == end.y){
      path.push(end);
      curr = parents[end];
      while(curr != start){
        path.push(curr);
        curr = parents[curr];
      }
      return;
    }
    paths.pop();
    for(auto [x, y] : diffs){
      float nx = curr.x + x;
      float ny = curr.y + y;
      if ((i32)nx < 0 || (i32)ny < 0 || (u32)nx >= tableWidth || (u32)ny >= tableHeight || table[nx + ny * tableWidth] || boardBG[(i32)ny][(i32)nx]->_c != 0)
        continue;
      parents[{nx, ny}] = curr;
      table[nx + ny * tableWidth] = 1;
      paths.push({curr.x + x, curr.y + y});
    }
  }
}
std::stack<Vector2> board_t::find_path(Vector2 start, Vector2 end) {
  std::stack<Vector2> path;
  std::vector<u8> table(screenWidth / cellSizeX * screenHeight / cellSizeY, 0);
  u8 found = 0;
  path.push(end);
  bfs(table, end ,start, path);
  return path;
}
