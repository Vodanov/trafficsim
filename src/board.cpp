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
  at(y, x)._col.r += 200;
  _destinations.push_back({x, y});
}
void board_t::size() {
  std::cout << boardBG.size() << ' ' << boardBG.front().size() << '\n';
}
u32 board_t::entity_count() { return this->entities.size(); }
void board_t::draw_cells(u8 &pause, VisibleArea area) {
  double time = GetTime();
  for (u32 i = area.startY; i < area.endY; i++)
    for (u32 j = area.startX; j < area.endX; j++)
      at(i, j).draw(time, pause);
}
void board_t::get_destination(entity_t &ent) {
  if (_destinations.size() < entities.size() + 5) {
    for (u32 i = 0; i < 5; i++) {
      i32 x = GetRandomValue(0, tableWidth - 1);
      i32 y = GetRandomValue(0, tableHeight - 1);
      _destinations.push_back({static_cast<float>(x), static_cast<float>(y)});
    }
  }
  i32 x = GetRandomValue(0, _destinations.size() - 1);
  if (ent._dest == _destinations.at(x))
    get_destination(ent);
  else
    ent._dest = _destinations.at(x);
}
void board_t::create_entity(float x, float y, Color col) {

  entity_t new_ent(x, y, col);
  get_destination(new_ent);
  std::stack<Vector2> paths = find_path({x, y}, new_ent._dest);
  new_ent._path = std::move(paths);
  new_ent._has_path = 1;
  entities.push_back(std::move(new_ent));
}
void board_t::draw_entities() {
  for (auto &entity : entities) {
    entity.draw();
  }
}
void board_t::process_entities(double &time, u8 &pause) {
  for (auto &entity : entities) {
    if (time - entity._time >= ENTITY_UPDATE_TIME) {
      if (entity._position.x == -99) {
        if (GetTime() - entity._time >= ENTITY_TIMEOUT) {
          pause = 1;
          entity._position = entity._start;
          get_destination(entity);
          Vector2 start = entity._start;
          Vector2 dest = entity._dest;
          entity._path = find_path(entity._position, entity._dest);
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
      if (nx >= 1 && ny >= 1)
        dir = 4;
      else if (nx <= -1 && ny >= 1)
        dir = 5;
      else if (nx >= 1 && ny <= -1)
        dir = 6;
      else if (nx <= -1 && ny <= -1)
        dir = 7;
      auto &cell = at(nextPos.y, nextPos.x);
      if (cell._t == TileType::GRASS) {
        cell.set(directionToTileType[dir]);
      }
            dir++;
      entity.move(dir);
      // Vector2 positions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
      // Vector2 relative_position = positions[dir];
      // u8 entityBlocking = 0;
      // for (auto &other_entity : entities) {
      //   if (other_entity._position == nextPos + relative_position ||
      //       other_entity._position == nextNextPos) {
      //     entityBlocking = 1;
      //     break;
      //   }
      // }
      // dir++;
      // if (cell._t == TileType::SIGNAL_DOWN_RED ||
      //     cell._t == TileType::SIGNAL_LEFT_RED ||
      //     cell._t == TileType::SIGNAL_RIGHT_RED ||
      //     cell._t == TileType::SIGNAL_UP_RED || entityBlocking) {
      //   entity._speed = 0;
      // } else {
      //   entity.move(dir);
      //   entity._time = GetTime();
      // }
    }
  }
}
void board_t::draw_board(u8 &pause, VisibleArea area) {
  draw_cells(pause, area);
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
        row.push_back(cell_t(j, i, TileType::GRASS));
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
      row.push_back(cell_t(j, i, static_cast<TileType>(c - 70)));
      j += cellSizeX;
      idx++;
    }
    boardBG.emplace_back(row);
    i += cellSizeY;
  }
}
bool board_t::movement_not_allowed(i32 x, i32 y, Vector2 prev) {
  if (at(y, x)._t == TileType::BASE_ROAD) {
    return 1;
  } else if (at(y, x)._t == TileType::ROAD_CROSS ||
             at(prev)._t == TileType::ROAD_CROSS ||
             at(y, x)._t == TileType::GRASS ||
             at(y, x)._t == TileType::ROAD_GRASS)
    return 0;
  float movX = x - prev.x;
  float movY = y - prev.y;
  TileType type = at(prev.y, prev.x)._t;
  auto allowed_movements = allowedMovements[type];
  if (allowed_movements.size() == 0)
    return 1;
  return allowed_movements.find({movX, movY}) == allowed_movements.end();
}
struct Vec2WithCost {
  Vector2 pos;
  u32 cost;
  bool operator>(const Vec2WithCost &other) const { return cost > other.cost; }
};
u32 heuristic(Vector2 start, Vector2 end) {
  return std::sqrt(std::abs((start.x - end.x) * (start.x - end.x)) +
                   std::abs((start.y - end.y) * (start.y - end.y)));
}
void board_t::bfs(std::bitset<tableWidth * tableHeight> &table, Vector2 &end,
                  Vector2 &start, std::stack<Vector2> &path) {
  std::priority_queue<Vec2WithCost, std::vector<Vec2WithCost>,
                      std::greater<Vec2WithCost>>
      pq;

  std::unordered_map<Vector2, u32> dist;
  std::unordered_map<Vector2, Vector2> parent;

  dist[start] = 0;
  pq.push({start, 0});

  while (!pq.empty()) {
    auto [pos, cost] = pq.top();
    pq.pop();

    if (dist[pos] != cost)
      continue;

    i32 x = pos.x;
    i32 y = pos.y;

    if (x == end.x && y == end.y) {
      Vector2 cur = end;
      while (cur != start) {
        path.push(cur);
        cur = parent[cur];
      }
      path.push(start);
      return;
    }

    auto &dirs = movementOnTile[at(pos)._t];

    for (auto [dx, dy] : dirs) {
      i32 nx = x + dx;
      i32 ny = y + dy;
      if (nx < 0 || nx >= tableWidth || ny < 0 || ny >= tableHeight)
        continue;

      i32 idx = nx + ny * tableWidth;
      if (table[idx])
        continue;

      Vector2 npos{(float)nx, (float)ny};
      // bfs_search_res.push_back(npos);
      u8 diagonal = (dx != 0 && dy != 0);
      double tileCost = at(ny, nx)._cost;
      table[idx] = 1;
      double moveCost = diagonal ? tileCost * 1.141 : tileCost;
      double newCost = cost + moveCost;

      if (!dist.count(npos) || newCost < dist[npos]) {
        dist[npos] = newCost;
        parent[npos] = pos;
        pq.push({npos, static_cast<u32>(newCost)});
      }
    }
  }
}

std::stack<Vector2> board_t::find_path(Vector2 start, Vector2 end) {
  std::stack<Vector2> path;
  std::bitset<tableWidth * tableHeight> table;
  u8 found = 0;
  bfs(table, end, start, path);
  return path;
}
