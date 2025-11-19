#include "cell.hpp"
#include "definitions.hpp"
#include "entity.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <stack>
#include <string>
#include <vector>
class board_t {
public:
  cell_t &at(u32 idx, u32 idx2);
  void create_entity(float x, float y, Color col, Vector2 dest);
  void draw_board(u8 &pause);
  void size(){
    std::cout << boardBG.size() << ' ' << boardBG.front().size() << '\n';
  }
  board_t() {
    boardBG.reserve(720/cellSizeX);
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
    u32 i = 0;
    while (std::getline(file, line)) {
      std::vector<cell_t> row;
      u32 idx = 0;
      u32 j = 0;
      for (auto &c : line) {
        row.push_back(cell_t(j, i));
        row[idx].set(c);
        j += cellSizeX;
        idx++;
      }
      boardBG.emplace_back(row);
      i += cellSizeY;
    }
  }
  ~board_t() {
    std::ofstream file("board.brd");
    for (auto &row : boardBG) {
      for (auto &cell : row) {
        file << cell.info();
      }
      file << '\n';
    }
    file.close();
  }

private:
  void bfs(std::vector<u8> &table, Vector2 &end, Vector2 &start,
           std::stack<Vector2> &path);
  void draw_cells(u8 &pause);
  void draw_entities(u8 &pause);
  bool valid_road(i32 x, i32 y);
  std::vector<std::vector<cell_t>> boardBG;
  bool cant_move(i32 x, i32 y, Vector2 prev);
  std::vector<entity_t> entities;
  std::stack<Vector2> find_path(Vector2 start, Vector2 end);
};