#include "cell.hpp"
#include "entity.hpp"
#include <fstream>
#include <memory>
#include <raylib.h>
#include <stack>
#include <string>
#include <vector>
class board_t {
public:
  std::unique_ptr<cell_t> &at(u32 idx, u32 idx2);
  void draw_cells();
  void draw_entities();
  void create_entity(float x, float y, Color col, Vector2 dest);
  void draw_board();
  bool valid_road(i32 x, i32 y);
  board_t() {
    std::ifstream file("board.brd");
    if (!file.is_open()) {
      for (uint32_t i = 0; i < screenHeight; i += cellSizeY) {
        std::vector<std::unique_ptr<cell_t>> row;
        for (uint32_t j = 0; j < screenWidth; j += cellSizeX) {
          row.push_back(std::make_unique<cell_t>(j, i));
        }
        boardBG.push_back(std::move(row));
      }
      return;
    }
    std::string line;
    u32 i = 0;
    while (std::getline(file, line)) {
      std::vector<std::unique_ptr<cell_t>> row;
      u32 idx = 0;
      u32 j = 0;
      for (auto &c : line) {
        row.push_back(std::make_unique<cell_t>(j, i));
        row[idx]->set(c);
        j += cellSizeX;
        idx++;
      }
      boardBG.push_back(std::move(row));
      i += cellSizeY;
    }
  }
  ~board_t() {
    std::ofstream file("board.brd");
    for (auto &row : boardBG) {
      for (auto &cell : row) {
        file << cell->info();
      }
      file << '\n';
    }
    file.close();
  }

private:
void bfs(std::vector<u8> &table,
         Vector2 &end, Vector2& start, std::stack<Vector2>& path);
  std::vector<std::vector<std::unique_ptr<cell_t>>> boardBG;
  std::vector<entity_t> entities;
  std::stack<Vector2> find_path(Vector2 start, Vector2 end);
};