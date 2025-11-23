#include "board.hpp"
#include "definitions.hpp"
#include "entity.hpp"
#include "visible_area.hpp"
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
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
void board_t::process_entities(double &time, u8& pause){
  for(auto& entity : entities){
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
        auto &cell = at(nextPos.y, nextPos.x);
        if (cell._t == TileType::GRASS) {
          cell.set(TileType::ROAD_GRASS);
        }
        Vector2 positions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        Vector2 relative_position = positions[dir];
        u8 entityBlocking = 0;
        for (auto &other_entity : entities) {
          if (other_entity._position == nextPos + relative_position ||
              other_entity._position == nextNextPos) {
            entityBlocking = 1;
            break;
          }
        }
        dir++;
        if (cell._t == TileType::SIGNAL_DOWN_RED ||
            cell._t == TileType::SIGNAL_LEFT_RED ||
            cell._t == TileType::SIGNAL_RIGHT_RED ||
            cell._t == TileType::SIGNAL_UP_RED || entityBlocking) {
          entity._speed = 0;
        } else {
          entity.move(dir);
          entity._time = GetTime();
      }
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
      row.at(idx).set(static_cast<TileType>(c - 70));
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
  return allowed_movements.find({movX, movY}) != allowed_movements.end();
}
struct Vec2WithCost {
  union {
    Vector2 pos;
    float x, y;
  };
  u32 cost;
  bool operator>(const Vec2WithCost &other) const { return cost > other.cost; }
};
void board_t::bfs(std::bitset<tableWidth * tableHeight> &table, Vector2 &end,
                  Vector2 &start, std::stack<Vector2> &path) {
  std::priority_queue<Vec2WithCost, std::vector<Vec2WithCost>,
                      std::greater<Vec2WithCost>>
      paths;
  std::unordered_map<Vector2, Vector2> parents;
  paths.push({start, 1});
  Vector2 diffs[] = {
      {1, 0},
      {-1, 0},
      {0, 1},
      {0, -1},
  };
  while (!paths.empty()) {
    u32 currCost = paths.top().cost;
    auto curr = paths.top().pos;
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
      if (nx < 0 || nx >= tableWidth || ny >= tableHeight || ny < 0)
        continue;
      if (table[(nx + ny * tableWidth)] == 1 ||
          movement_not_allowed(nx, ny, curr))
        continue;
      parents[{static_cast<float>(nx), static_cast<float>(ny)}] = curr;
      table[(nx + ny * tableWidth)] = 1;
      paths.push({{static_cast<float>(nx), static_cast<float>(ny)},
                  currCost + at(ny, nx)._cost});
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
