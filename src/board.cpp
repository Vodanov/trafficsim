#include "board.hpp"
#include "definitions.hpp"
#include "visible_area.hpp"
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <raylib.h>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "operators.hpp"
namespace std {
template <> struct hash<Vector2> {
  size_t operator()(Vector2 const &v) const noexcept {
    return (std::hash<int>()(v.x) << 1) ^ std::hash<int>()(v.y);
  }
};
} // namespace std
std::unordered_map<u8, u8> tileToValue{{BASE_ROAD, NOWHERE},
                                       {ROAD_UP, UP},
                                       {ROAD_DOWN, DOWN},
                                       {ROAD_LEFT, LEFT},
                                       {ROAD_LEFT_DOWN, DOWN},
                                       {ROAD_LEFT_UP, UP},
                                       {ROAD_RIGHT_DOWN, DOWN},
                                       {ROAD_RIGHT_UP, UP},
                                       {ROAD_CROSS, NOWHERE},
                                       {GRASS, NOWHERE},
                                       {BUILDING, NOWHERE},
                                       {SIGNAL_UP_RED, UP},
                                       {SIGNAL_RIGHT_RED, RIGHT},
                                       {SIGNAL_DOWN_RED, DOWN},
                                       {SIGNAL_LEFT_RED, LEFT},
                                       {SIGNAL_UP_YELLOW, UP},
                                       {SIGNAL_RIGHT_YELLOW, RIGHT},
                                       {SIGNAL_DOWN_YELLOW, DOWN},
                                       {SIGNAL_LEFT_YELLOW, LEFT},
                                       {SIGNAL_UP_GREEN, LEFT},
                                       {SIGNAL_RIGHT_GREEN, RIGHT},
                                       {SIGNAL_DOWN_GREEN, RIGHT},
                                       {SIGNAL_LEFT_GREEN, LEFT}};
