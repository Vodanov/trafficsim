#include "board.hpp"
#include "definitions.hpp"
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <raylib.h>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
std::unordered_map<u8, u8> tileToValue{{BASE_ROAD, 0},
                                       {ROAD_UP, 1},
                                       {ROAD_DOWN, 2},
                                       {ROAD_LEFT, 1},
                                       {ROAD_LEFT_DOWN, 1},
                                       {ROAD_LEFT_UP, 1},
                                       {ROAD_RIGHT_DOWN, 1},
                                       {ROAD_RIGHT_UP, 1},
                                       {ROAD_CROSS, 1},
                                       {GRASS, 0},
                                       {BUILDING, 0},
                                       {SIGNAL_UP_RED, 1},
                                       {SIGNAL_RIGHT_RED, 1},
                                       {SIGNAL_DOWN_RED, 1},
                                       {SIGNAL_LEFT_RED, 1},
                                       {SIGNAL_UP_YELLOW, 1},
                                       {SIGNAL_RIGHT_YELLOW, 1},
                                       {SIGNAL_DOWN_YELLOW, 1},
                                       {SIGNAL_LEFT_YELLOW, 1},
                                       {SIGNAL_UP_GREEN, 1},
                                       {SIGNAL_RIGHT_GREEN, 1},
                                       {SIGNAL_DOWN_GREEN, 1},
                                       {SIGNAL_LEFT_GREEN, 1}};
void board_t::size() {
  std::cout << boardBG.size() << ' ' << boardBG.front().size() << '\n';
}
void board_t::draw_cells(u8 &pause) {
  for (auto &row : boardBG)
    for (auto &cell : row)
      cell.draw(GetTime(), pause);
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
    Vector2 nextPos = entity.next_pos();
    if (nextPos.x == entity._dest.x && nextPos.y == entity._dest.y)
      continue;
    if (!pause) {
      auto nx = nextPos.x - entity._positions.front().x,
           ny = nextPos.y - entity._positions.front().y;
      u8 dir = (nx >= 1) ? 1 : (nx <= -1) ? 2 : (ny >= 1) ? 3 : 4;
      auto &cell = at(nextPos.y, nextPos.x);
      std::vector<Vector2> relative_positions;
      switch (dir) {
      case 1:
        relative_positions = {
            {nextPos.x + 1, nextPos.y + 0},
            {nextPos.x + 0, nextPos.y + 1},
            {nextPos.x + 0, nextPos.y - 1}
        };
        break;
      case 2:
        relative_positions = {
            {nextPos.x - 1, nextPos.y + 0},
            {nextPos.x + 0, nextPos.y + 1},
            {nextPos.x + 0, nextPos.y - 1}
        };
        break;
      case 3:
        relative_positions = {
            {nextPos.x + 0, nextPos.y + 1},
            {nextPos.x + 1, nextPos.y + 0},
            {nextPos.x - 1, nextPos.y + 0}
        };
        break;
      case 4:
        relative_positions = {
            {nextPos.x + 0, nextPos.y - 1},
            {nextPos.x + 1, nextPos.y + 0},
            {nextPos.x - 1, nextPos.y + 0}
        };
        break;
      }
      bool entityBlocking = 0;
      for (auto &other_entity : entities) {
        for (auto &relative_pos : relative_positions) {
          if (other_entity._positions.front().x == relative_pos.x &&
              other_entity._positions.front().y == relative_pos.y) {
            entityBlocking = 1;
            break;
          }
        }
        if (entityBlocking)
          break;
      }

      if (cell._t == SIGNAL_DOWN_RED || cell._t == SIGNAL_LEFT_RED ||
          cell._t == SIGNAL_RIGHT_RED || cell._t == SIGNAL_UP_RED ||
          entityBlocking)
        entity._speed = 0;
      else
        entity.move(dir);
    }
    entity.draw(pause);
  }
}
void board_t::draw_board(u8 &pause) {
  draw_cells(pause);
  draw_entities(pause);
}
cell_t &board_t::at(i32 y, i32 x) { return boardBG.at(y).at(x); }
namespace std {
template <> struct hash<Vector2> {
  size_t operator()(Vector2 const &v) const noexcept {
    return (std::hash<int>()(v.x) << 1) ^ std::hash<int>()(v.y);
  }
};
}
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
  boardBG.reserve(screenHeight / cellSizeY);
  std::ifstream file("board.brd");
  if (!file.is_open()) {
    for (uint32_t i = 0; i < screenHeight; i += cellSizeY) {
      std::vector<cell_t> row;
      for (uint32_t j = 0; j < screenWidth; j += cellSizeX) {
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
  if (x < 0 || y < 0 || x >= tableWidth || y >= tableHeight)
    return 1;
  if (at(y, x)._t == BASE_ROAD)
    return 1;
  i32 movX = x - prev.x, movY = y - prev.y;
  auto type = at(prev.y, prev.x)._c;
  if (type == ROAD_CROSS)
    return 0;
  if (movX == 0 && movY == 1) {
    return !(type == ROAD_DOWN || type == ROAD_LEFT_DOWN ||
             type == ROAD_RIGHT_DOWN || type == ROAD_UP_DOWN ||
             type == ROAD_UP_LEFT_DOWN || type == ROAD_UP_RIGHT_DOWN ||
             type == ROAD_LEFT_DOWN_RIGHT || type == ROAD_CROSS ||
             type == SIGNAL_DOWN_RED || type == SIGNAL_DOWN_YELLOW ||
             type == SIGNAL_DOWN_GREEN);
  } else if (movX == 0 && movY == -1) {
    return !(type == ROAD_UP || type == ROAD_LEFT_UP || type == ROAD_RIGHT_UP ||
             type == ROAD_UP_DOWN || type == ROAD_UP_LEFT_RIGHT ||
             type == ROAD_UP_LEFT_DOWN || type == ROAD_UP_RIGHT_DOWN ||
             type == ROAD_CROSS || type == SIGNAL_UP_RED ||
             type == SIGNAL_UP_YELLOW || type == SIGNAL_UP_GREEN);
  } else if (movX == 1 && movY == 0) {
    return !(type == ROAD_RIGHT || type == ROAD_RIGHT_UP ||
             type == ROAD_RIGHT_DOWN || type == ROAD_LEFT_RIGHT ||
             type == ROAD_UP_LEFT_RIGHT || type == ROAD_UP_RIGHT_DOWN ||
             type == ROAD_LEFT_DOWN_RIGHT || type == ROAD_CROSS ||
             type == SIGNAL_RIGHT_RED || type == SIGNAL_RIGHT_YELLOW ||
             type == SIGNAL_RIGHT_GREEN);
  } else if (movX == -1 && movY == 0) {
    return !(type == ROAD_LEFT || type == ROAD_LEFT_UP ||
             type == ROAD_LEFT_DOWN || type == ROAD_LEFT_RIGHT ||
             type == ROAD_UP_LEFT_RIGHT || type == ROAD_UP_LEFT_DOWN ||
             type == ROAD_LEFT_DOWN_RIGHT || type == ROAD_CROSS ||
             type == SIGNAL_LEFT_RED || type == SIGNAL_LEFT_YELLOW ||
             type == SIGNAL_LEFT_GREEN);
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
      if (table.at((i32)nx + (i32)ny * tableWidth) ||
          cant_move((i32)nx, (i32)ny, curr))
        continue;
      parents[{nx, ny}] = curr;
      table.at((i32)nx + (i32)ny * tableWidth) = 1;
      paths.push({nx, ny});
    }
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
