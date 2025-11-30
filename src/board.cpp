#include "board.hpp"
#include "definitions.hpp"
#include "entity.hpp"
#include "visible_area.hpp"
#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <cstdlib>
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
void board_t::create_desitation(float x, float y) {
  // at(y, x)._col.r += 200;
  _destinations.push_back({x, y});
}
void board_t::size() {
  std::cout << boardBG.size() << ' ' << boardBG.front().size() << '\n';
}
u32 board_t::entity_count() { return this->entities.size(); }
void board_t::update_cells(u8 &pause) {
  double time = GetTime();
  for (auto &i : boardBG) {
    for (auto &j : i) {
      j.update(time, pause);
    }
  }
}
void board_t::draw_cells(u8 &pause, VisibleArea area,
                         std::pair<u32, u32> &mouse) {
  for (u32 i = area.startY; i < area.endY; i++)
    for (u32 j = area.startX; j < area.endX; j++) {
      if (i == mouse.second && j == mouse.first) {
        auto color = at(mouse.second, mouse.first)._col;
        at(i, j)._col = {static_cast<u8>(color.r + 40),
                         static_cast<unsigned char>(color.g + 40),
                         static_cast<unsigned char>(color.b + 40), color.a};
        at(i, j).draw();
        at(i, j)._col = color;
      } else
        at(i, j).draw();
    }
}
void board_t::get_destination(entity_t &ent) {
  if (_destinations.size() >= 15) {
    ent._dest = _destinations.at(GetRandomValue(0, _destinations.size() - 1));
    return;
  }
  if (_destinations.size() < entities.size() + 5) {
    for (u32 i = 0; i < 5; i++) {
      i32 x = GetRandomValue(0, tableWidth - 1);
      i32 y = GetRandomValue(0, tableHeight - 1);
      _destinations.push_back({static_cast<float>(x), static_cast<float>(y)});
    }
  }

  int attempts = 0;
  const int max_attempts = 10;

  do {
    i32 x = GetRandomValue(0, _destinations.size() - 1);
    if (ent._dest != _destinations.at(x)) {
      ent._dest = _destinations.at(x);
      return;
    }
    attempts++;
  } while (attempts < max_attempts);

  ent._dest = _destinations.front();
}
void board_t::create_entity(float x, float y, Color col) {

  entity_t new_ent(x, y, col);
  get_destination(new_ent);
  std::stack<Vector2> paths = find_path({x, y}, new_ent._dest);
  new_ent._path = std::move(paths);
  new_ent._has_path = 0;
  if (new_ent._path.size() != 0) {
    new_ent._has_path = 1;
    new_ent._lane = GetRandomValue(0, at(y, x)._maxCapacity - 1);
    entities.push_back(std::move(new_ent));
  } else {
    new_ent._position.x = -99;
    new_ent._position.y = -99;
    new_ent._time = GetTime();
    new_ent._has_path = 0;
  }
}
void board_t::draw_entities() {
  for (auto &entity : entities) {
    entity.draw();
  }
}
void board_t::process_entities(double &time, u8 &pause) {
  for (auto &entity : entities) {
    if (entity._position.x == -99)
      continue;
    at(entity._position)._sections.at(entity._lane) = 1;
  }
  for (auto &entity : entities) {
    if (1) {
      if (entity._position.x == -99) {
        if (GetTime() - entity._time >= ENTITY_TIMEOUT) {
          pause = 1;
          entity._position = entity._start;
          get_destination(entity);
          // std::swap(entity._start,entity._dest);
          Vector2 start = entity._start;
          Vector2 dest = entity._dest;
          entity._path = find_path(entity._position, entity._dest);
          if (!entity._path.empty())
            entity._has_path = 1;
          pause = 0;
        }
        continue;
      }
      Vector2 nextPos = entity.next_pos(), nextNextPos = entity.next_next_pos();
      if (nextPos.x == entity._dest.x && nextPos.y == entity._dest.y) {
        std::swap(entity._dest, entity._start);
        entity._position.x = -99;
        entity._position.y = -99;
        entity._time = GetTime();
        entity._has_path = 0;
        continue;
      }
      auto nx = nextPos.x - entity._position.x,
           ny = nextPos.y - entity._position.y;
      u8 dir = (nx >= 1) ? 0 : (nx <= -1) ? 1 : (ny >= 1) ? 2 : 3;
      auto &cell = at(nextPos);
      if (cell._t == TileType::GRASS) {
        cell.set(TileType::ROAD_GRASS);
      }
      entity._maxSpeed = cell._maxSpeed;
      Vector2 positions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
      Vector2 relative_position = positions[dir];
      u8 entityBlocking = 0;
      auto &currCell = at(entity._position);
      u8 origLane = entity._lane;
      u8 carOnNext = 0;
      for (auto ent : entities)
        if (ent._position == nextPos && ent._lane == entity._lane)
          carOnNext = 1;

      if ((entity._speed != 0 && cell._sections[entity._lane] ||
           cell._maxCapacity <= entity._lane) ||
          carOnNext) {
        entityBlocking = 1;
        u8 leftLane = origLane - 1;
        u8 rightLane = origLane + 1;
        if (leftLane < 8) {
          if(currCell._sections[leftLane] == 0 &&
            cell._sections[leftLane] == 0){
          entity._lane = leftLane;
          entityBlocking = 0;}
        } else if (rightLane <= currCell._maxCapacity &&
                   currCell._sections[rightLane] == 0 &&
                   cell._sections[rightLane] == 0) {
          entity._lane = rightLane;
          entityBlocking = 0;
        }
      }
      if (cell._maxCapacity < entity._lane) {
        entity._lane = cell._maxCapacity - 1;
        entityBlocking = 1;
      }
      if (entity._lane != origLane) {
        currCell._sections[origLane] = 0;
        currCell._sections[entity._lane] = 1;
      }
      if (cell._t == TileType::SIGNAL_DOWN_RED ||
          cell._t == TileType::SIGNAL_LEFT_RED ||
          cell._t == TileType::SIGNAL_RIGHT_RED ||
          cell._t == TileType::SIGNAL_UP_RED || entityBlocking) {
        entity._speed = 0;
      } else {
        u8 moved = 0;
        entity.move(dir, moved);
        entity._time = GetTime();
        if (moved) {
          cell._sections[entity._lane] = 1;
          currCell._sections[entity._lane] = 0;
        }
      }
    }
  }
}
void board_t::draw_board(u8 &pause, VisibleArea area,
                         std::pair<u32, u32> &mouse) {
  update_cells(pause);
  draw_cells(pause, area, mouse);
  double time = GetTime();
  if (!pause)
    process_entities(time, pause);
  draw_entities();
}
cell_t &board_t::at(i32 y, i32 x) { return boardBG.at(y).at(x); }
cell_t &board_t::at(Vector2 v) { return boardBG.at(v.y).at(v.x); }
board_t::~board_t() {
  std::ofstream file("board.brd");
  for (auto &row : boardBG) {
    for (auto &cell : row) {
      if (cell._t != TileType::ROAD_GRASS)
        file << u8(cell.info() + CHAR_OFFSET);
      else
        file << u8(static_cast<u8>(TileType::GRASS) + CHAR_OFFSET);
      file << u8(static_cast<u8>(cell._maxCapacity) + CHAR_OFFSET);
      file << u8(static_cast<u8>(cell._maxSpeed * 100));
    }
    file << '\n';
  }
  file.close();
}
board_t::board_t() {
  entities.reserve(1000);
  boardBG.reserve(boardHeight / cellSizeY);
  std::ifstream file("board.brd");
  if (!file.is_open()) {
    for (u32 i = 0; i < boardHeight; i += cellSizeY) {
      std::vector<cell_t> row;
      for (u32 j = 0; j < boardWidth; j += cellSizeX) {
        row.push_back(cell_t(j, i, TileType::GRASS));
      }
      boardBG.emplace_back(row);
    }
    return;
  }
  std::string line;
  i32 i = 0, j = 0;
  while (std::getline(file, line)) {
    std::vector<cell_t> row;
    j = 0;
    for (size_t idx = 0; idx < line.length(); idx += 3) {
      u8 tileTypeByte = line[idx] - CHAR_OFFSET;
      u8 maxCapacity = line[idx + 1] - CHAR_OFFSET;
      u8 maxSpeed = line[idx + 2] / 100;

      TileType tileType = TileType(tileTypeByte);
      if(tileType == TileType::TARGET) _destinations.push_back({(float)j / cellSizeY,(float)i / cellSizeX});
      cell_t cell(j, i, tileType);
      cell._maxCapacity = maxCapacity;
      cell._maxSpeed = maxSpeed;

      row.push_back(cell);
      j += cellSizeX;
    }
    boardBG.emplace_back(row);
    i += cellSizeY;
  }
  file.close();
}
struct Vec2WithCost {
  Vector2 pos;
  float cost;
  bool operator>(const Vec2WithCost &other) const { return cost > other.cost; }
};
inline u32 heuristic_euclidian(Vector2 &start, Vector2 &end) {
  return std::sqrt(std::abs((start.x - end.x) * (start.x - end.x)) +
                   std::abs((start.y - end.y) * (start.y - end.y)));
}
inline u32 heuristic(Vector2 &start, Vector2 &end) {
  return std::abs((start.x - end.x) + std::abs((start.y - end.y)));
}
void board_t::bfs(std::bitset<tableWidth * tableHeight> &table, Vector2 &end,
                  Vector2 &start, std::stack<Vector2> &path) {
  std::priority_queue<Vec2WithCost, std::vector<Vec2WithCost>,
                      std::greater<Vec2WithCost>>
      paths;
  std::unordered_map<Vector2, Vector2> parents;
  parents[start] = start;
  paths.push({start, 0});
  table[static_cast<int>(start.x + start.y * tableWidth)] = 1;

  std::vector<Vector2> diffs;

  while (!paths.empty()) {
    u32 currCost = paths.top().cost;
    auto curr = paths.top().pos;
    paths.pop();

    if (curr.x == end.x && curr.y == end.y) {
      Vector2 node = end;
      while (node.x != start.x || node.y != start.y) {
        path.push(node);
        auto it = parents.find(node);
        if (it == parents.end() || it->second == node) {
          break;
        }
        node = it->second;
      }
      return;
    }

    diffs = allowedMovements[at(curr)._t];
    for (auto [x, y] : diffs) {
      i32 nx = curr.x + x;
      i32 ny = curr.y + y;

      if (nx < 0 || nx >= tableWidth || ny < 0 || ny >= tableHeight)
        continue;

      u32 index = nx + ny * tableWidth;
      if (table[index])
        continue;

      Vector2 new_pos{static_cast<float>(nx), static_cast<float>(ny)};
      // bfs_search_res.push_back(new_pos);
      parents[new_pos] = curr;
      table[index] = 1;
      paths.push(
          {new_pos, currCost + (at(ny, nx)._cost *
                                (DEFAULT_SPEED / at(ny, nx)._maxSpeed))});
    }
  }
  std::println("NOT FOUND {} {} {} {} {}", start.x, start.y, end.x, end.y,
               path.size());
}
std::stack<Vector2> board_t::find_path(Vector2 start, Vector2 end) {
  std::stack<Vector2> path;
  std::bitset<tableWidth * tableHeight> table;
  u8 found = 0;
  bfs(table, end, start, path);
  return path;
}