static const std::unordered_map<u8, std::set<Vector2>> allowedMovements = {
    {ROAD_UP, {{0, -1}}},
    {ROAD_DOWN, {{0, 1}}},
    {ROAD_LEFT, {{-1, 0}}},
    {ROAD_RIGHT, {{1, 0}}},

    {ROAD_UP_DOWN, {{0, 1}, {0, -1}}},
    {ROAD_LEFT_RIGHT, {{1, 0}, {-1, 0}}},

    {ROAD_LEFT_UP, {{-1, 0}, {0, -1}}},
    {ROAD_RIGHT_UP, {{1, 0}, {0, -1}}},
    {ROAD_LEFT_DOWN, {{-1, 0}, {0, 1}}},
    {ROAD_RIGHT_DOWN, {{1, 0}, {0, 1}}},

    {ROAD_UP_LEFT_RIGHT, {{0, -1}, {-1, 0}, {1, 0}}},
    {ROAD_UP_LEFT_DOWN, {{0, -1}, {-1, 0}, {0, 1}}},
    {ROAD_UP_RIGHT_DOWN, {{0, -1}, {1, 0}, {0, 1}}},
    {ROAD_LEFT_DOWN_RIGHT, {{-1, 0}, {0, 1}, {1, 0}}},

    {ROAD_CROSS, {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}},

    {SIGNAL_DOWN_RED, {{0, 1}}},
    {SIGNAL_DOWN_YELLOW, {{0, 1}}},
    {SIGNAL_DOWN_GREEN, {{0, 1}}},

    {SIGNAL_UP_RED, {{0, -1}}},
    {SIGNAL_UP_YELLOW, {{0, -1}}},
    {SIGNAL_UP_GREEN, {{0, -1}}},

    {SIGNAL_RIGHT_RED, {{1, 0}}},
    {SIGNAL_RIGHT_YELLOW, {{1, 0}}},
    {SIGNAL_RIGHT_GREEN, {{1, 0}}},

    {SIGNAL_LEFT_RED, {{-1, 0}}},
    {SIGNAL_LEFT_YELLOW, {{-1, 0}}},
    {SIGNAL_LEFT_GREEN, {{-1, 0}}},
};
void board_t::size() {
  std::cout << boardBG.size() << ' ' << boardBG.front().size() << '\n';
}
void board_t::draw_cells(u8 &pause, VisibleArea area) {
  double time = GetTime();
  for (u32 i = area.startY; i < area.endY; i++)
    for (u32 j = area.startX; j < area.endX; j++)

      at(i, j).draw(time, pause);
}
void board_t::create_entity(float x, float y, Color col, Vector2 dest) {
  if (tileToValue[at((i32)y, (i32)x)._c] == 0)
    return;
  entity_t new_ent(x, y, col);
  std::stack<Vector2> paths = find_path({x, y}, dest);
  new_ent._path = std::move(paths);
  entities.push_back(new_ent);
}
void board_t::draw_entities(u8 &pause) {
  for (auto &entity : entities) {
    Vector2 nextPos = entity.next_pos(), nextNextPos = entity.next_next_pos();
    if (nextPos.x == entity._dest.x && nextPos.y == entity._dest.y)
      continue;
    if (!pause && GetTime() - entity._time >= ENTITY_UPDATE_TIME) {
      auto nx = nextPos.x - entity._positions.front().x,
           ny = nextPos.y - entity._positions.front().y;
      u8 dir = (nx >= 1) ? 1 : (nx <= -1) ? 2 : (ny >= 1) ? 3 : 4;
      auto &cell = at(nextPos.y, nextPos.x);
      Vector2 relative_position;
      switch (dir) {
      case 1:
        relative_position = {1, 0};
        break;
      case 2:
        relative_position = {-1, 0};
        break;
      case 3:
        relative_position = {0, 1};
        break;
      case 4:
        relative_position = {0, -1};
        break;
      }
      bool entityBlocking = 0;
      for (auto &other_entity : entities) {
        if (other_entity._positions.front() == nextPos + relative_position ||
            other_entity._positions.front() == nextNextPos) {
          entityBlocking = 1;
          break;
        }
        if (entityBlocking)
          break;
      }

      if (cell._t == SIGNAL_DOWN_RED || cell._t == SIGNAL_LEFT_RED ||
          cell._t == SIGNAL_RIGHT_RED || cell._t == SIGNAL_UP_RED ||
          entityBlocking)
        entity._speed = 0;
      else {
        entity.move(dir);
        entity._time = GetTime();
      }
    }
    entity.draw(pause);
  }
}
void board_t::draw_board(u8 &pause, VisibleArea area) {
  draw_cells(pause, area);
  draw_entities(pause);
}
cell_t &board_t::at(i32 y, i32 x) { return boardBG.at(y).at(x); }
board_t::~board_t() {
  std::ofstream file("board.brd");
  for (auto &row : boardBG) {
    for (auto &cell : row) {
      file << u8(cell.info() + 70);
    }
    file << '\n';
  }
  file.close();
}
board_t::board_t() {
  boardBG.reserve(boardHeight / cellSizeY);
  std::ifstream file("board.brd");
  if (!file.is_open()) {
    for (uint32_t i = 0; i < boardHeight; i += cellSizeY) {
      std::vector<cell_t> row;
      for (uint32_t j = 0; j < boardWidth; j += cellSizeX) {
        row.push_back(cell_t(j, i));
      }
      boardBG.emplace_back(row);
    }
    return;
  }
  std::string line;
  i32 i = 0;
  while (std::getline(file, line)) {
    std::vector<cell_t> row;
    i32 idx = 0;
    i32 j = 0;
    for (auto &c : line) {
      row.push_back(cell_t(j, i));
      row.at(idx).set(c - 70);
      j += cellSizeX;
      idx++;
    }
    boardBG.emplace_back(row);
    i += cellSizeY;
  }
}
bool board_t::cant_move(i32 x, i32 y, Vector2 prev) {
  if (at(y, x)._t == BASE_ROAD) {
    return true;
  }

  float movX = x - prev.x;
  float movY = y - prev.y;
  u8 type = at(prev.y, prev.x)._c;

  auto iterator = allowedMovements.find(type);
  if (iterator == allowedMovements.end())
    return 1;

  return iterator->second.find({movX, movY}) == iterator->second.end();
}
void board_t::bfs(std::vector<u8> &table, Vector2 &end, Vector2 &start,
                  std::stack<Vector2> &path) {
  std::queue<Vector2> paths;
  std::unordered_map<Vector2, Vector2> parents;
  paths.push(start);
  Vector2 diffs[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  while (!paths.empty()) {
    auto curr = paths.front();
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
      if ((i32)nx < 0 || (i32)nx > boardWidth || (i32)ny >= boardHeight ||
          (i32)ny < 0)
        continue;
      if (table.at(nx + ny * tableWidth) || cant_move((i32)nx, (i32)ny, curr))
        continue;
      parents[{nx, ny}] = curr;
      table.at((i32)nx + (i32)ny * tableWidth) = 1;
      paths.push({nx, ny});
    }
  }
}
std::stack<Vector2> board_t::find_path(Vector2 start, Vector2 end) {
  std::stack<Vector2> path;
  std::vector<u8> table(boardWidth / cellSizeX * boardHeight / cellSizeY, 0);
  u8 found = 0;
  bfs(table, end, start, path);
  if (path.size() == 1)
    path.pop();
  return path;
}
