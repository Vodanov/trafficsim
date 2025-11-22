#include "board.hpp"
#include "definitions.hpp"
#include "operators.hpp"
#include "visible_area.hpp"
#include <bitset>
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
namespace std {
template <> struct hash<Vector2> {
  size_t operator()(Vector2 const &v) const noexcept {
    return (std::hash<int>()(v.x) << 1) ^ std::hash<int>()(v.y);
  }
};
} // namespace std
std::unordered_map<u8, u8> tileToValue{
    {BASE_ROAD, NOWHERE},
    {ROAD_UP, UP},
    {ROAD_RIGHT, RIGHT},
    {ROAD_DOWN, DOWN},
    {ROAD_LEFT, LEFT},
    {ROAD_UP_DOWN, UP | DOWN},
    {ROAD_LEFT_RIGHT, LEFT | RIGHT},
    {ROAD_LEFT_UP, LEFT | UP},
    {ROAD_RIGHT_UP, RIGHT | UP},
    {ROAD_LEFT_DOWN, LEFT | DOWN},
    {ROAD_RIGHT_DOWN, RIGHT | DOWN},
    {ROAD_UP_LEFT_RIGHT, UP | LEFT | RIGHT},
    {ROAD_UP_LEFT_DOWN, UP | LEFT | DOWN},
    {ROAD_UP_RIGHT_DOWN, UP | RIGHT | DOWN},
    {ROAD_LEFT_DOWN_RIGHT, LEFT | DOWN | RIGHT},
    {ROAD_CROSS, UP | DOWN | LEFT | RIGHT},
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
    {SIGNAL_UP_GREEN, UP},
    {SIGNAL_RIGHT_GREEN, RIGHT},
    {SIGNAL_DOWN_GREEN, DOWN},
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
void board_t::create_desitation(float x, float y) {
  at(y, x)._col.r += 200;
  _destinations.push_back({x, y});
}
void board_t::size() {
  std::cout << boardBG.size() << ' ' << boardBG.front().size() << '\n';
}
void board_t::draw_cells(u8 &pause, VisibleArea area) {
  double time = GetTime();
  for (u32 i = area.startY; i < area.endY; i++)
    for (u32 j = area.startX; j < area.endX; j++)
      at(i, j).draw(time, pause);
}
void board_t::create_entity(float x, float y, Color col) {
  entity_t new_ent(x, y, col);
  if (!_destinations.empty()) {
    Vector2 dest = _destinations[GetRandomValue(0, _destinations.size() - 1)];
    new_ent._dest = dest;
  } else
    new_ent._dest = {0, 0};
  std::stack<Vector2> paths = find_path({x, y}, new_ent._dest);
  new_ent._path = std::move(paths);
  entities.push_back(new_ent);
}
void board_t::draw_entities(u8 &pause) {
  for (auto &entity : entities) {
    if (entity._positions.front().x == -99) {
      if (GetTime() - entity._time >= ENTITY_TIMEOUT) {
        entity._positions.front() = entity._start;
        entity._path = find_path(entity._start, entity._dest);
      }
      continue;
    }
    Vector2 nextPos = entity.next_pos(), nextNextPos = entity.next_next_pos();
    if (nextPos.x == entity._dest.x && nextPos.y == entity._dest.y) {
      std::swap(entity._dest, entity._start);
      entity._positions.front().x = -99;
      entity._positions.front().y = -99;
      entity._time = GetTime();
      continue;
    }
    if (!pause) {
      auto nx = nextPos.x - entity._positions.front().x,
           ny = nextPos.y - entity._positions.front().y;
      u8 dir = (nx >= 1) ? 0 : (nx <= -1) ? 1 : (ny >= 1) ? 2 : 3;
      auto &cell = at(nextPos.y, nextPos.x);
      Vector2 positions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
      Vector2 relative_position = positions[dir];
      bool entityBlocking = 0;
      for (auto &other_entity : entities) {
        if (other_entity._positions.front() == nextPos + relative_position ||
            other_entity._positions.front() == nextNextPos) {
          entityBlocking = 1;
          break;
        }
      }
      dir++;
      if (cell._t == SIGNAL_DOWN_RED || cell._t == SIGNAL_LEFT_RED ||
          cell._t == SIGNAL_RIGHT_RED || cell._t == SIGNAL_UP_RED ||
          entityBlocking) {
        entity._speed = 0;
      } else {
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
    for (u32 i = 0; i < boardHeight; i += cellSizeY) {
      std::vector<cell_t> row;
      for (u32 j = 0; j < boardWidth; j += cellSizeX) {
        row.push_back(cell_t(j, i));
      }
      boardBG.emplace_back(row);
    }
    return;
  }
  std::string line;
  i32 i = 0, idx = 0, j = 0;
  while (std::getline(file, line)) {
    std::vector<cell_t> row;
    idx = 0;
    j = 0;
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
    return 1;
  } else if (at(y, x)._t == ROAD_CROSS || at(prev.y, prev.x)._t == ROAD_CROSS)
    return 0;
  float movX = x - prev.x;
  float movY = y - prev.y;
  u8 type = at(prev.y, prev.x)._c;

  auto iterator = allowedMovements.find(type);
  if (iterator == allowedMovements.end())
    return 1;

  return iterator->second.find({movX, movY}) == iterator->second.end();
}
void board_t::bfs(std::bitset<tableWidth * tableHeight> &table, Vector2 &end,
                  Vector2 &start, std::stack<Vector2> &path) {
  std::queue<Vector2> paths;
  std::unordered_map<Vector2, Vector2> parents;
  paths.push(start);
  Vector2 diffs[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  while (!paths.empty()) {
    // BeginDrawing();
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
      i32 nx = curr.x + x;
      i32 ny = curr.y + y;
      if (nx < 0 || nx > boardWidth || ny >= boardHeight || ny < 0)
        continue;
      if (table[(nx + ny * tableWidth)] == 1 || cant_move(nx, ny, curr))
        continue;
      // DrawRectangle(nx * 10, ny * 10, 10, 10, {0, 0, 255, 255});
      parents[{(float)nx, (float)ny}] = curr;

      table[(nx + ny * tableWidth)] = 1;
      paths.push({(float)nx, (float)ny});
    }
    // EndDrawing();
  }
}
std::stack<Vector2> board_t::find_path(Vector2 start, Vector2 end) {
  std::stack<Vector2> path;
  // std::vector<u8> table(boardWidth / cellSizeX * boardHeight / cellSizeY, 0);
  std::bitset<tableWidth * tableHeight> table;
  u8 found = 0;
  bfs(table, end, start, path);
  return path;
}
